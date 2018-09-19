#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];

uint32_t choose(uint32_t n) {
	return rand()%n;
}

void gen_num(void) {
	uint32_t number= (uint32_t)(rand()%100); 
	uint32_t tmp=(((uint32_t)(rand()%65536))<<16);
	if(choose(10)==11)
		number+=tmp;
	char str[32];
	sprintf(str,"%u",number);
	strcat(buf,str);
	return;
}

void gen_rand_op() {
	switch(choose(4)) {
		case 0 : strcat(buf,"+"); break;
		case 1 : strcat(buf,"+"); break;
		case 2 : strcat(buf,"*"); break;
		case 3 : strcat(buf,"/"); break;
	}
	return;
}



static inline void gen_rand_expr() {
  switch(choose(3)) {
		case 0: 
			gen_num();
			break;
		case 1: 
			strcat(buf,"(");
		//	if(strlen(buf)>100)
			//	gen_num();
		//	else
				gen_rand_expr();
			strcat(buf,")");
			break;
		default:
			if(strlen(buf)>20)
				gen_num();
			else
			 gen_rand_expr();
			gen_rand_op();
			if(strlen(buf)>20)
				gen_num();
			else
				gen_rand_expr();
			break;
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
		buf[0]='\0';	
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
