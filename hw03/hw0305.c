#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#define LEN(X) (sizeof(X)/sizeof(*X))
typedef double fp;
void err(const char*);
int max(int, int);
bool sorted(fp*, uint64_t);
bool positive(fp*, uint64_t);
void positiveErr(int);
int main(){
	int32_t annPay, feeOff, procFee, age, payYear, retRate;
	fp insFee[5];
	printf("SIP Plan\nAnnually payment: ");
	scanf("%d", &annPay);
	positiveErr(annPay);
	printf("Insurance fee in the first five years (0 is assumed afterwards):\n");
	for(int32_t i = 0 ; i < 4 ; ++i)
		scanf(" %lf ,", insFee+i);
	scanf(" %lf", &insFee[4]);
	if(!sorted(insFee, LEN(insFee)) || !positive(insFee, LEN(insFee)))
		err("Insureance fee in first five years must be decremental and positive.");
	printf("Insurance fee off: ");
	scanf("%d", &feeOff);
	positiveErr(feeOff);
	printf("Monthly insurance processing fee: ");
	scanf("%d", &procFee);
	positiveErr(procFee);
	printf("Age: ");
	scanf("%d", &age);
	if(age < 0) err("This plan is not available for baby before born.");
	if(age > 100) err("Age greater than 100 is not allowed.");
	printf("How many years of payment: ");
	scanf("%d", &payYear);
	positiveErr(payYear);
	if(100 - age < payYear) err("You wouldn't pay all fee when the calculate ends.");
	printf("Expected annual return on investment rate: ");
	scanf("%d", &retRate);
	positiveErr(retRate);
	fp mRate = round((retRate/12.0)*1000.0)/1000.0;
	printf("------------------------------\nYour Payment and Account Value Table\n");
	double ans = 0;
	int32_t pay = 0;
	for(int32_t i = age ; i <= 100 ; ++i){
		int paidYear = i - age;
		if(paidYear < payYear){
			pay += annPay;
			// pay less than 5 years
			if(paidYear < 5){
				ans += annPay * ( 100-insFee[paidYear]+feeOff ) / 100 - 100 * ( max(i, 20) - 20 ) - 2000;
			}
			else if(paidYear >= 5){ // pay more than 5 years (strictly larger)
				ans += annPay * ( 100+feeOff ) /100 - 100 * ( max(i, 20) - 20 ) - 2000;
			}
		}
		for(int32_t j = 0; j < 12 ; ++j){
			// skip first month calculate
			if(i == age && j == 0) continue;
			ans -= procFee;
			ans += ans * mRate/100;
		}
		printf("%d: %d, %lf\n", i, pay, ans);
		if(ans < 0){
			printf("Account value less than 0.\n");
			break;
		}
	}
	return 0;
}

void positiveErr(int val){
	if(val < 0) err("This input value must be positive");
}

void err(const char *s){ // error handler
	fprintf(stderr, "%s\nFatal error occured and thus program is going to terminate.\n", s);
	exit(1);
}

int max(int a, int b){
	return (a>b)?a:b;
}
// check if array is decremental
bool sorted(fp *arr, uint64_t size){
	for(uint64_t i = 0 ; i < size-1 ; ++i){
		if(arr[i] < arr[i+1]){
			return false;
		}
	}
	return true;
}
bool positive(fp *arr, uint64_t size){
	for(uint64_t i = 0 ; i < size ; ++i){
		if(arr[i] < 0){
			return false;
		}
	}
	return true;
}