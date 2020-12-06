#pragma once

// aggresively control *val life span
// no need of manual memroy control toward *val
typedef struct _Node{
    void *val;
    struct _Node *prev, *next;
}Node;

typedef struct {
    Node *head, *tail;
}List;

List *create_list();
// free all underlying nodes
void free_list(List **l);
void free_node(Node **n);
void push(Node *n, List *l);
// tail node is not freed
Node *pop(List *l);

void print_list(List *l);
