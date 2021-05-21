//
// Created by jw910731 on 2021/5/16.
//
#include "basic.h"
#include "file_split.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

void print_help() {
    puts("./hw0401 -h --help\n"
         "Print this help message.\n"
        "Split:\n"
         "./hw0401 -s [file] --size [Small File Size]\n"
         "The default small file size is 1000 bytes.\n"
         "Recover:\n"
         "./hw0401 -r [output file] [small files]\n"
         "The input small files may not be in order.");
}

const struct option lopts[] = {
        /*name, arg, flag, val*/
        {"help", 0, NULL, 'h'},
        {"size", 1, NULL, 0x100},
};

enum program_mode{
    SPLIT,
    MERGE,
    UNDEFINED
};

int main(int argc, char **argv){
    int c;
    bool parse_arg = true;
    char *file_name = NULL;
    enum program_mode mode = UNDEFINED;
    i64 split_size = -1;
    while(parse_arg){
        switch((c = getopt_long(argc, argv, "s:r:h", lopts, NULL))){
            case 's':
                if(mode != UNDEFINED){ // argument conflict
                    fputs("-s and -r cannot use together.\n", stderr);
                    return 1;
                }
                file_name = strndup(optarg, 2048);
                mode = SPLIT;
                break;
            case 'r':
                if(mode != UNDEFINED){ // argument conflict
                    fputs("-s and -r cannot use together.\n", stderr);
                    return 1;
                }
                file_name = strndup(optarg, 2048);
                mode = MERGE;
                break;
            case 'h':
                print_help();
                return 0;
            // --size
            case 0x100: {
                char *end;
                split_size = (i64)strtoll(optarg, &end, 10);
                if(*end || split_size < 0){ // numeric conversion error or negative size
                    fputs("Invalid argument!", stderr);
                    goto argparse_error;
                }
                break;
            }
            // end of parse arg
            case -1:
                parse_arg = false;
                break;
            // invalid case
            case '?':
            default:
                fputs("Argument parse error.\n", stderr);
            argparse_error:
                print_help();
                return 1;
        }
    }
    // prevent --size used with -r
    if(split_size != -1 && mode == MERGE){
        fputs("--size cannot used with -r.\n",stderr);
        return 1;
    }

    switch (mode) {
        case SPLIT:
            // if --size is not used at all
            if(split_size < 0){
                split_size = 1000;
            }
            split(file_name, split_size);
            break;
        case MERGE:
            merge(file_name, argv+optind, argc - optind);
            break;

        default:
        case UNDEFINED:
            fputs("No mode argument is specified.\n", stderr);
            print_help();
            return 1;
    }

    // release resource
    free(file_name);
    return 0;
}