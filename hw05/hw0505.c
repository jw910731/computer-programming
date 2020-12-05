#include "minesweeper.h"
#include "terminal.h"
#include "basic.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <signal.h>
#include <unistd.h>
#include <termios.h>
TerminalContext context;
void dbg();
void game();
void motion(byte, byte, byte);
void click(byte, byte, byte);
bool isTerminate(byte);
int main(){
    if(!isatty(fileno(stdout)) || !isatty(fileno(stdin))) err("You must run the program in interactive tty with mouse support.");
    bzero(&context, sizeof(context));
    // setup options
    context.inFd = fileno(stdin);
    context.outFd = fileno(stdout);
    context.newOpt.c_cflag = (B38400 | CLOCAL | CREAD);
    context.newOpt.c_lflag |= ~(ICANON | ECHO | ECHOE | ISIG);
    context.newOpt.c_oflag |= ~OPOST;
    context.newOpt.c_cc[VMIN] = 1;
    context.newOpt.c_cc[VTIME] = 0;
    // init context
    init_terminal(&context);
    // main logic
    dbg();
    return 0;
}

void dbg(){
    ClickListener listener;
    listener.click = click;
    listener.motion = motion;
    listener.isTerminate = isTerminate;
    switchMouseTrack(true);
    /*
    char c ;
    while((c = getchar())) printf("%x ", (uint32_t)c&((1<<9)-1));
    */
    listen(&listener);
    switchMouseTrack(false);
}

void motion(byte cb, byte cx, byte cy){
    switchMouseTrack(false);
    printf("%x, %x, %x\n", cb, cx, cy);
    switchMouseTrack(true);
}
void click(byte cb, byte cx, byte cy){
    switchMouseTrack(false);
    printf("%x, %x, %x\n", cb, cx, cy);
    switchMouseTrack(true);
}
bool isTerminate(byte cb){
    return false;
}

void game(){
    i32 w, h, mines;
    printf("Please enter the width (10-16): ");
    scanf("%d", &w);
    if(w < 10 || w > 16) err("Width input invalid.");
    printf(" Please enter the height (10-16): ");
    scanf("%d", &h);
    if(h < 10 || h > 16) err("Height input invalid.");
    printf("Please enter the mine number (30-90): ");
    scanf("%d", &mines);
    if(mines < 30 || mines > 90) err("Mine number input invalid.");

}
