#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include <linenoise.h>

#include "basic.h"
#include "file_selector.h"

int main() {
    char *fileInput;
    linenoiseSetCompletionCallback(file_completion);
    fileInput = linenoise("Open a LRC file: ");
    puts(fileInput);

    // free resource
    free(fileInput);
}
