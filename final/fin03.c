//
// Created by jw910731 on 2021/6/5.
//

#include "basic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

#include <curl/curl.h>

#include <unistd.h>

static const char endpoint[] = {0x68,0x74,0x74,0x70,0x73,0x3a,0x2f,0x2f,0x61,0x70,0x69,0x2e,0x62,0x69,0x72,0x6b,0x68,0x6f,0x66,0x66,0x2e,0x6d,0x65,0x2f,0x76,0x31,0x2f,0x64,0x69,0x63,0x74,0x2f, 0x00};

struct string {
    char *ptr;
    size_t len;
};

void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s);

void print_help(FILE *f){
    fputs("fin03 :\n"
          "    -w <word >\n"
          "    -h\n", f);
}

int main(int argc, char **argv){
    char *query_str = NULL;
    int c = 0;
    while(c != -1){
        switch ((c = getopt(argc, argv, "w:h"))) {
            case 'w': {
                if(query_str != NULL){
                    fputs("Do not use same argument multiple times.\n", stderr);
                    return 1;
                }
                size_t siz = strlen(optarg);
                query_str = calloc(siz + (sizeof(endpoint)/sizeof(*endpoint)), sizeof(char));
                strcpy(query_str, endpoint);
                strcat(query_str, optarg);
                break;
            }
            case 'h':
                print_help(stdout);
                return 0;
            case -1:
                // end of argparse
                break;
            case '?':
            default:
                print_help(stderr);
                return 1;
        }
    }
    CURL *curl = curl_easy_init();
    if(!curl){
        fputs("Unable to use libcurl.\n", stderr);
        return 1;
    }

    struct string s;
    static char err_buf[CURL_ERROR_SIZE];

    // perform curl connection
    bool retry = true;
    i64 resp_code = 0;
    while(retry){
        init_string(&s);
        curl_easy_setopt(curl, CURLOPT_URL, query_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, err_buf);
        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK){ // check is data fetch success
            fprintf(stderr, "%s: %s\n", "Fail to fetch api data", err_buf);
            goto fail;
        }

        // check if retry
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp_code);

        if(resp_code == 429){ // api fail -> retry later
            fprintf(stderr, "API current busy, waiting 1 minute for cooldown.\n");
            sleep(65); // wait 1 minute to retry
            fprintf(stderr, "Retrying!\n");
            free(s.ptr);
        }
        else{
            retry = false;
        }
    }
    puts(s.ptr);

    free(s.ptr);
    free(query_str);
    curl_easy_cleanup(curl);
    return 0;
fail:
    free(s.ptr);
    free(query_str);
    curl_easy_cleanup(curl);

    return 1;
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