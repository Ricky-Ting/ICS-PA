#include "nemu.h"
#include <stdlib.h>
#include <stdio.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>


bool check_parentheses(int p, int q);
uint32_t eval(int p, int q); 
int get_priority(int type);


enum {
  TK_NOTYPE = 256, TK_EQ, TK_LP, TK_RP, TK_PLUS, TK_MINU, TK_MULT, TK_DIVI,  TK_DEC, TK_HEX, TK_REG, TK_NEQ , TK_AND, TK_DEREF, TK_UND

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"0x[0-9a-fA-F]+", TK_HEX}, //Hexadecimal number
	{"\\$[A-Za-z]+",TK_REG},    // register
  {" +", TK_NOTYPE},    // spaces
	{"\\(", TK_LP},         // left parenthesis
	{"\\)",TK_RP},					// right parenthesis
	{"\\*", TK_MULT},					//multiply
	{"/", TK_DIVI},           // divide
  {"\\+", TK_PLUS},         // plus
	{"-",TK_MINU},            // minus
	{"[0-9]+", TK_DEC},    // decimal number
  {"==", TK_EQ},         // equal
	{"!=",TK_NEQ},        // not equal
	{"&&",TK_AND},        // AND
};

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

Token tokens[65536];
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
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
			

						
        position += substr_len;
		
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
					case TK_DEC : 
						tokens[nr_token].type=TK_DEC;  
						//tokens[nr_token].str=substr_start;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						break;
					case TK_HEX :
						tokens[nr_token].type=TK_HEX;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						nr_token++;
						break;
					case TK_REG :
						tokens[nr_token].type=TK_REG;
						strncpy(tokens[nr_token].str,substr_start+1,substr_len-1);
						nr_token++;
						break;
					case TK_NOTYPE :	break;
					
          default: 
						tokens[nr_token].type=rules[i].token_type;
						nr_token++;
						break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
	for(int i=0;i<nr_token;i++) {
		if(tokens[i].type=='*' && (i==0 || (tokens[i-1].type!=TK_DEC && tokens[i-1].type!=TK_HEX && tokens[i-1].type!=TK_RP && tokens[i-1].type!=TK_REG  )))
			tokens[i].type= TK_DEREF;
	}

 return (uint32_t)(eval(0,nr_token-1));


  return 0;
}

uint32_t eval(int p, int q)
{
	extern CPU_state cpu;
	extern const char *regsl[];
	extern const char *regsw[];
	extern const char *regsb[];


	if(p>q) {
		printf("This expr is invalid.\n");
		exit(0);
		return 0xffffffff;           // Use 0xffffffff to indicate the error.
	}
	else if (p==q) { 
		if(tokens[p].type==TK_DEC)
			return (uint32_t )(strtol(tokens[p].str,NULL,10));
		if(tokens[p].type==TK_HEX)
			return (uint32_t )(strtol(tokens[p].str,NULL,16));
		if(tokens[p].type==TK_REG) {
			for(int i=0;i<8;i++) 
				if(strcmp(tokens[p].str,regsl[i]))
					return reg_l(i);
			for(int i=0;i<8;i++)
				if(strcmp(tokens[p].str,regsw[i]))	
					return reg_w(i);
			for(int i=0;i<8;i++)
				if(strcmp(tokens[p].str,regsb[i]))
				return reg_b(i);
			if(strcmp(tokens[p].str,"eip"))
				return cpu.eip;
			printf("The letters should not be captitalized!\n");
		}
	}
	else if (check_parentheses(p,q)) 
		return eval(p+1,q-1);
	else if (q-p==1 && tokens[p].type==TK_DEREF) {
		return vaddr_read(eval(p+1,q),4);
	} 
	else {
		int op=TK_UND;
		int pos=-1;
		int a=p; int b=q;
		while(a<b)
		{
			int cnt=0;
			if(tokens[a].type==TK_LP) {
				do {
					if(tokens[a].type==TK_LP) 
						cnt++;
					if(tokens[a].type==TK_RP) 
						cnt--;
					a++;
				}while(cnt>0);
				a--;
			}
			else if(get_priority(tokens[a].type)!=-1 && get_priority(tokens[a].type)<=get_priority(op)) {
				op=tokens[a].type;
				pos=a;
			}
			a++;
		}
		/* debug code
		switch(op) {
			case TK_PLUS: printf("+ %d\n",pos); break;
			case TK_MINU: printf("- %d\n",pos); break;
			case TK_MULT: printf("* %d\n",pos); break;
			case TK_DIVI: printf("/ %d\n",pos); break;
		
		}
		*/

		uint32_t val1 = eval(p,pos-1);
		uint32_t val2 = eval(pos+1,q);	
	//	if(val1==0xffffffff  || val2==0xffffffff)
		//		return 0xffffffff;
		switch(op) {
			case TK_PLUS: return val1+val2;
			case TK_MINU: return val1-val2;
			case TK_MULT: return val1*val2;
			case TK_DIVI: 
				if(val2==0) { 
					printf("divide by 0\n");
					exit(0);
				}
				return val1/val2; 
			case TK_EQ: return val1==val2;
			case TK_NEQ: return val1!=val2;
			case TK_AND: return val1&&val2;
		}
	}
	return 0xffffffff;
}


bool check_parentheses(int p, int q) {
	bool flag=true;
	if(tokens[p].type!=TK_LP || tokens[q].type!=TK_RP)
		flag=false;
	int cnt=0;
	for(int i=p;i<=q;i++) {
		if(tokens[i].type==TK_LP)
			cnt++;
		if(tokens[i].type==TK_RP)
			cnt--;
		if(cnt==0 && i!=q)
			flag=false;
		if(cnt<0)
			assert(0);
	}
	if(cnt!=0) flag=false;
	return flag;
}


int get_priority(int type) {
	switch(type) {
		case TK_MULT:
		case TK_DIVI:
			return 3;

		case TK_PLUS:
		case TK_MINU:
			return 4;
		
		case TK_EQ:
		case TK_NEQ:
			return 7;

		case TK_AND:
			return 11;
		
		case TK_UND:
			return 100;
		default :
			return -1;
	}
}
