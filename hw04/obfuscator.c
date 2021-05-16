//
// Created by jw910731 on 2021/5/8.
//

#include "obfuscator.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

inline static int isdelim(char c){
    const static char delim[] = " \n\t{}()";
    for(int i = 0 ; i < (int)(sizeof(delim)/sizeof(*delim)) ; ++i){
        if(delim[i] == c) return 1;
    }
    return 0;
}

struct memfd_man *memfdman_free(struct memfd_man * mem){
    close(mem->fd);
    munmap(mem->buf, mem->cap);
    free(mem);
}

struct memfd_man *format_obf(struct memfd_man *mem) {
    if(mem == NULL) return NULL;
    struct memfd_man *ret = calloc(1, sizeof(struct memfd_man));
    ret->fd = memfd_create("format", 0);
    char *rit, *split;
    rit = split = mem->buf;
    while ((rit - mem->buf) < mem->len) {
        // tokenizer
        // TODO: refactor messy condition
        if(isdelim(*rit)){
            if(!isdelim(*split)){

            }
            else{
                // TODO: handle token
            }
            // reset split tag
            split = rit + 1;
        }
        /*Else => continue to accumulate rit only*/
        ++rit;
    }
    struct stat stat;
    fstat(ret->fd, &stat);
    memfdman_free(mem); // mem cannot be use after here
    ret->len = stat.st_size;
    ret->cap = PAGE_ROUND(ret->len);
    if((ret->buf = mmap(NULL, ret->cap, PROT_READ | PROT_WRITE, MAP_SHARED, ret->fd, 0)) == MAP_FAILED){
        perror("mmap()");
        exit(1);
    }
    return ret;
}

struct memfd_man * varname_obf(struct memfd_man *mem) {
    if(mem == NULL) return NULL;
    return mem;
}

struct memfd_man * funcname_obf(struct memfd_man *mem) {
    if(mem == NULL) return NULL;
    return mem;
}

struct memfd_man * intliteral_obf(struct memfd_man *mem) {
    if(mem == NULL) return NULL;
    return mem;
}