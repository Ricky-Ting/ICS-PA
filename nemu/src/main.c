//#define CHECK

#ifdef CHECK
#include<stdio.h>
#include"monitor/expr.h"
#include"nemu.h"
#endif

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

	#ifdef CHECK
	FILE *input = fopen("/home/tuomianzigan/ics2018/nemu/tools/gen-expr/input", "r");
	/*
	while(!feof(input)){
		char input_term = '\0';
		fscanf(input, "%c", input_term);
		printflog("%c", input_term);
	}
	*/
	///*
	if(input){
		uint32_t result = 0;
		uint32_t expr_result = 0;
		char expression[65536];
		bool success = true;
		while(!feof(input)){
			expression[0] = '\0';
			fscanf(input, "%u %[^\n]", &result, expression);
			if(expression[0] == '\0'){
				printflog("The empty string is scanfed\n");
				break;
			}
			expr_result = expr(expression, &success);
			if(!success){
				printflog("\"make_token\" 失败\n");
				break;
			}

			printflog("Expression: %s\nAnswer: %u\n Your answer: %u\n", expression, result, expr_result);

			if(result == expr_result){
				printflog("点一下，玩一年，计算不花一分钱\n");
			}
			else{
				printflog("算对是不可能算对的，就算从这里跳下去，死外面\n");
				//assert(0);
			}
		}
	}
	else{
		printflog("\"input\"文件打开失败\n");
	}
	//*/
	#endif

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
