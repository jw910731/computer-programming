#pragma once

#include "basic.h"

// data processing macros
#define MINE(x) ((x)&1)
#define DISPLAY(x) (((x)&3) != 0)
#define ISNUM(x) ((x>>1)&1)
#define NUM(x) (((x)>>2)&7)
#define LIT(x) (((x)>>5)&1)
#define FLAG(x) (((x)>>6)&1)

typedef struct {
    i32 w, h, mines;
    // 1 => is mine or not
    // 1 => is number or not
    // 3 => number
    // 1 => lit up or not
    // 1 => flag
    uint32_t **data;
    // y -> x ([y][x])
    bool end;
} Game;

Game *create_game(i32 w, i32 h, i32 mines);
void init_game(Game*);
void free_game(Game**);

void print_game(Game*);
// return true if trying to reveal mine
bool reveal_game(Game*, i32 x, i32 y);

// return true if only mines are not lit
bool game_check(Game*);