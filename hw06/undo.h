#pragma once
#include "basic.h"
#include "queue.h"

typedef struct{
    // inheritance
    List *l;
    i32 len, cap;
}UndoQueue;

UndoQueue *create_undo_queue(i32 cap);
void free_undo_queue(UndoQueue **p);
void u_push(Node *n, UndoQueue *u);
void u_redo(UndoQueue *u);
void u_undo(UndoQueue *u);
void u_close(UndoQueue *u);