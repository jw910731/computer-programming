#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

List *create_list(){
    List *list = calloc(1, sizeof(List));
    list->head = list->tail = NULL;
    return list;
}

void free_list(List **l){
    Node *it = (*l)->head;
    while(it != NULL){
        Node *tmp = it->next;
        free_node(&it);
        it = tmp;
    }
    free(*l);
    *l = NULL;
}

void free_node(Node **n){
    free((*n)->val);
    free(*n);
    *n = NULL;
}

void push(Node *n, List *l){
    Node *h = l->head;
    l->head = n;
    n->next = h;
    if(h != NULL)
        h->prev = n;
    if(l->tail == NULL){
        l->tail = n;
    }
}

Node *pop(List *l){
    if(l->tail != NULL){
        Node *t = l->tail->prev, *ret = l->tail;
        if(t != NULL){
            t->next = NULL;
        }
        if(l->tail == l->head){
            l->head = t;
        }
        l->tail = t;
        return ret;
    }
    return NULL;
}

void print_list(List *l){
    Node *it =l->head;
    while(it != NULL){
        Node *tmp = it->next;
        printf("%p ", it->val);
        it = tmp;
    }
}