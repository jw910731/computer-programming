#pragma once

#include "basic.h"

#include <termios.h>

typedef struct{
    int outFd, inFd, ttyFd;
    struct termios newOpt, oldOpt;
} TerminalContext;

typedef struct{
    void (*motion)(byte, byte, byte);
    void (*click)(byte, byte, byte);
    bool (*isTerminate)(byte);
}ClickListener;

// auto register exit callback, no manual resource clean up is needed
void init_terminal(TerminalContext*);
void clean_terminal();

void switchMouseTrack(bool stat);
// exit while click event successfully triggered
void listen(ClickListener *clickListener);
int open_tty(int _fd);