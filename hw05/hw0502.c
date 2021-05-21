//
// Created by jw910731 on 2021/5/21.
//

#include "basic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

sig_atomic_t int_flag = 0;

void int_hand(int signum){
    int_flag = 1;
}

int main(){
    FILE *f = fopen("/proc/stat", "r");
    if(f == NULL){
        perror("File open error");
        fputs("System may not support /proc filesystem.\n", stderr);
        return 1;
    }
    struct sigaction interrupt_handler = {0};
    interrupt_handler.sa_handler = int_hand;
    sigaction(SIGINT, &interrupt_handler, NULL);
    i64 prevSum = 0, prevIdle = 0;
    while(1){
        static char buf[2048];
        fgets_n(buf, 2048, f);
        if(int_flag){
            printf("\rBye~\n");
            break;
        }
        // tokenize
        char *saveptr;
        strtok_r(buf, " ", &saveptr);
        i64 sum = 0, idle = 0;
        i32 cnt = 0;
        for(char *it = strtok_r(NULL, " ", &saveptr); it != NULL ; it = strtok_r(NULL, " ", &saveptr), ++cnt){
            const i64 tmp = (i64)strtoll(it, NULL, 10);
            if(cnt == 3){
                idle = tmp;
            }
            sum += tmp;
        }
        double idleFraction = 100 - (idle-prevIdle)*100.0 / (sum-prevSum);
        printf("CPU usage: %.2lf%%\n", idleFraction);
        prevIdle = idle;
        prevSum = sum;
        sleep(1);
    }
    fclose(f);
    return 0;
}