#include "undo.h"
#include <stdlib.h>

UndoQueue *create_undo_queue(i32 cap){
    UndoQueue *ret = calloc(1, sizeof(UndoQueue));
    ret->l = create_list();
    ret->cap = cap;
    ret->len = 0;
    return ret;
}

void free_undo_queue(UndoQueue **p){
    free_list(&((*p)->l));
    free(*p);
    *p = NULL;
}

void head_clean(UndoQueue *u){
    //debug("HAIYA!\n");
    Node *h = u->l->head;
    h->prev->next = NULL;
    Node *it = h->prev;
    // clean head prev
    h->prev = NULL;
    // free orphan linked list chain (in reverse order)
    while(it != NULL){
        Node *tmp = it->prev;
        free_node(&it);
        it = tmp;
    }
}

void u_push(Node *n, UndoQueue *u){
    // if head is not the real head
    if(u->l->head != NULL && u->l->head->prev != NULL){
        head_clean(u);
    }
    // if exceed capacity
    //debug("!%d %d\n", u->len, u->cap);
    if(u->len >= u->cap){
        Node *tmp = l_pop(u->l);
        free_node(&tmp);
    }
    // push element in
    l_push(n, u->l);
    ++u->len;
}

void u_redo(UndoQueue *u){
    if(u->l->head != NULL && u->l->head->prev != NULL){
        ++u->len;
        u->l->head = u->l->head->prev;
    }
}

void u_undo(UndoQueue *u){
    if(u->l->head != NULL && u->l->head->next != NULL){
        --u->len;
        u->l->head = u->l->head->next;
    }
}

void u_close(UndoQueue *u){
    if(u->l->head != NULL && u->l->head->prev != NULL){
        head_clean(u);
    }
}