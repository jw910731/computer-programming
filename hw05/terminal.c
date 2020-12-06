#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

typedef enum{
     a, b
}ascii_state;

TerminalContext *g_context;
void term_callback(int n);

void init_terminal(TerminalContext *context){
    g_context = context;
    // set callback
    atexit(clean_terminal);
    signal(SIGINT, term_callback);
    // disable IO buf
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    // setup context
    context->ttyFd = open_tty(context->inFd);
    tcgetattr(context->ttyFd, &(context->oldOpt));
}

void switchMouseTrack(bool stat){
    if(stat){
        // setup termios
        tcflush(g_context->ttyFd, TCIFLUSH);
        tcsetattr(g_context->ttyFd, TCSANOW, &(g_context->newOpt));
        // enable mouse tracking
        printf("\e[?1003h");
    }
    else{
        // disable mouse tracking
        printf("\e[?1003l");
        tcflush(g_context->ttyFd, TCIFLUSH);  
        tcsetattr(g_context->ttyFd, TCSANOW, &g_context->oldOpt);
    }
}

void clean_terminal(){
    switchMouseTrack(false);
    // cleanup context
    close(g_context->ttyFd);
}

int open_tty(int _fd){
    int fd;
    fd = open(ttyname(_fd), O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd == -1){
        perror("open error");
    }
    return fd;
}

//callback wrapper
void term_callback(int n){
    clean_terminal();
    exit(1);
    signal(SIGINT, SIG_DFL);
}

void listen(ClickListener *clickListener){
    static const char target[] = "\e[";
    static const size_t tsize = 2;
    i32 counter = 0;
    switchMouseTrack(true);
    while(1){
        byte c;
        c = getchar() & ((1<<9)-1);
        if(c == target[counter] && counter < (i32)tsize){
            ++counter;
        }
        else{
            if(counter >= (i32)tsize){
                // enter control state
                if(c == 'M'){
                    byte cb = getchar() & ((1<<9)-1);
                    byte cx = getchar() & ((1<<9)-1);
                    byte cy = getchar() & ((1<<9)-1);
                    if(cb & (1<<6) && clickListener->motion != NULL){
                        switchMouseTrack(false);
                        clickListener->motion(cb, cx, cy);
                        switchMouseTrack(true);
                    }
                    else if(cb & (1<<5) && clickListener->click != NULL){
                        switchMouseTrack(false);
                        clickListener->click(cb, cx, cy);
                        switchMouseTrack(true);
                    }
                    // check if is goint to terminate
                    if(clickListener->isTerminate(cb, cx, cy)){
                        switchMouseTrack(false);
                        break;
                    }
                }
            }
            if(!counter && c == 'q'){
                switchMouseTrack(false);
                printf("See You Next Time...\n");
                exit(0);
            }
            counter = 0;
        }
    }
    switchMouseTrack(false);
}