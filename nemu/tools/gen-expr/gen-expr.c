#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];

static inline int choose(int n){
	return rand()%3;
}

static inline void gen_num(){
	int num = rand();
	char num_c[20];
	sprintf(num_c, "%d", num%1000);
	strcat(buf, num_c);
}

static inline void gen(char sign){
	char Sign[2];
	Sign[0] = sign;
	Sign[1] = '\0';
	strcat(buf, Sign);
}

static inline void gen_rand_op(){
	char op[2];
	switch(rand()%4){
		case 0: op[0] = '+'; op[1] = '\0'; break;
		case 1: op[0] = '-'; op[1] = '\0'; break;
		case 2: op[0] = '*'; op[1] = '\0'; break;
		case 3: op[0] = '/'; op[1] = '\0'; break;
	}
	strcat(buf, op);
}

int TIMES;
static inline void gen_rand_expr() {
	TIMES++;
	if(TIMES > 10){
		gen('1'); gen('0');
		return;
	}
	switch(choose(6)){
		case 0: case 3: gen_num(); break;
		case 1: gen('('); gen_rand_expr(); gen(')'); break;
		default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
	}
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
		buf[0] = '\0';
		TIMES = 0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
