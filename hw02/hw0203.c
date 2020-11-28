#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
typedef int64_t i64;
typedef int32_t i32;
const double rate[]={.05, .12, .2, .3, .4};
const i32 discount[]={0, 37800, 134600, 376600, 829600};
const i64 range[]={540000, 1210000, 2420000, 4530000};
void input(char *format, void *v){
	int stat;
	stat = scanf(format, v);
	if(stat == EOF) exit(0);
	if(stat < 1){
		printf("Input Error Occurred, Program will be terminated.\n");
		exit(1);
	}
}
int main(){
	i64 salary, family, income_people, deduction_saving, deduction_tuition, preschool_child, disabled_people;
	i32 married;
	printf("Gross salary:");
	input("%ld", &salary);
	printf("Married? (0/1): ");
	input("%d", &married);
	if(married & (UINT_MAX>>1)<<1){
		printf("Please input 0 or 1. Program will be terminated.\n");
		exit(1);
	}
	printf("How many people are there in your family: ");
	input("%ld", &family);
	printf("How many people in your family have income: ");
	input("%ld", &income_people);
	printf("Special Deduction for Savings and Investment: ");
	input("%ld", &deduction_saving);
	printf("Special Deduction for Tuition: ");
	input("%ld", &deduction_tuition);
	printf("How many preschool children: ");
	input("%ld", &preschool_child);
	printf("How many disable people: ");
	input("%ld", &disabled_people);
	i64 tax = salary - (family*88000) - (income_people*200000) - ((married+1)*120000) - (deduction_saving*270000) - (deduction_tuition*25000) - (preschool_child*120000) - (disabled_people*200000);
	i32 stat;
	if(tax < 0) tax = 0;
	for(stat = 0 ; stat < 4 ; ++stat){
		if(tax <= range[stat]){
			break;
		}
	}
	printf("Total tax: %ld", (i64)(tax*rate[stat]) - discount[stat]);
	return 0;
}