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

    // not selecting pwd
    if (pos != NULL) {
        len = pos - buf + 1; // len = end_pos + 1
        // extra space for null terminator
        dir_path = calloc(len + 1, sizeof(char));
        strncpy(dir_path, buf, len);
        // add trailing null terminator
        dir_path[len] = 0;
    }
    // open dir with dir path
    dir = opendir(dir_path);
    // Dir open failed with an invalid dir path
    if (dir == NULL)
        return;
    // traverse dir entries
    for (struct dirent *f = readdir(dir); f != NULL; f = readdir(dir)) {
        if (entry_check(f, buf + len)) {
            if (len > 0) { // select non pwd
                size_t name_siz = strlen(f->d_name);
                char *suggest =
                    calloc(len + name_siz + (f->d_type == DT_DIR) + 1, 1);

                // concat parent dir name
                strncpy(suggest, buf, len);
                // concat entry name
                strncat(suggest, f->d_name, name_siz);
                // add trailing slash after dir name
                if (f->d_type == DT_DIR)
                    strncat(suggest, "/", 1);
                linenoiseAddCompletion(lc, suggest);
                free(suggest);
            } else { // selecting pwd
                linenoiseAddCompletion(lc, f->d_name);
            }
        }
    }

    // free resource
    closedir(dir);
    if (len > 0) { // free if space is overwritten with dynamic allocated space
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