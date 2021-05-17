//
// Created by jw910731 on 2021/5/18.
//

#ifndef HW04_FILE_SPLIT_H
#define HW04_FILE_SPLIT_H

#include "basic.h"

void split(char *split_file, i64 size);

void merge(char *output_file, char **source_files, i32 size);

#endif //HW04_FILE_SPLIT_H
