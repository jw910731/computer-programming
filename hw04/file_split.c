//
// Created by jw910731 on 2021/5/18.
//

#include "file_split.h"
#include "basic.h"

#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZ 1024
static char buf[BUF_SIZ];

void split(char *split_file, i64 size){
    FILE *split_f = fopen(split_file, "r");
    if (split_f == NULL){
        perror("Failed to open file");
        exit(1);
    }
    // loop of each piece
    int cnt = 1;
    while(!feof(split_f)){
        i64 piece_siz = size;
        // format piece name
        static char fname_buf[2048];
        snprintf(fname_buf, 2048,"%s.%d", split_file, cnt);
        FILE *piece = fopen(fname_buf, "w+");
        if(piece == NULL){
            perror("File creation error");
            exit(1);
        }
        // write in current position
        u64 position = ftell(split_f);
        fwrite(&position, 1, sizeof(u64), piece);
        // buffer IO
        while(piece_siz > 0){
            if(feof(split_f)) break;
            i64 opsiz = (i64)fread(buf, 1, BUF_SIZ, split_f);
            piece_siz -= (i64)fwrite(buf, 1, opsiz, piece);
        }
        ++cnt;
        fclose(piece);
    }
    fclose(split_f);
}

void merge(char *output_file, char **source_files, i32 size){
    FILE *output = fopen(output_file, "w+");
    if (output == NULL){
        perror("Failed to open file");
        exit(1);
    }
    for(i32 i = 0 ; i < size ; ++i){
        FILE *f = fopen(source_files[i], "r");
        if(f == NULL){
            perror("Unable to open file");
            exit(1);
        }
        u64 pos;
        fread(&pos, 1, sizeof(u64), f);
        fseek(output, (long)pos, SEEK_SET);
        // buffer IO
        while(!feof(f)){
            i64 opsiz = fread(buf, 1, BUF_SIZ, f);
            fwrite(buf, 1, opsiz, output);
        }
        fclose(f);
    }
    fclose(output);
}
