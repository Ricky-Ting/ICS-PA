#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
	{"si","Step In",cmd_si},
	{"info","Print info of registers or watchpoints",cmd_info},
	{"x","Print the value of specific address",cmd_x},
	{"w","Set a watchpoint",cmd_w},
	{"d", "Delete a watchpoint",cmd_d}
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
	//Extract the argument 
	char *arg = strtok(NULL, " ");

	if (arg == NULL) { //if no argument given, use the default value 1
		cpu_exec(1);
	}
	else {
		int ins_num=atoi(arg); //Translate the string to int, if no number in the string, it returns 0;
		if(ins_num>0) //A postive number
			cpu_exec((uint64_t)(ins_num));
		else       // We need a positive number to execute the si.
			printf("Parameter error! Need positive number!\n");
	}
	return 0;
}

static int cmd_info(char *args) {
	extern CPU_state cpu;
	extern const char *regsl[];   // We have to get the cpu state from other files
	char *arg = strtok(NULL," "); // Extract the argument 
	if (arg == NULL) {        // No argument, print the error.
		printf("SUBCMD Needed! w or r\n");
		return 0;
	}
	if (strcmp(arg,"r")==0) {     // info r
		for(int i=0;i<8;i++)                             
			printf("%s \t %#x\n",regsl[i],reg_l(i));	 
		printf("eip \t %#x\n",cpu.eip);
	}
	else if(strcmp(arg,"w")==0) {    // info w
		walk();
	}
	else
		printf("Unkown command '%s'\n",arg);
	return 0;
}


static int cmd_x(char *args) { 
	char *arg1 = strtok(NULL," ");
	int num = atoi(arg1);

	char *arg2 = strtok(NULL," ");
	int addr=strtol(arg2,NULL,16);

	for(int i=0;i<num;i++) {
		if(i%4==0)                      // Every four print a address
			printf("%#x: ",addr);
		printf("%#x  ",vaddr_read(addr,4));
		if(i%4==3)                      // Every four in a line
			printf("\n");
		addr+=4;                       // Increase the address
	}
	printf("\n");
	return 0;
}

static int cmd_w(char *args) {
	char * args1=strtok(NULL,"\0");
	WP *tmp = new_wp();
	strcpy(tmp->e,args1);
	bool success=true;;
	tmp->value = expr(tmp->e,&success);
	if(!success) {
		printf("Invalid Expr in cmd_w\n");
		assert(0);
	}
	printf("Watchpoint %d: %s\n",tmp->NO,tmp->e);

	return 0;
}

static int cmd_d(char *args) {
	char * args1 = strtok(NULL," ");
	int num=atoi(args1);
	printf("%d\n",num);
	delete_wp(num);
	return 0;
}


void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
