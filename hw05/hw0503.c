//
// Created by jw910731 on 2021/5/22.
//

#include "basic.h"
#include "data_fetch_helper.h"

#include <curl/curl.h>
#include <cJSON/cJSON.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

const char *pat = "https://dblp.org/search/publ/api?h=10&format=json&q=%s";

void print_help();

int main(int argc, char **argv){
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if(!curl){
        return 1;
    }

    char *buf = NULL;

    bool flag = true;
    while(flag){
        int c;
        switch ((c = getopt(argc, argv, "hq:"))) {
            case 'q': {
                const size_t pat_len = strlen(pat);
                const size_t siz = strlen(optarg) + pat_len - 1;
                if(siz - 1 > 8000000){
                    fputs("Query string too long.\n", stderr);
                    return 1;
                }
                buf = calloc(siz, sizeof(char));
                snprintf(buf, siz, pat, optarg);
                break;
            }
            case 'h':
                print_help(stdout);
                return 0;

            case -1:
                flag = false;
                break;

            case '?':
            default:
                print_help(stderr);
                return 1;
        }
    }

    if(buf == NULL){
        fputs("-q must be used.\n", stderr);
        return 1;
    }

    // setup curl connection
    struct string s;
    static char err_buf[CURL_ERROR_SIZE];
    init_string(&s);
    curl_easy_setopt(curl, CURLOPT_URL, buf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err_buf);

    // perform curl connection
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK){ // check is data fetch success
        fprintf("%s: %s\n", "Fail to fetch api data", err_buf);
        goto fail;
    }

    // parse response json
    cJSON_Minify(s.ptr);
    cJSON *json = cJSON_Parse(s.ptr);
    if (json == NULL) {
        fputs("API returned malformed json data.\n", stderr);
        goto fail;
    }

    // check is response valid
    if(!resp_isvalid(json)){
        fputs("API returned with error.\n", stderr);
        goto fail_json;
    }

    // fetch search answer array object
    struct cJSON *hits = getRecursiveObject(json, "result.hits.hit");
    if(!cJSON_IsArray(hits)){
        fputs("API returned malformed structure json.\n", stderr);
        goto fail_json;
    }

    i32 arr_size = cJSON_GetArraySize(hits);
    for(i32 i = 0 ; i < arr_size ; ++i){
        struct cJSON *item = cJSON_GetArrayItem(hits, i);
        struct paper *p = json2paper(item);
        if(p == NULL){
            fputs("API returned malformed structure json.\n", stderr);
            goto fail_json;
        }

        printf("Paper %02d:\n"
               "    Title: %s"
               "    Author: ", i+1, p->title);
        for(i64 j = 0 ; j < p->author_size ; ++j){
            printf("%s%c ", p->authors[j], ",."[j == p->author_size - 1]);
        }
        printf("\n    Source: %s\n"
               "    Year: %s\n", p->source, p->year);
        free_paper(p);
    }

    // free resource
    cJSON_Delete(json);
    free(s.ptr);
    free(buf);
    curl_easy_cleanup(curl);
    return 0;
fail_json:
    cJSON_Delete(json);
fail:
    // free resource
    free(buf);
    free(s.ptr);
    curl_easy_cleanup(curl);
    return 1;
}

void print_help(FILE *f){
    fputs("./hw0503 [options]\n"
          "    -q <query string>"
          "        search the query string in dblp.\n"
          "    -h\n"
          "        print this help message.\n", f);
}