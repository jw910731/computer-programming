#include <linenoise.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "basic.h"
#include "file_selector.h"

inline static bool entry_check(struct dirent *f, const char *s) {
    return f->d_type != DT_SOCK && strPreCmp(f->d_name, s) &&
           strcmp(f->d_name, ".") != 0 && strcmp(f->d_name, "..") != 0;
}

void file_completion(const char *buf, linenoiseCompletions *lc) {
    const char *pos = strrchr(buf, '/');
    // stop starting slash as dir split
    DIR *dir;
    // default offset to start from the beginning
    i64 len = 0;
    char *dir_path = "./";
    if (pos != NULL) {
        len = pos - buf + 1; // len = end_pos + 1
        // extra space for null terminator
        dir_path = calloc(len + 1, sizeof(char));
        const char *it = buf;
        for (char *dest = dir_path; it - buf <= len; ++it, ++dest)
            if (it - buf < len)
                *dest = *it;
            else
                *dest = 0; // add null terminator
    }
    dir = opendir(dir_path);
    // Dir open failed with an invalid dir path
    if (dir == NULL)
        return;
    for (struct dirent *f = readdir(dir); f != NULL; f = readdir(dir)) {
        if (entry_check(f, buf + len)) {
            if (len > 0) {
                char *suggest =
                    calloc(len + f->d_namlen + (f->d_type == DT_DIR) + 1, 1);
                strncpy(suggest, buf, len);
                strncat(suggest, f->d_name, f->d_namlen);
                if (f->d_type == DT_DIR)
                    strncat(suggest, "/", 1);
                linenoiseAddCompletion(lc, suggest);
                free(suggest);
            } else {
                linenoiseAddCompletion(lc, f->d_name);
            }
        }
    }

    // free resource
    closedir(dir);
    if (len > 0) {
        // free allocated space
        free(dir_path);
    }
}

bool strPreCmp(const char *str, const char *target) {
    for (const char *s = str, *t = target; *t; ++s, ++t)
        if (*s != *t)
            return false;
    return true;
}

bool strPostCmp(const char *str, const char *target) {
    i64 str_siz = strlen(str), tar_siz = strlen(target);
    for (const char *s = str + (str_siz - tar_siz), *t = target; *t; ++s, ++t)
        if (*s != *t)
            return false;
    return true;
}