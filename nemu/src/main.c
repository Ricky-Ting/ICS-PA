#include <stdio.h>
#include <sys/types.h>
#include "monitor/expr.h"
#include <stdlib.h>
#include <stdint.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
	
	/* My add */
	FILE * fp=fopen("./tools/gen-expr/input","r");
	if(fp==NULL) {
		printf("File error");
		exit(1);
	}
	char e[65536];
	uint32_t ans;
	bool success;
	for(int i=0;i<100;i++) {
		fscanf(fp,"%d",&ans);
		e[0]='\0';  success=true;
		fscanf(fp,"%s",e);
		uint32_t tmp=expr(e,&success);
		if(!success) {
			printf("Match Failed at %d line,with equation: %s",i+1,e);
			exit(0);
		}
		if(tmp==ans)
			printf("Success");
		else {
			printf("Error at %d line, with the equation: %s, should get %u, but get %u",i,e,ans,tmp);
			exit(0);
		}
	}
	

	fclose(fp);


  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
