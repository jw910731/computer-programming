//
// Created by jw910731 on 2021/5/8.
//

#include "obfuscator.h"

#include <assert.h>
#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "basic.h"
#include "hash_map.h"
#include "unique_string.h"
#define nil NULL

inline static int isdelim(char c) {
    static const char delim[] = " \n\t{}()";
    for (int i = 0; i < (int)(sizeof(delim) / sizeof(*delim)); ++i) {
        if (delim[i] == c) return 1;
    }
    return 0;
}

struct memfd_man *memfdman_free(struct memfd_man *mem) {
    close(mem->fd);
    munmap(mem->buf, mem->cap);
    free(mem);
    return nil;
}

struct memfd_man *format_obf(struct memfd_man *mem) {
    if (mem == NULL) return NULL;
    assert(mem->cap - mem->len > 0);
    mem->buf[mem->len] = '\0';  // cap must > len + 4096, so it's safe.
    struct memfd_man *ret = calloc(1, sizeof(struct memfd_man));
    int out_fd = ret->fd = memfd_create("format", 0);

    for (char *saveptr = nil, *line = strtok_r(mem->buf, "\n", &saveptr);
         line != nil; line = strtok_r(nil, "\n", &saveptr)) {
        if (line[0] == '\0') continue;
        // strip front
        while (*line) {
            if (isspace(*line))
                line++;
            else
                break;
        }
        int dont_edit = (*line == '#') || (line[0] == '/' && line[1] == '/');
        if (dont_edit) {
            write(out_fd, line, strlen(line));
            write(out_fd, "\n", 1);
            continue;
        }

        for (char *ptr = line; *ptr; ptr++) {
            // 字串字面值
            if (*ptr == '"') {
                do {
                    write(out_fd, ptr, 1);
                    ptr++;
                } while (*ptr && *ptr != '"');
                // write(out_fd, "\"", 1);
            }
            write(out_fd, ptr, 1);
            if (isspace(*ptr)) {
                while (rand() % 5) {
                    write(out_fd, "\n \t" + (rand() % 3), 1);
                }
            }
        }
        write(out_fd, "\n", 1);
    }
    ssize_t len = lseek(out_fd, 0, SEEK_END);
    ret->len = len;
    ret->cap = len + 1;
    if ((ret->buf = mmap(NULL, ret->cap, PROT_READ | PROT_WRITE, MAP_SHARED,
                         ret->fd, 0)) == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }

    memfdman_free(mem);  // mem cannot be use after here
    mem = nil;
    return ret;
}

struct memfd_man *varname_obf(struct memfd_man *mem) {
    if (mem == NULL) return NULL;
    assert(mem->cap - mem->len > 0);
    mem->buf[mem->len] = '\0';  // cap must > len + 4096, so it's safe.
    struct memfd_man *ret = calloc(1, sizeof(struct memfd_man));
    int out_fd = ret->fd = memfd_create("format", 0);
    /******* LOOPS HERE ********/
    HashTable *VARS = table_create();
    do {
        regex_t reg_assign;
        int reg_ret = regcomp(&reg_assign,
                              "(int|float|double|char)(\\s+(((\\s*,\\s*)?([a-"
                              "zA-Z0-9_]+))(\\s*=\\s*[^,;]+)?)+);",
                              REG_EXTENDED);
        assert(!reg_ret);
        regmatch_t pmatch[1024];

        int rc;
        char *ptr = mem->buf;
        while (1) {
            // debug("%s\n", ptr);
            rc = regexec(&reg_assign, ptr, 1024, pmatch, REG_EXTENDED);
            if (rc) {
                break;
            }
            int l_len = pmatch[2].rm_eo - pmatch[2].rm_so;
            char buf[8192];
            memcpy(buf, ptr + pmatch[2].rm_so, l_len);
            buf[l_len] = '\0';
            char *saveptr1, *saveptr2, *saveptr3;
            for (char *u = strtok_r(buf, ",", &saveptr1); u != nil;
                 u = strtok_r(nil, ",", &saveptr1)) {
                char ptn_buf[8192];
                char *ptn = ptn_buf;
                strncpy(ptn, u, 8000);
                ptn[8191] = '\0';
                char new_name[8192] = "";
                unique_str(new_name);
                ptn = strtok_r(ptn, "=", &saveptr2);
                assert(ptn != nil);
                ptn = strtok_r(ptn, " \r\n\t", &saveptr3);
                assert(ptn != nil);
                debug("var name: %s, new_name: %s\n", ptn, new_name);
                table_emplace(VARS, ptn, new_name);
            }
            ptr += pmatch[2].rm_eo;
        }
        regfree(&reg_assign);
    } while (0);
    // func sig
    do {
        char reg_func_replace_patten[] =
            "[a-zA-Z0-9_]+\\s+[a-zA-Z]+\\s*\\(\\s*((,?\\s*(int|double|float|"
            "char)\\s+[a-zA-Z0-9_]\\s*)+)\\s*\\)";
        regex_t reg_assign;
        int reg_ret =
            regcomp(&reg_assign, reg_func_replace_patten, REG_EXTENDED);
        assert(!reg_ret);
        regmatch_t pmatch[1024];

        int rc;
        char *ptr = mem->buf;
        while (1) {
            // debug("%s\n", ptr);
            rc = regexec(&reg_assign, ptr, 1024, pmatch, REG_EXTENDED);
            if (rc) {
                break;
            }
            int l_len = pmatch[2].rm_eo - pmatch[2].rm_so;
            char buf[8192];
            memcpy(buf, ptr + pmatch[2].rm_so, l_len);
            buf[l_len] = '\0';
            char *saveptr1, *saveptr2, *saveptr3;
            for (char *u = strtok_r(buf, ",", &saveptr1); u != nil;
                 u = strtok_r(nil, ",", &saveptr1)) {
                char ptn_buf[8192];
                char *ptn = ptn_buf;
                strncpy(ptn, u, 8192);
                ptn[8191] = '\0';
                char new_name[8192] = "";
                unique_str(new_name);
                ptn = strtok_r(ptn, "=", &saveptr2);
                assert(ptn != nil);
                ptn = strtok_r(ptn, " \r\n\t", &saveptr3);
                assert(ptn != nil);
                ptn = strtok_r(nil, " \r\n\t", &saveptr3);
                assert(ptn != nil);
                debug("var name: %s, new_name: %s\n", ptn, new_name);
                table_emplace(VARS, ptn, new_name);
            }
            ptr += pmatch[2].rm_eo;
        }
        regfree(&reg_assign);
    } while (0);
    do {
        regex_t reg_assign;
        int reg_ret = regcomp(&reg_assign, "([0-9a-zA-Z_]+)", REG_EXTENDED);
        assert(reg_ret == 0);
        char *ptr = mem->buf;
        while (*ptr) {
            regmatch_t pmatch[1024];
            if (*ptr == '"') {
                write(out_fd, ptr++, 1);
                while (*ptr != '"') {
                    write(out_fd, ptr++, 1);
                }
                write(out_fd, ptr++, 1);
                continue;
            }
            int rc = regexec(&reg_assign, ptr, 1024, pmatch, REG_EXTENDED);
            if (rc || pmatch->rm_so != 0) {
                // debug("%s\n", ptr);
                write(out_fd, ptr, 1);
                ptr++;
                continue;
            }
            write(out_fd, ptr, pmatch[0].rm_so);
            char buf[8192];
            memcpy(buf, ptr + pmatch->rm_so, pmatch->rm_eo - pmatch->rm_so);
            buf[pmatch->rm_eo - pmatch->rm_so] = '\0';
            Item *item = table_query(VARS, buf);
            if (item != NULL) {
                write(out_fd, item->data, strlen(item->data));
            } else {
                write(out_fd, buf, strlen(buf));
            }
            ptr += pmatch->rm_eo;
        }
        regfree(&reg_assign);
    } while (0);

    ssize_t len = lseek(out_fd, 0, SEEK_END);
    ret->len = len;
    ret->cap = len + 1;
    if ((ret->buf = mmap(NULL, ret->cap, PROT_READ | PROT_WRITE, MAP_SHARED,
                         ret->fd, 0)) == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }
    table_free(&VARS);
    memfdman_free(mem);  // mem cannot be use after here
    mem = nil;
    return ret;
}

struct memfd_man *funcname_obf(struct memfd_man *mem) {
    if (mem == NULL) return NULL;
    assert(mem->cap - mem->len > 0);
    mem->buf[mem->len] = '\0';  // cap must > len + 4096, so it's safe.
    struct memfd_man *ret = calloc(1, sizeof(struct memfd_man));
    int out_fd = ret->fd = memfd_create("format", 0);
    /******* LOOPS HERE ********/
    HashTable *FUNCS = table_create();
    do {
        char reg_func_replace_patten[] =
            "[a-zA-Z0-9_]+\\s+([a-zA-Z0-9_]+)\\s*\\(\\s*((,?\\s*(int|double|"
            "float|"
            "char)\\s+[a-zA-Z0-9_]+\\s*)*)\\s*\\)";
        regex_t reg_assign;
        int reg_ret =
            regcomp(&reg_assign, reg_func_replace_patten, REG_EXTENDED);
        assert(!reg_ret);
        regmatch_t pmatch[1024];

        int rc;
        char *ptr = mem->buf;
        while (1) {
            // debug("%s\n", ptr);
            rc = regexec(&reg_assign, ptr, 1024, pmatch, REG_EXTENDED);
            if (rc) {
                break;
            }
            int l_len = pmatch[1].rm_eo - pmatch[1].rm_so;
            char buf[8192], new_name[8192];
            memcpy(buf, ptr + pmatch[1].rm_so, l_len);
            buf[l_len] = '\0';
            char *saveptr1, *saveptr2, *saveptr3;
            if (strcmp(buf, "main") != 0) {
                table_emplace(FUNCS, buf, new_name);
                debug("func name: %s, new_name: %s\n", buf, new_name);
            }
            ptr += pmatch[1].rm_eo;
        }
        regfree(&reg_assign);
    } while (0);

    do {
        regex_t reg_assign;
        int reg_ret =
            regcomp(&reg_assign, "([0-9a-zA-Z_]+)\\s*\\(", REG_EXTENDED);
        assert(reg_ret == 0);
        char *ptr = mem->buf;
        while (*ptr) {
            regmatch_t pmatch[1024];
            if (*ptr == '"') {
                write(out_fd, ptr++, 1);
                while (*ptr != '"') {
                    write(out_fd, ptr++, 1);
                }
                write(out_fd, ptr++, 1);
                continue;
            }
            int rc = regexec(&reg_assign, ptr, 1024, pmatch, REG_EXTENDED);
            if (rc || pmatch->rm_so != 0) {
                write(out_fd, ptr, 1);
                ptr++;
                continue;
            }
            write(out_fd, ptr, pmatch[0].rm_so);
            char buf[8192];
            memcpy(buf, ptr + pmatch->rm_so, pmatch->rm_eo - pmatch->rm_so);
            buf[pmatch->rm_eo - pmatch->rm_so - 1] = '\0';
            Item *item = table_query(FUNCS, buf);
            if (item != NULL) {
                write(out_fd, item->data, strlen(item->data));
                write(out_fd, "(", 1);
            } else {
                write(out_fd, buf, strlen(buf));
                write(out_fd, "(", 1);
            }
            ptr += pmatch->rm_eo;
        }
        regfree(&reg_assign);
    } while (0);

    ssize_t len = lseek(out_fd, 0, SEEK_END);
    ret->len = len;
    ret->cap = len + 1;
    if ((ret->buf = mmap(NULL, ret->cap, PROT_READ | PROT_WRITE, MAP_SHARED,
                         ret->fd, 0)) == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }
    table_free(&FUNCS);
    memfdman_free(mem);  // mem cannot be use after here
    mem = nil;
    return ret;
}

struct memfd_man *intliteral_obf(struct memfd_man *mem) {
    if (mem == NULL) return NULL;
    assert(mem->cap - mem->len > 0);
    mem->buf[mem->len] = '\0';  // cap must > len + 4096, so it's safe.
    struct memfd_man *ret = calloc(1, sizeof(struct memfd_man));
    int out_fd = ret->fd = memfd_create("format", 0);
    /******* LOOPS HERE ********/
    HashTable *FUNCS = table_create();
    do {
        regex_t reg_assign;
        int reg_ret = regcomp(&reg_assign, "([0-9]+)", REG_EXTENDED);
        assert(reg_ret == 0);
        char *ptr = mem->buf;
        while (*ptr) {
            regmatch_t pmatch[1024];
            if (*ptr == '"') {
                write(out_fd, ptr++, 1);
                while (*ptr != '"') {
                    write(out_fd, ptr++, 1);
                }
                write(out_fd, ptr++, 1);
                continue;
            }
            int rc = regexec(&reg_assign, ptr, 1024, pmatch, REG_EXTENDED);
            if (rc || pmatch->rm_so != 0) {
                write(out_fd, ptr, 1);
                ptr++;
                continue;
            }
            write(out_fd, ptr, pmatch[0].rm_so);
            char buf[8192];
            memcpy(buf, ptr + pmatch->rm_so, pmatch->rm_eo - pmatch->rm_so);
            buf[pmatch->rm_eo - pmatch->rm_so] = '\0';
            u64 u;
            sscanf(buf, "%lu", &u);
            srand(time(0));
            u64 n = (rand() % 7 + 1);
            u64 p = u / n;
            u64 r = u - p * n;
            char buf_n[8192];
            debug("(%lu+%lu*%lu) = %lu\n", r, p, n, u);
            sprintf(buf_n, "(%lu+%lu*%lu)", r, p, n);
            assert(u == r + p * n);
            write(out_fd, buf_n, strlen(buf_n));
            ptr += pmatch->rm_eo;
        }
        regfree(&reg_assign);
    } while (0);

    ssize_t len = lseek(out_fd, 0, SEEK_END);
    ret->len = len;
    ret->cap = len + 1;
    if ((ret->buf = mmap(NULL, ret->cap, PROT_READ | PROT_WRITE, MAP_SHARED,
                         ret->fd, 0)) == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }
    table_free(&FUNCS);
    memfdman_free(mem);  // mem cannot be use after here
    mem = nil;
    return ret;
}

#undef nil
