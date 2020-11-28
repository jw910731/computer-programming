#include <stdio.h>
#include "basic.h"
#include "hanoi.h"

void hanoi(i32 disk, i32 from, i32 tmp, i32 dest){
	if(disk <= 1) {
		printf("move disk %d to rod %d\n", disk, dest);
		return;
	}
	hanoi(disk-1, from, dest, tmp);
	printf("move disk %d to rod %d\n", disk, dest);
	hanoi(disk-1, tmp, from, dest);
}