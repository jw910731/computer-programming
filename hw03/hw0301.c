#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

#include <linenoise.h>

#include "basic.h"
#include "color.h"
#include "file_selector.h"

struct timespec timediff(struct timespec a, struct timespec b);

void stupid_newline_elliminator(char *s);

int main() {
    char *fileInput;
    linenoiseSetCompletionCallback(file_completion);
    fileInput = linenoise("Open a LRC file: ");
    // open file
    FILE *file = fopen(fileInput, "r");

    // file open error handler
    if (file == NULL) {
        perror("File open error");
        return 1;
    }

    static char buf[1024];
    struct timespec prev = {0, 0};
    char color[][8] = {CLR_LRED, CLR_LGRN, CLR_LBL, CLR_LMGT,
                       CLR_LCYN, CLR_GRY,  CLR_RED, CLR_YLW};
    char clr_name[8][1024];
    i32 name_idx = -1, stk = 0;
    while (!feof(file)) {
        size_t siz = fgets_n(buf, 1024, file);
        stupid_newline_elliminator(buf);
        // this line is People name
        char *colon = strchr(buf, ':');
        if (buf[0] != '[' && colon != NULL) {
            // name process
            *colon = 0; // eliminate ':'
            // search for same name
            bool flag = false;
            for (i32 i = 0; i < stk; ++i) {
                if (strcmp(buf, clr_name[i]) == 0) {
                    name_idx = i;
                    flag = true;
                    break;
                }
            }
            // not found => push stack
            if (!flag) {
                strncpy(clr_name[stk], buf, 1024);
                name_idx = stk++;
            }
        } else if (buf[0] == '[') {
            if (isdigit(buf[1])) {
                i64 m, s, ms;
                char *end = NULL;
                m = strtol(buf + 1, &end, 10);
                s = strtol(end + 1, &end, 10);
                ms = strtol(end + 1, &end, 10);
                struct timespec t = {m * 60 + s, ms * 1000000}, diff;
                diff = timediff(prev, t);
                nanosleep(&diff, &diff);
                fputs(color[name_idx], stdout);
                printf("%s: %s\n", clr_name[name_idx], end + 1);
                fputs(CLR_RST, stdout);
                prev = t;
            }
            // else condition is ignored
        } else { // unknown time lyric is instant displayed
            fputs(color[name_idx], stdout);
            printf("%s: %s\n", clr_name[name_idx], buf);
            fputs(CLR_RST, stdout);
        }
    }

    // free resources
    fclose(file);
    free(fileInput);
}

void stupid_newline_elliminator(char *ss) {
    for (char *s = ss; *s; ++s) {
        if (*s == '\r') {
            *s = 0;
        }
    }
}

struct timespec timediff(struct timespec t1, struct timespec t2) {
    struct timespec diff;
    if (t2.tv_nsec - t1.tv_nsec < 0) {
        diff.tv_sec = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return diff;
}
