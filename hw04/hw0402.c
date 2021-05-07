#include "basic.h"
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

void print_help() {
    printf("./hw0402 -l [obfuscation level] -i [input file] -o [output file]\n"
           "./hw0402 -h\n"
           "./hw0402 --help\n"
           "Display this help manual.\n");
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
        break;
    case 'l': {
        char *edp = NULL;
        level = strtol(optarg, &edp, 10);
        if (*edp) {
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
        break;
    }

    FILE *inFile, *outFile;
    inFile = fopen(inFName, "r");
    if (inFile == NULL) {
        perror("File open failed");
        return 1;
    }
    outFile = fopen(outFName, "rw+");
    if (outFile == NULL) {
        perror("File open failed");
        return 1;
    }

    return 0;
}