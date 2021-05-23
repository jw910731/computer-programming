//
// Created by jw910731 on 2021/5/23.
//

#ifndef HW05_DATA_FETCH_HELPER_H
#define HW05_DATA_FETCH_HELPER_H

#include "basic.h"

#include <stdlib.h>

#include <cJSON/cJSON.h>

struct string {
    char *ptr;
    size_t len;
};

struct paper{
    char *title;
    i64 author_size;
    char **authors;
    char *source;
    char *year;
};

struct paper *json2paper(struct cJSON *obj);
void free_paper(struct paper *p);

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

int resp_isvalid(struct cJSON *json);
struct cJSON *getRecursiveObject(struct cJSON *json, const char *recusiveKey);
#endif //HW05_DATA_FETCH_HELPER_H
