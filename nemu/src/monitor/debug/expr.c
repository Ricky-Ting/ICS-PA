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



enum {
  TK_NOTYPE = 256, TK_EQ, TK_LP, TK_RP, TK_PLUS, TK_MINU, TK_MULT, TK_DIVI,  TK_DEC

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
	{"\\(", TK_LP},         // left parenthesis
	{"\\)",TK_RP},					// right parenthesis
	{"\\*", TK_MULT},					//multiply
	{"/", TK_DIVI},           // divide
  {"\\+", TK_PLUS},         // plus
	{"-",TK_MINU},            // minus
	{"[0-9]+", TK_DEC},    // decimal number
  {"==", TK_EQ}         // equal
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
    return 0xffffffff;
  }

  /* TODO: Insert codes to evaluate the expression. */
 return  eval(0,nr_token-1);


  return 0;
}

uint32_t eval(int p, int q)
{
	if(p>q) {
		printf("This expr is invalid.\n");
		exit(0);
		return 0xffffffff;           // Use 0xffffffff to indicate the error.
	}
	else if (p==q) 
	{
		return (uint32_t)(strtol(tokens[p].str,NULL,10));
	}
	else if (check_parentheses(p,q)) 
		return eval(p+1,q-1);
	else {
		int op=-1;
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
			else if(tokens[a].type==TK_PLUS || tokens[a].type==TK_MINU) {
					op=tokens[a].type;
					pos=a;
			}
			else if(tokens[a].type==TK_MULT || tokens[a].type==TK_DIVI) {
				if(op==-1 || op==TK_MULT || op==TK_DIVI) {
					op=tokens[a].type;
					pos=a;
				}
			}
			a++;
		}
		switch(op) {
			case TK_PLUS: printf("+\n"); break;
			case TK_MINU: printf("-\n"); break;
			case TK_MULT: printf("*\n"); break;
			case TK_DIVI: printf("/\n"); break;
		
		}

		uint32_t val1 = eval(p,pos-1);
		uint32_t val2 = eval(pos+1,q);	
		if(val1==0xffffffff  || val2==0xffffffff)
				return 0xffffffff;
		switch(op) {
			case TK_PLUS: return val1+val2;
			case TK_MINU: return val1-val2;
			case TK_MULT: return val1*val2;
			case TK_DIVI: return val1/val2; 
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
