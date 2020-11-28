#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic.h"
#include "guess_number.h"
#include "color.h"
void game();
void test();
i32 easter_egg(char *s);
void rainbow(const char *s);
int main(){
	game();
	return 0;
}

void test(){
	printf(CLR_CYN"HI\n"CLR_RST);
}

void game(){
	printf(CLR_24_FG("187","0","187")"Bulls and Cows Game\n"CLR_RST);
	i32 gameNum = 4;
	i32 cnt = 1, ans = number_gen(gameNum);
	do{
		char s[100];
		printf(CLR_BOLD"Round" CLR_GRN" %d "CLR_RST CLR_BOLD">>>\n"CLR_RST, cnt);
		printf("\tYour Guess: "CLR_GRY);
		fgets(s, 100, stdin);
		printf(CLR_RST);
		i32 huh = easter_egg(s);
		if(huh <= -127) {
			if(gameNum <= 4){
				rainbow("\tYou know too much >.<\n");
				printf(CLR_BOLD"\tYou Know Something More: "CLR_RST"%0*d\n", gameNum, ans);
			}
			else
				rainbow("\tNo hateness is allowed in the new world!\n");
			continue;
		}
		if(huh == 2147483647){
			printf(CLR_BOLD CLR_LGRN"\tGreat: "CLR_RST"%0*d\n", gameNum, ans);
			continue;
		}
		if(huh >= 128){
			
			rainbow("===================================\n");
			rainbow("Choose a number: ");
			i32 tmp;
			scanf("%d", &tmp);
			getchar(); // to eliminate newline character
			if(tmp > 9 || tmp < 4){

				rainbow("\tAn error occurred during importing magic!\n");
				continue;
			}
			gameNum = tmp;
			rainbow("A New World Had Been Opened for YOU!\n");
			rainbow("Now the number of digit of the answer is the number you input\n");
			cnt = 1;
			ans = number_gen(gameNum);
			continue;
		}
		if(huh) continue;
		i32 len = strlen(s), guess_num = atoi(s);	
		if(len != gameNum || !unique_ck(guess_num, gameNum)){
			printf(CLR_CYN"Input Invalid\n"CLR_RST);
			continue;
		}
		printf("\tResponse: "); 
		bool ret = guess(ans, guess_num, gameNum);
		putchar('\n');
		if(!ret) break;
		++cnt;
	}while(1);
}

i32 easter_egg(char *s){
	char *tmp = s;
	while(*tmp!=0){
		if(*tmp == '\n') *tmp = 0;
		++tmp;
	}
	if(strcmp(s, "Hello World") == 0){
		putchar('\t');
		rainbow("Hello World");
		putchar('\n');
		return true;
	}
	if(strcmp(s, "quit") == 0 || strcmp(s, "exit") == 0){
		printf(CLR_BOLD"See You Next Time\n"CLR_RST);
		exit(0);
	}
	if(strstr(s, "Chi") != NULL && (strstr(s, "hate") != NULL|| strstr(s, "Hate") != NULL)){
		return -127;
	}
	if((strstr(s, "import") != NULL || strstr(s, "Import")!=NULL) && (strstr(s, "Magic")!=NULL || strstr(s, "magic") != NULL)){
		putchar('\t');
		rainbow("Magic is importing...");
		putchar('\n');
		return 128;
	}
	if(strstr(s, "God") != NULL && strstr(s, "bless") != NULL){
		return 2147483647;
	}
	return false;
}

void rainbow(const char *s){
	static const char *list[]={CLR_LRED, CLR_LGRN, CLR_LYLW, CLR_LBL, CLR_LMGT, CLR_LCYN};
	while(*s != 0){
		printf("%s%c"CLR_RST, list[rand()%(sizeof(list)/sizeof(*list))],*s);
		++s;
	}
}