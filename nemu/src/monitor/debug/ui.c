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

static int cmd_p(char *args);

static int cmd_x(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "打印命令的帮助信息。（Display informations about all supported commands）", cmd_help },
  { "c", "继续运行被暂停的程序。（Continue the execution of the program）", cmd_c },
  { "q", "推出NEMU。（Exit NEMU）", cmd_q },
  { "si", "让程序单步执行N条指令后暂停执行，当N没有给出时，缺省为1。si [N]", cmd_si },
  { "info", "info r:打印寄存器状态/ info w:打印监视点信息（此时SUBCMD为w）", cmd_info },
  { "p", "求出表达式EXPR的结果，将结果作为起始内存地址，以16进制形式输出连续的N个4字节。p EXPR。如：p $eax + 1", cmd_p },
  { "x", "求出表达式EXPR的值，将结果作为起始内存地址，以16进制的形式输出连续的N个4字节。p EXPR。如：x 10 $esp", cmd_x },
  { "w", "当表达式EXPR的值发生变化时，暂停程序执行。w EXPR。如：w *0x2000", cmd_w },
  { "d", "删除序号为N的监视点。d N。", cmd_d }

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

static int cmd_si(char *args){
  char* arg = strtok(NULL, " ");
  int i;
  if(arg != NULL)
    //sscanf(arg, "%d", &i);
    i = atoi(arg);//如果输入不能转化成数字，atoi将返回0
  else
    i = 1;//这里是设置默认值
  cpu_exec(i);
  return 0;  
}

static int cmd_info(char *args){
  char *arg = strtok(NULL, " ");
	if(arg == NULL || !(strcmp(arg, "r") == 0 || strcmp(arg, "w") == 0)){
		printf("非法输入，第二个字符只能为r或w\n");
		return 0;
	}
  else if(strcmp(arg, "r") == 0){
    extern CPU_state cpu;
    extern const char *regsw[];
    int i;
    for(i = 0;i < 8;i++)
    {
      printf("%s\t\t\t%#x\t\t%d\n", regsw[i], cpu.gpr[i]._32, cpu.gpr[i]._32);//, *(cpu.gpr[i]._32));
      /*
      if(cpu.gpr[i]._32 < 128*1024*1024)//防止超过内存范围
        printf("\t\t%d\n",vaddr_read(cpu.gpr[i]._32,2));//输出值对应的内存的值。如果超过内存范围则不输出
      else
        printf("\n");
	*/
    }
    printf("eip\t\t\t%#x\t\t%d\n", cpu.eip, cpu.eip);//, *(cpu.gpr[9]));a  设置eip的值和对应内存的值
    /*
    if(cpu.eip < 128*1024*1024)
      printf("\t\t%d\n",vaddr_read(cpu.eip,2));
    else
      printf("\n");
    */
  }
  else if(strcmp(arg, "w") == 0){
		/*
    //printf("这个作业还没做到<^_^>\n");
		extern WP *head;
		//WP* Pointer = head;
		while(Pointer != NULL){
			printf("NO:%d\t\n", Pointer->NO);
		}
		*/
		printf("监视点信息：\n");
		printf("%s", show_watchpoint(0));
  }
  return 0;
}

static int cmd_p(char *args){
	char *arg = strtok(NULL, "\n");
	bool success = true;
	int expr_answer = expr(arg, &success);
	printf("Success:%d\tAnswer:%d\n", (int)success, expr_answer);
  return 0;
}

static int cmd_x(char *args){
  char *arg = strtok(NULL, " ");
  int num;
  sscanf(arg, "%d", &num);
  arg = strtok(NULL, " ");
  int addr;
  sscanf(arg, "%x", &addr);
  int i;
  int unit = 4;
  for(i = 0;i < num;i++,addr += unit){
    if(i%4 == 0 && i != 0)//保证开始时不换行，后面每输出4个值就换行
      printf("\n");
    if(i%4 == 0)//给每行加上第一个值对应的地址
      printf("%#x\t:\t",addr);
    printf("%#x\t",vaddr_read(addr,unit));//对应的值
  }
  printf("\n");
  return 0;
}

static int cmd_w(char *args){
	char *arg = strtok(NULL, "\n");
	new_wp(arg);
  return 0;
}

static int cmd_d(char *args){
	char *arg = strtok(NULL, " ");
	int NO = 0;
	sscanf(arg, "%d", &NO);
	/*
	extern static WP* head;
	WP* Pointer = head;
	while(Pointer != NULL){
		if(Pointer->NO == NO){
			free_wp(Pointer);
			printf("%d号监视点已被删除\n", NO);
		}
	}
	*/
	bool success = delete_watchpoint(NO);
	if(success)
		printf("删除%s成功\n", arg);
	else
		printf("删除%s失败\n", arg);
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  /*
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }
  */

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
