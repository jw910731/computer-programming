#include <stdio.h>
#include "basic.h"
#include "maze.h"
#include <stdbool.h>
typedef enum{
    w_up, w_down, w_left, w_right
}Walk;

bool visit[1005][1005];
Walk stack[1000005], idx=0;

// x, y
i32 dfs(i32 i, i32 j){
    if(visit[i][j]){
        return 0; 
    }
    visit[i][j] = true;
    if(is_palace()){
        return 1;
    }
    // try up
    i32 up = move_up();
    if(up){
        i32 d = dfs(i, j+1);
        if(d){
            stack[idx++] = w_up;
            return 1;
        }
        // else
        move_down();
    }
    i32 down = move_down();
    if(down){
        i32 d = dfs(i, j-1);
        if(d){
            stack[idx++] = w_down;
            return 1;
        }
        // else 
        move_up();
    }
    i32 left = move_left();
    if(left){
        i32 d = dfs(i-1, j);
        if(d){
            stack[idx++] = w_left;
            return 1;
        }
        //else 
        move_right();
    }
    i32 right = move_right();
    if(right){
        i32 d = dfs(i+1, j);
        if(d){
            stack[idx++] = w_right;
            return 1;
        }
        // else
        move_left();
    }
    return 0;
}

int main(){ 
    if(dfs(500, 500)){ //gotcha
        puts("I am here.");
        for(i32 i = idx-1; i >= 0 ; --i){
            switch (stack[i]){
            case w_up:
                puts("Move Up.");
                break;
            case w_down:
                puts("Move Down.");
                break;
            case w_left:
                puts("Move Left.");
                break;
            case w_right:
                puts("Move Right.");
                break;
            default:
                break;
            }
        }
    }
    else{ //failed
        puts("Where are you, Christine?");
    }
    return 0;
}