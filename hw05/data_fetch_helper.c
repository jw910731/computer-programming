//
// Created by jw910731 on 2021/5/23.
//

#include "data_fetch_helper.h"
#include "basic.h"

#include <cJSON/cJSON.h>

#include <stdlib.h>
#include <string.h>

struct paper *json2paper(struct cJSON *obj){
    obj = cJSON_GetObjectItem(obj, "info");
    if(obj == NULL) return NULL;
    struct paper *ret = calloc(1, sizeof(struct paper));
    struct cJSON *target;

    // title
    target = cJSON_GetObjectItem(obj, "title");
    if(!cJSON_IsString(target)){
        goto fail;
    }
    ret->title = cJSON_GetStringValue(target);

    // source string
    char *src = calloc(1, sizeof(char));
    i32 src_size = 1;
    static const char *src_key[] = {"venue", "volume", "number", "pages"};
    static const int src_extra_space[] = {0, 1, 2, 2};
    for(i32 i = 0 ; i < 4 ; ++i){
        target = cJSON_GetObjectItem(obj, src_key[i]);
        // manage resize
        char *tmp = cJSON_GetStringValue(target);
        if(tmp == NULL) continue;
        src_size += (i32)strlen(tmp) + src_extra_space[i];
        src = realloc(src, src_size);
        switch (i) {
            case 0:
                strncat(src, tmp, src_size);
                break;
            case 1:
                //snprintf(src, src_size, "%s %s", src, tmp);
                strncat(src, " ", src_size);
                strncat(src, tmp, src_size);
                break;
            case 2:
                //snprintf(src, src_size, "%s(%s)", src, tmp);
                strncat(src, "(", src_size);
                strncat(src, tmp, src_size);
                strncat(src, ")", src_size);
                break;
            case 3:
                //snprintf(src, src_size, "%s: %s", src, tmp);
                strncat(src, ": ", src_size);
                strncat(src, tmp, src_size);
                break;
            default:
                break;
        }

    }
    ret->source = src;

    // authors
    target = getRecursiveObject(obj, "authors.author");
    if(!cJSON_IsArray(target)){
        free(src);
        goto fail;
    }
    ret->author_size = (i64) cJSON_GetArraySize(target);
    ret->authors = calloc(ret->author_size, sizeof(char *));
    for(i64 i = 0 ; i < ret->author_size ; ++i){
        struct cJSON *item = cJSON_GetArrayItem(target, (int)i);
        item = cJSON_GetObjectItem(item, "text");
        ret->authors[i] = cJSON_GetStringValue(item);
    }
    // year
    target = cJSON_GetObjectItem(obj, "year");
    if(!cJSON_IsString(target)){
        free(src);
        free(ret->authors);
        goto fail;
    }
    ret->year = cJSON_GetStringValue(target);

    return ret;
    fail:
    free(ret);
    return NULL;
}

// return 1 if result.status.@code is 2xx
int resp_isvalid(struct cJSON *json){
    struct cJSON *obj = getRecursiveObject(json, "result.status.@code");
    if(obj == NULL) return 0;
    char *str = cJSON_GetStringValue(obj);
    if(str == NULL) return 0;
    char *err;
    i32 code = (i32)strtol(str, &err, 10);
    if(*err) return 0;
    if(code >= 200 && code < 300){
        return 1;
    }
    return 0;
}

/*
 * separates keys by '.'
 * return NULL when something went wrong.
 */
struct cJSON *getRecursiveObject(struct cJSON *json, const char * const recusiveKey){
    char *key = calloc(strlen(recusiveKey)+1, sizeof(char));
    strncpy(key, recusiveKey, strlen(recusiveKey));
    char *saveptr;
    for(const char *tok = strtok_r(key, ".", &saveptr) ; tok != NULL ; tok = strtok_r(NULL, ".", &saveptr)){
        if(cJSON_HasObjectItem(json, tok)){
            json = cJSON_GetObjectItem(json, tok);
        }
        else{
            return NULL;
        }
    }
    free(key);
    return json;
}


size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s){
    size_t new_len = s->len + size*nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr+s->len, ptr, size*nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size*nmemb;
}

void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

void free_paper(struct paper *p){
    free(p->authors);
    free(p->source);
    free(p);
}
