#include "basic.h"
#include <stdio.h>
#include <stdlib.h>
#include "undo.h"
int main(){
    UndoQueue *q = create_undo_queue(10);
    bool flag = true;
    while(flag){
        i32 n;
        printf("input: ");
        scanf("%d", &n);
        if(n > 0){
            Node *tmp_n = calloc(1, sizeof(Node));
            tmp_n->val = calloc(1, sizeof(i32));
            *(i32 *)tmp_n->val = n;
            u_push(tmp_n, q);
        }
        else{
            switch (n){
            case 0:
                flag = false;
                break;
            case -1:
                u_undo(q);
                break;
            case -2:
                u_redo(q);
                break;
            default:
                fprintf(stderr, "Error Command!\n");
                break;
            }
        }
    }
    u_close(q);
    printf("Result: ");
    Node *it = q->l->tail;
    while(it != NULL){
        Node *tmp = it->prev;
        printf("%d ", *(i32 *)(it->val));
        it = tmp;
    }
    putchar('\n');
    free_undo_queue(&q);
    return 0;
}