#include "nemu.h"
#include "monitor/expr.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

#include<string.h>
#include<stdio.h>

enum {
  TK_NOTYPE = 256, TK_DI, TK_HE, TK_RE, TK_EQ, TK_NE, TK_AN, TK_DR

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},          // spaces
  {"\\+", '+'},               // plus
	{"-",   '-'},               // sub
	{"\\*",   '*'},             // mult
	{"/",   '/'},               // divide

	{"\\(",   '('},             // left bracket
	{"\\)",   ')'},             // right bracket

	{"\\b[0-9]+\\b", TK_DI},    // digit
	{"\\b0x[0-9a-z]+\\b", TK_HE},  // hexadeimal numbr
	{"\\$[a-z]{2,3}", TK_RE},     // register

  {"==", TK_EQ},              // equal
	{"!=", TK_NE},
	{"&&", TK_AN}              // and
};

int get_priority(int sign){
	switch(sign){
		case TK_DR:
			return 2;
		case '*':
		case '/':
			return 3;
		case '+':
		case '-':
			return 4;
		case TK_EQ:
		case TK_NE:
			return 7;
		case TK_AN:
			return 11;
		default:
			return 0;
	}
}

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				//Log("expression:%s", e+position);
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            //i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
					case TK_NOTYPE:
						break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case TK_EQ:
					case TK_NE:
					case TK_AN:
						tokens[nr_token].type = rules[i].token_type;
						nr_token++;
						break;

					case TK_DI:
					case TK_HE:
					case TK_RE:
						tokens[nr_token].type = rules[i].token_type;
						if(substr_len > 31){
							//Log("Warning: The length of digital number is longer than 31. Only higher 31 bit will be token.\n"); //used to warning that the answer maybe wrong
						}
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						//tokens[nr_token].str[31] = '\0';
						//Log("the stored things in tokens[nr_token].str::%s", tokens[nr_token].str);
						//Log("nr_token:%d", nr_token);
						nr_token++;
						break;

          default: //TODO();
						Log("Warning: Un expected sign in make_token() in expr.c \n");
						//assert(0);
        }
				//Log("the stored things in tokens[nr_token-1].type::%u\t%c", (uint32_t)tokens[nr_token-1].type, tokens[nr_token-1].type);
				//Log("(int)e[position]:%d\tposition:%d", (int)e[position], position);
        break;
      }
    }

		if(e[position] == '\0')// used to exclude the next judgement
				break;

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

inline bool is_certain_type(int type){
	return !(type == TK_DI || type == TK_HE || type == TK_RE);
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
	else
		*success = true;

	for(int i = 0;i < nr_token;i++){
		if(tokens[i].type == '*' && (i == 0 || is_certain_type(tokens[i-1].type))){
			tokens[i].type = TK_DR;
		}
	}

  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
	uint32_t answer = eval(0, nr_token-1);
	//Log("answer:::%u", answer);
	return answer;
  //return 0;
}

inline bool check_parentheses(int p, int q){
	if(tokens[p].type == '(' && tokens[q].type == ')'){
		int counter = 1;
		for(int i = p+1;i <= q-1;i++){
			if(tokens[i].type == '(')
				counter++;
			if(tokens[i].type == ')')
				counter--;
			if(!counter)
				return false;
		}
	}
	else
		return false;
	
	return true;
}

uint32_t get_register_num(char* String, int p, int q){
	extern CPU_state cpu;
	//Log("strlen(String):%u", (unsigned)strlen(String));
	if(strlen(String) != 3 && strlen(String) != 4){
		//Log("Error: The length of the string of resigter is not 3 or 4, but %d", (uint32_t)strlen(String));
			return vaddr_read(eval(p+1, q), 4);
		assert(0);
	}

	if(String[1] == 'e'){
		switch(String[2]){
			case 'a':
				if(String[3] == 'x')
					return cpu.eax;
				assert(0);
			case 'c':
				if(String[3] == 'x')
					return cpu.ecx;
				assert(0);
			case 'd':
				if(String[3] == 'x')
					return cpu.edx;
				if(String[3] == 'i')
					return cpu.edi;
				assert(0);
			case 'b':
				if(String[3] == 'x')
					return cpu.ebx;
				if(String[3] == 'p')
					return cpu.ebp;
				assert(0);
			case 's':
				if(String[3] == 'p')
					return cpu.esp;
				if(String[3] == 'i')
					return cpu.esi;
				assert(0);
			case 'i':
				if(String[3] == 'p')
					return cpu.eip;
				assert(0);
		}
	}
	else{
		switch(String[1]){
			case 'a':
				if(String[2] == 'x')
					return cpu.gpr[0]._16;
				if(String[2] == 'l')
					return cpu.gpr[0]._8[0];
				if(String[2] == 'h')
					return cpu.gpr[0]._8[1];
				assert(0);
			case 'c':
				if(String[2] == 'x')
					return cpu.gpr[1]._16;
				if(String[2] == 'l')
					return cpu.gpr[1]._8[0];
				if(String[2] == 'h')
					return cpu.gpr[1]._8[1];
				assert(0);
			case 'd':
				if(String[2] == 'x')
					return cpu.gpr[2]._16;
				if(String[2] == 'l')
					return cpu.gpr[2]._8[0];
				if(String[2] == 'h')
					return cpu.gpr[2]._8[1];

				if(String[2] == 'i')
					return cpu.gpr[7]._16;
				assert(0);
			case 'b':
				if(String[2] == 'x')
					return cpu.gpr[3]._16;
				if(String[2] == 'l')
					return cpu.gpr[3]._8[0];
				if(String[2] == 'h')
					return cpu.gpr[3]._8[1];

				if(String[2] == 'p')
					return cpu.gpr[5]._16;
				assert(0);
			case 's':
				if(String[2] == 'p')
					return cpu.gpr[4]._16;
				if(String[2] == 'i')
					return cpu.gpr[6]._16;
				assert(0);

		}
	}

	assert(0);
	return 0;
}

uint32_t eval(int p, int q){
	if(p > q){
		//Log("Enter eval() \"p>q\"");
		assert(0);
		return 0;
	}

	else if(p == q){
		//Log("Enter eval() \"p==q\"");
		uint32_t num = 0;
		switch(tokens[p].type){
			case TK_DI:
				sscanf(tokens[p].str, "%u", &num);
				break;
			case TK_HE:
				sscanf(tokens[p].str, "%x", &num);
				break;
			case TK_RE:
				num = get_register_num(tokens[p].str, p, q);
				break;
		}
		//sscanf(tokens[p].str, "%x", &num);
		//Log("char* kinds of num::%s", tokens[p].str);
		//Log("type:%d", tokens[p].type);
		//Log("p:::%d", p);
		//Log("num in (p==q)::%u", num);
		return num;
	}

	else if(check_parentheses(p, q)){
		//Log("Enter eval() \"check_parentheses(%d, %d)\"", p, q);
		return eval(p+1, q-1);
	}

	else{
		//Log("Enter eval() else");
		int num_of_right_bracket = 0;// Used to record the number of let bracket
		int main_prio = 0;
		int main_posi = -1;
		for(int i = q;i >= p;i--){
			if(tokens[i].type == ')'){
				num_of_right_bracket++;
				continue;
			}
			if(tokens[i].type == '('){
				num_of_right_bracket--;
				continue;
			}
			if(num_of_right_bracket)
				continue;

			if(tokens[i].type == TK_DI || tokens[i].type == TK_HE)
				continue;
			
			//Log("get_priority(tokens[i].type): %d\ttokens[i].type: %d\tmain_prio: %d", get_priority((int)tokens[i].type), (int)tokens[i].type,  main_prio);
			if(get_priority((int)tokens[i].type) > main_prio){
				//Log("tokens[i].type: %c\ti: %d", tokens[i].type, i);
				main_prio = get_priority((int)tokens[i].type);
				main_posi = i;
			}
		}
		//Log("main_prio: %c\tmain_posi: %d", tokens[i].type, i);

		if(tokens[main_posi].type == TK_DR){// used to del with dereference
			return vaddr_read(eval(p+1, q), 4);
		}

		uint32_t val1 = eval(p, main_posi-1);
		uint32_t val2 = eval(main_posi+1, q);

		//Log("val1: %d\tval2: %d", val1, val2);

		switch(tokens[main_posi].type){
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/':
				if(!val2){
					//Log("Warning: 0 cannot used as divisor.");
					return 0;
				}
				else
					return val1 / val2;
			
			case TK_EQ: return val1 == val2;
			case TK_NE: return val1 != val2;
			case TK_AN: return val1 && val2;
			default: 
				//Log("Error in eval(), 'takens[main_posi].type' is not reconcile");
			assert(0);
		}
	}
	return 0;
}
