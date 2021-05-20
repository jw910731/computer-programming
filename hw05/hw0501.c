#include "basic.h"
#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

enum program_mode{
    ENCODE,
    DECODE,
    UNDEFINED
};

struct option lopts[]= {
        {"help", 0, NULL, 'h'},
        {"enc", 1, NULL, 'e'},
        {"dec", 1, NULL, 'd'},
        {"output", 1, NULL, 'o'},
};

void print_help(FILE *f){
    fputs("./hw0501 [options]\n"
           "Options:\n"
           "    -e, --enc Encode a file to a text file.\n"
           "    -d, --dec Decode a text file to a file.\n"
           "    -o, --output Output file name.\n"
           "        If this option is not specified, stdout is used instead.", f);
}

int main(int argc, char **argv) {
    int c;
    enum program_mode mode = UNDEFINED;
    char *output_file = NULL, *input_file = NULL;
    while(1){
        switch ((c = getopt_long(argc, argv, "he:d:o:", lopts, NULL))) {
            case 'h':
                print_help(stdout);
                return 0;
            case 'e':
                if (input_file != NULL){
                    free(input_file);
                    fputs("Encode and decode cannot be used together.\n", stderr);
                    return 1;
                }
                mode = ENCODE;
                input_file = strndup(optarg, 2048);
                break;
            case 'd':
                if (input_file != NULL){
                    free(input_file);
                    fputs("Encode and decode cannot be used together.\n", stderr);
                    return 1;
                }
                mode = DECODE;
                input_file = strndup(optarg, 2048);
                break;
            case 'o':
                output_file = strndup(optarg, 2048);
                break;
            case -1: // end of argparse
                goto end_argparse;
            case '?':
            default:
                fputs("Error parsing argument.\n",stderr);
                print_help(stderr);
                return 1;
        }
    }
    end_argparse:
    if(mode == UNDEFINED){
        fputs("No mode argument is given.\n", stderr);
        print_help(stderr);
        return 1;
    }
    int outputfd;
    if(output_file == NULL) {
        // set to stdout
        outputfd = STDOUT_FILENO;
    }
    else{
        outputfd = open(output_file, O_RDWR | O_CREAT | O_TRUNC);
        if(outputfd == -1){
            perror("Unable to open output file");
            return 1;
        }
        // set file size to 0
        ftruncate(outputfd, 0);
    }

    int inputfd = open(input_file, O_RDONLY);
    if(inputfd == -1){
        perror("Unable to open input file");
        return 1;
    }

    if(mode == DECODE){
        if(base64_decode(inputfd, outputfd) != 0){
            fputs("Decode error!\n", stderr);
            goto error;
        }
    }
    if(mode == ENCODE){
        if(base64_encode(inputfd, outputfd) != 0){
            fputs("Encode error!\n", stderr);
            goto error;
        }
    }

    // free filename string
    free(output_file);
    free(input_file);
    // close file
    close(outputfd);
    close(inputfd);
    return 0;

    // error handling in BSD way (X
error:
    // free filename string
    free(output_file);
    free(input_file);
    // close file
    close(outputfd);
    close(inputfd);
    return 1;
}