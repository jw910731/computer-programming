#include "minesweeper.h"
#include "queue.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct vec{
    i32 x, y;
};
static const i32 dx[] = {1, 1, 1, -1, -1, -1, 0, 0}, 
                 dy[] = {1, 0, -1, 1, 0, -1, 1, -1};

Game *create_game(i32 w, i32 h, i32 mines){
    Game *ret = calloc(1, sizeof(Game));
    ret->w = w; ret->h = h; ret->mines = mines;
    ret->data = calloc(h, sizeof(i32*));
    for(i32 i = 0 ; i < h ; ++i){
        ret->data[i] = calloc(w, sizeof(i32));
    }
    return ret;
}

void free_game(Game** p){
    for(i32 i = 0 ; i < (*p)->h ; ++i){
        free((*p)->data[i]);
    }
    free((*p)->data);
    free(*p);
    *p = NULL;
}

void init_game(Game* game){
    srand(time(NULL));
    // place mine
    i32 cnt = 0;
    while(cnt < game->mines){
        i32 x = rand()%game->w, y = rand()%game->h;
        if(!MINE(game->data[y][x])){
            game->data[y][x] |= 1;
            ++cnt;
        }
    }
    // generate number
    for(i32 i = 0 ; i < game->h ; ++i){ // y
        for(i32 j = 0 ; j < game->w ; ++j){ // x
            if(MINE(game->data[i][j])){
                // tag around block
                for(i32 k = 0 ; k < 8 ; ++k){
                    // border check
                    i32 tmpy = i+dy[k], tmpx = j+dx[k];
                    if(tmpy >= 0 && tmpy < game->h && tmpx >= 0 && tmpx < game->w && !MINE(game->data[tmpy][tmpx])){
                        game->data[tmpy][tmpx] |= 1<<1; //num tag
                        // add
                        // clear number segment OR calc number
                        game->data[tmpy][tmpx] = (game->data[tmpy][tmpx] & (UINT32_MAX ^ ((1<<4)-1)<<2)) | 
                                                ((NUM(game->data[tmpy][tmpx])+1)&7) << 2;
                    }
                }
            }
        }
    }
}

void print_game(Game *g){
    i32 fmt_cnt = 3;
    fputs("    ", stdout);
    for(i32 i = 0 ; i < g->w ; ++i){
        fmt_cnt += printf("%02d ", i+1);
    }
    putchar('\n');
    for(i32 i = 0 ; i <= fmt_cnt ; ++i){
        putchar('-');
    }
    putchar('\n');
    for(i32 i = 0 ; i < g->h ; ++i){ // y
        printf("%02d|", i+1);
        for(i32 j = 0 ; j < g->w ; ++j){ // x
            i32 d = g->data[i][j];
            fputs("  ", stdout);
            if(LIT(d)){
                if(ISNUM(d)){
                    putchar('0'+NUM(d));
                }
                else if(MINE(d)){
                    putchar('X');
                }
                else{
                    putchar(' ');
                }
            }
            else if (FLAG(d)){
                putchar('F');
            }
            else{
                putchar('*');
            }
        }
        putchar('\n');
    }
}

void reveal_all(Game *g){
    for(i32 i = 0 ; i < g->h ; ++i){ // y
        for(i32 j = 0 ; j < g->w ; ++j){ // x
            uint32_t d = g->data[i][j];
            if(!LIT(d)){
                g->data[i][j] |= (1>>5);
            }
        }
    }
}

bool reveal_game(Game* g, i32 _x, i32 _y){
    if(MINE(g->data[_y][_x])){
        return true;
    }
    bool **vis = calloc(g->h, sizeof(bool*));
    for(i32 i = 0 ; i < g->h ; ++i){
        vis[i] = calloc(g->w, sizeof(bool));
    }
    for(i32 i = 0 ; i < g->h ; ++i){ // y
        for(i32 j = 0 ; j < g->w ; ++j){ // x
            if(LIT(g->data[i][j])){
                vis[i][j] = true;
            }
        }
    }
    List *l = create_list();
    Node *_n = malloc(sizeof(Node));
    *_n = (Node){calloc(1, sizeof(struct vec)), NULL, NULL};
    *((struct vec*)_n->val) = (struct vec){_x, _y};
    // emplace first element in queue
    push(_n, l);
    while(l->head != NULL && l->tail != NULL){
        // pop from queue
        Node *n = pop(l);
        struct vec *tmp = (struct vec*)n->val;
        // lit up or not
        if(!vis[tmp->y][tmp->x]){
            vis[tmp->y][tmp->x] = true;
            // lit up
            g->data[tmp->y][tmp->x] |= (1<<5);
            // reach out
            // check if empty block
            if(!NUM(g->data[tmp->y][tmp->x])){
                for(i32 i = 0 ; i < 8 ; ++i){
                    i32 y = tmp->y+dy[i], x = tmp->x+dx[i];
                    // check boarder and is mine / number
                    if(y >= 0 && y < g->h && x >= 0 && x < g->w && !MINE(g->data[y][x])){
                        Node *new = malloc(sizeof(Node));
                        *new = (Node){calloc(1, sizeof(struct vec)), NULL, NULL};
                        *((struct vec*)new->val) = (struct vec){x, y};
                        push(new, l);
                    }
                }
            }
        }
        free_node(&n);
    }
    print_list(l);
    free_list(&l);
    for(i32 i = 0 ; i < g->h ; ++i){
        free(vis[i]);
    }
    free(vis);
    return false;
}

bool game_check(Game* g){
    bool ret = true;
    for(i32 i = 0 ; i < g->h && ret ; ++i){ // y
        for(i32 j = 0 ; j < g->w && ret ; ++j){ // x
            uint32_t d = g->data[i][j];
            if(!LIT(d) && !MINE(d)){
                ret = false;
            }
        }
    }
    return ret;
}