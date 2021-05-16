//
// Created by jw910731 on 2021/5/8.
//
#pragma once

#include <stdint.h>

#define PAGE_ROUND(x) ((((x)+4095)/4096)*4096);

struct memfd_man{
    // associated mmap segment
    char *buf;
    // associated file descriptor
    int fd;
    // cap: mmap segment size
    // len: content size
    int64_t cap, len;
};
struct memfd_man *memfdman_free(struct memfd_man *);

struct memfd_man * format_obf(struct memfd_man *mem);
struct memfd_man * varname_obf(struct memfd_man *mem);
struct memfd_man * funcname_obf(struct memfd_man *mem);
struct memfd_man * intliteral_obf(struct memfd_man *mem);