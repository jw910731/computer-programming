#include "basic.h"
#include "obfuscator.h"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/file.h>

void print_help() {
    printf("./hw0402 -l [obfuscation level] -i [input file] -o [output file]\n"
           "obfuscation level is in [1, 4]\n"
           "\n"
           "./hw0402 -h\n"
           "./hw0402 --help\n"
           "\tDisplay this help manual.\n");
}

const struct option lopts[] = {
        /*name, arg, flag, val*/
        {"help", 0, NULL, 'h'},
};

int main(int argc, char **argv) {
    static char inFName[1024], outFName[1024];
    /*
     * opt is tmp var for arg parse
     * level is for obfuscation level
     */
    int opt, level;
    switch ((opt = getopt_long(argc, argv, "lioh", lopts, NULL))) {
        case 'h':
            print_help();
            return 0;
        case 'l': {
            char *edp = NULL;
            level = (int)strtol(optarg, &edp, 10);
            if (*edp || level < 1 || level > 4) {
                fputs("Invalid argument usage. Use \"--help\" for help.", stderr);
                return 1;
            }
            break;
        }
        case 'i':
            strncpy(inFName, optarg, 1024);
            break;
        case 'o':
            strncpy(outFName, optarg, 1024);
            break;
        case -1:
            /* End of arg parse*/
            break;
        case '?':
        default:
            fputs("Unknown argument!\n", stderr);
            return 1;
    }

    // Open file stream for later use
    int inFd, outFd;
    inFd = open(inFName, O_RDONLY);
    if (inFd == -1) {
        perror("File open failed");
        return 1;
    }
    // lock input file with exclusive advisory lock
    if (flock(inFd, LOCK_EX | LOCK_NB) == -1) {
        if (errno == EWOULDBLOCK) {
            fputs("Lock acquire failed. Please try later or unlock the file "
                  "lock.\n",
                  stderr);
        }
        else{
            perror("Lock error");
        }
        close(inFd);
        return 1;
    }

    outFd = open(outFName, O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    if (outFd == -1) {
        perror("File open failed");
        close(inFd);
        return 1;
    }
    FILE *outFile;
    outFile = fdopen(outFd, "rw+");
    if (outFile == NULL) {
        perror("File open failed");
        return 1;
    }

    // get file info
    struct stat inFstat;
    if(fstat(inFd, &inFstat) == -1) {
        perror("fstat");
        return 1;
    }
    const i64 cap = PAGE_ROUND(inFstat.st_size);

    // prepare memory mapping
    char *inFileBuf =
            mmap(NULL, cap, PROT_READ | PROT_WRITE, MAP_PRIVATE, inFd, 0);
    if(inFileBuf == MAP_FAILED){
        perror("mmap");
        return 1;
    }

    //prepare memfd_man init state
    struct memfd_man *memfdMan = calloc(1, sizeof(struct memfd_man));
    memfdMan->fd = inFd;
    memfdMan->buf = inFileBuf;
    memfdMan->cap = cap;
    memfdMan->len = inFstat.st_size;

    // process
    memfdMan = format_obf(memfdMan);
    if(level > 1){
        memfdMan = varname_obf(memfdMan);
    }
    if(level > 2){
        memfdMan = funcname_obf(memfdMan);
    }
    if(level > 3){
        memfdMan = intliteral_obf(memfdMan);
    }

    // clean resource
    memfdman_free(memfdMan);
    close(outFd);
    return 0;
}

/*
 * Original File -> CoW mmap segment -> process
 * -> result store in memfd -> expand mmap segment if needed
 * -> copy result back to mmap segment -> back to process
 * -> write data to output
 */
