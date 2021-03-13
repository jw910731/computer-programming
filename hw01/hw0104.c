#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

#include "basic.h"
#include "hash_map.h"

// use to construct stack like key
static char stack_buf[4096], tmp[4096];
static i32 bp = 0, sp = 0, stack[2048], tp = 0;

typedef enum{
    Key,
    Separator,
    Value
}State;

// returning parser end position
const char *object_parser(HashTable *t, const char *s);
const char *array_parser(HashTable *t, const char *s);

inline static const char *str_fetcher(const char *s){
    const char *top = ++s;
    // push stack
    while(*top != '"'){
        ++top;
    }
    return top;
}

int main(){
    // buffer for json and key query
    char buf[2050]; //extra buffer for newline character and null terminator
    HashTable *t = table_create();
    fgets_n(buf, 2050, stdin);
    object_parser(t, buf);
    // Choice Menu
    while(1){
        int cmd = -1, ck = -1;
        do{
            if(ck != -1){
                puts("Error: Invalid Input!");
            }
            printf("Choice (0:Exit ,1:Get) : ");
            ck = scanf("%d", &cmd);
            char c;
            while((c = getchar() != '\n'));
        }while(ck != 1 || (cmd != 1 && cmd != 0));
        if(cmd == 0){
            puts("Bye!");
            break;
        } else {
            printf("Key: ");
            fgets_n(buf, 2050,  stdin);
            Item *itm = table_query(t, buf);
            if(itm != NULL)
                printf("Value: %s\n", itm->data);
            else
                puts("Key Not Found");
        }
    }
    table_free(&t);
    return 0;
}

const char *object_parser(HashTable *t, const char *s){
    State state = Key;
    // callee stack setup
    if(tp){ // if not root object
        stack[tp++] = bp;
        bp = sp;
        sp += 1; // for dot
        memcpy(stack_buf+bp, ".", sizeof(".")); // null terminator is added
    }
    while(*s != '}'){
        switch (state) {
            case Key:
                if(*s == '"'){
                    const char *top = str_fetcher(s);
                    ++s;
                    stack[tp++] = bp;
                    bp = sp;
                    sp += (int)(top - s); // add size
                    memcpy(stack_buf + bp, s, top - s /* size */);
                    stack_buf[sp] = 0; // null terminator
                    s = top; // will be advance one more step by global increment
                    state = Separator;
                }
                break;
            case Separator:
                if(*s == ':'){
                    state = Value;
                }
                break;
            case Value:
                // deal with excess spaces
                if(isspace(*s)) break;
                if(*s == ',') {
                    state = Key;
                    break;
                }
                if(*s == '"'){ // string object
                    const char *top = str_fetcher(s);
                    ++s;
                    memcpy(tmp, s, top - s);
                    tmp[top-s] = 0; // null terminator
                    table_emplace(t, stack_buf, tmp);
                    s = top; // leave space for global incremental
                }
                else if (*s == '{') { // Object
                    s = object_parser(t, s);
                }
                else if (*s == '[') { // Array
                    s = array_parser(t, s+1);
                }
                else{ // null, number or boolean (end with trailing space)
                    const char *top = s;
                    // incremental scan
                    while(!isspace(*top) && *top != ',' && *top != '}'){
                        ++top;
                    }
                    memcpy(tmp, s, top - s);
                    tmp[top-s] = 0; // null terminator
                    table_emplace(t, stack_buf, tmp);
                    s = top - 1; // leave space for global incremental
                }
                // stack pop
                sp = bp;
                bp = stack[--tp];
                stack_buf[sp] = 0; // add trailing null terminator
                break;
        }
        ++s;
    }
    // callee clear stack
    if(tp){ // if not root object
        sp = bp;
        bp = stack[--tp];
        stack_buf[sp] = 0; // add trailing null terminator
    }
    return s;
}
const char *array_parser(HashTable *t, const char *s){
    int32_t idx = 0;
    while(*s != ']'){
        if(!isspace(*s)){ // is value
            // push index to stack
            stack[tp++] = bp;
            bp = sp;
            sp += snprintf(stack_buf+bp, 4096-bp,"[%d]", idx++);
            if(*s == '"'){ // string object
                const char *top = str_fetcher(s);
                ++s;
                memcpy(tmp, s, top - s);
                tmp[top-s] = 0; // null terminator
                table_emplace(t, stack_buf, tmp);
                s = top;
            }
            else if (*s == '{') { // Object
                s = object_parser(t, s);
            }
            else if (*s == '[') { // Array
                s = array_parser(t, s+1);
            }
            else{ // null, number or boolean (end with trailing space)
                const char *top = s;
                // incremental scan
                while(!isspace(*top) && *top != ',' && *top != ']'){
                    ++top;
                }
                memcpy(tmp, s, top - s);
                tmp[top-s] = 0; // null terminator
                table_emplace(t, stack_buf, tmp);
                s = top;
            }
            // pop stack
            sp = bp;
            bp = stack[--tp];
            stack_buf[sp] = 0; // add trailing null terminator
            // remove ','
            while(*s != ',' && *s != ']') ++s;
            if(*s == ']') break;
        }
        ++s;
    }
    return s;
}