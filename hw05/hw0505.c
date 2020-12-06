#include "minesweeper.h"
#include "terminal.h"
#include "basic.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <signal.h>
#include <unistd.h>
#include <termios.h>

void dbg();
void game();

void click(byte, byte, byte);
bool isTerminate(byte, byte, byte);

void click_d(byte, byte, byte);
bool isTerminate_d(byte, byte, byte);

TerminalContext context;
Game *g_game;
int main(){
    if(!isatty(fileno(stdout)) || !isatty(fileno(stdin))) err("You must run the program in interactive tty with mouse support.");
    bzero(&context, sizeof(context));
    //setup file descriptor
    context.inFd = fileno(stdin);
    context.outFd = fileno(stdout);
    // setup options
    context.newOpt.c_cflag = (B38400 | CLOCAL | CREAD);
    context.newOpt.c_lflag |= ~(ICANON | ECHO | ECHOE | ISIG);
    context.newOpt.c_cc[VMIN] = 1;
    context.newOpt.c_cc[VTIME] = 0;
    context.newOpt.c_cc[VINTR] = 3; // ^C
    
    // init context
    init_terminal(&context);
    // main logic
    game();
    return 0;
}

void dbg(){
    g_game = create_game(10, 10, 10);
    init_game(g_game);
    // setup listener
    context.listener.click = click_d;
    context.listener.motion = NULL;
    context.listener.isTerminate = isTerminate_d;
    print_game(g_game);
    listen(&context.listener);
    free_game(&g_game);
}
void click_d(byte cb, byte cx, byte cy){
    fputs(CLEAR_TERMINAL, stdout);
    print_game(g_game);
    printf("%x, %x, %x\n", cb, cx, cy);
}
bool isTerminate_d(byte cb, byte cx, byte cy){
    return false;
}

static bool stopFlag = false;
void click(byte cb, byte cx, byte cy){
    static const i32 xoff = 0x26, yoff = 0x23;
    static byte prevX = 255, prevY = 255, prev_key = -1;
    switch (cb&3){
    // release
    case 3:
        // if not drag move
        if(cx == prevX && cy == prevY){
            i32 x = (prevX - xoff)/3, y = (prevY - yoff);
            // left click
            if(prev_key  == 0){
                if(x >= 0 && x < g_game->w && y >= 0 && y < g_game->h){
                    g_game->end = reveal_game(g_game, x, y);
                    stopFlag = true;
                }   
            }
            // right click
            else if(prev_key == 2){
                uint32_t *d = &(g_game->data[y][x]);
                if(x >= 0 && x < g_game->w && y >= 0 && y < g_game->h && !LIT(g_game->data[y][x])){
                    // flag
                    if(FLAG(*d)){
                        // cancel 
                        *d = *d ^ (1<<6);
                    }
                    else{
                        // add flag
                        *d |= (1<<6);
                    }
                }
                stopFlag = true;
            }
        }
        break;
    case 0:
    case 2:
        prevX = cx;
        prevY = cy;
        prev_key = cb&3;
        stopFlag = false;
        break;
    default:
        break;
    }
}

bool isTerminate(byte cb, byte cx, byte cy){
    return stopFlag;
}

void game(){
    // setup contexts
    context.listener.click = click;
    context.listener.motion = NULL;
    context.listener.isTerminate = isTerminate;
    // start game
    i32 w, h, mines;
    puts(CLR_LRED"This is NOT a regular version of minesweeper.");
    puts("Further explain please see the README."CLR_RST);
    printf("Please enter the width (10-16): ");
    scanf("%d", &w);
    if(w < 10 || w > 16) err("Width input invalid.");
    printf(" Please enter the height (10-16): ");
    scanf("%d", &h);
    if(h < 10 || h > 16) err("Height input invalid.");
    printf("Please enter the mine number (10-90): ");
    scanf("%d", &mines);
    if(mines < 10 || mines > 90) err("Mine number input invalid.");
    g_game = create_game(w, h, mines);
    init_game(g_game);
    while(!g_game->end){
        stopFlag = false;
        fputs(CLEAR_TERMINAL, stdout);
        print_game(g_game);
        for(i32 i = 0 ; i < g_game->h ; ++i){
            for(i32 j = 0 ; j < g_game->w ; ++j){
                debug("%2x ", g_game->data[i][j] & ((1<<9)-1));
            }
            debug("\n");
        }
        listen(&context.listener);
        bool ck = game_check(g_game);
        if(g_game->end){
            reveal_all(g_game);
            fputs(CLEAR_TERMINAL, stdout);
            print_game(g_game);
            printf(CLR_RED"You had revealed the mine. >.<\n"CLR_RST);
        }
        if(ck){
            reveal_all(g_game);
            fputs(CLEAR_TERMINAL, stdout);
            print_game(g_game);
            printf(CLR_LGRN"You had reveal the entire field without touching any mine!\nYou Win!"CLR_RST);
            break;
        }
    }
    free_game(&g_game);
}
