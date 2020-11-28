#include <stdio.h>
#include "basic.h"
#include "hanoi.h"

struct para{
	i32 disk, from, tmp, dest;
	bool opt;
};

void hanoi(i32 _disk, i32 _from, i32 _tmp, i32 _dest){
	i32 ptr = 0;
	struct para stk[1000];
	stk[ptr++] = (struct para){_disk, _from, _tmp, _dest, false};
	while(ptr){
		struct para now = stk[--ptr];
		if(now.opt){
			printf("move disk %d to rod %d\n", now.disk, now.dest);
		}
		else{
			stk[ptr++] = (struct para){now.disk-1, now.tmp, now.from, now.dest, (now.disk-1)<=1};
			stk[ptr++] = (struct para){now.disk, now.from, now.tmp, now.dest, true};
			stk[ptr++] = (struct para){now.disk-1, now.from, now.dest, now.tmp, (now.disk-1)<=1};
		}
	}
}