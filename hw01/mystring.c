#include "mystring.h"

#include <string.h>
#include <stdbool.h>

char *mystrchr(const char *s, int c){
    if(!s) return NULL;
    while(*s != 0){
        if(*s == c) return s;
        ++s;
    }
    return (*s == c)?s:NULL;
}
char *mystrrchr(const char *s, int c){
    if(!s) return NULL;
    size_t l = strlen(s);
    // backward scan
    for(const char *it = s + l; it != s; --it ){
        if(*it == c) return it;
    }
    return NULL;
}
size_t mystrspn(const char *s, const char *accept){
    if(!s) return NULL;
    const char *it;
    for(it = s; *it ; ++it){
        bool match = false;
        for(const char *it2 = accept ; *it2 ; ++it2){
            if(*it == *it2){
                match = true;
                break;
            }
        }
        if(!match)
            break;
    }
    return it - s;
}
size_t mystrcspn(const char *s, const char *reject){
    if(!s) return NULL;
    const char *it;
    for(it = s; *it ; ++it){
        bool match = true;
        for(const char *it2 = reject ; *it2 ; ++it2){
            if(*it == *it2){
                match = false;
                break;
            }
        }
        if(!match)
            break;
    }
    return it - s;
}
char *mystrpbrk(const char *s, const char *accept){
    const char *it;
    for(it = s; *it ; ++it){
        bool match = false;
        for(const char *it2 = accept ; *it2 ; ++it2){
            if(*it == *it2){
                return it;
            }
        }
    }
    return NULL;
}
char *mystrstr(const char *haystack , const char *needle){
    size_t st = 0; char * start = NULL;
    for(char *it = haystack ; *it ; ++it){
        if(!*(needle + st)){ // touch null terminator
            return start;
        }
        if(*it == *(needle + st)){ // more correct match
            if(st == 0){
                start = it;
            }
            ++st;
        }
        else{
            st = 0;
        }
    }
    return NULL;
}
char *mystrtok(char *str, const char *delim){
    static char *state;
    if(str != NULL) state = str;
    char *occ = mystrpbrk(state, delim);
    if(occ == NULL) return NULL;
    *occ = 0;
    state = occ + 1;
    return state;
}