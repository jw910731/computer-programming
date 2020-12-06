#pragma once

#include "basic.h"

#include <termios.h>

#define CLEAR_TERMINAL "\033c"

typedef struct{
    void (*motion)(byte, byte, byte);
    void (*click)(byte, byte, byte);
    bool (*isTerminate)(byte, byte, byte);
}ClickListener;

typedef struct{
    int outFd, inFd, ttyFd;
    struct termios newOpt, oldOpt;
    ClickListener listener;
} TerminalContext;



// auto register exit callback, no manual resource clean up is needed
void init_terminal(TerminalContext*);
void clean_terminal();

void switchMouseTrack(bool stat);
// exit while click event successfully triggered
void listen(ClickListener *clickListener);
int open_tty(int _fd);