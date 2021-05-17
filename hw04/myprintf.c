//
// Created by jw910731 on 2021/5/18.
//

#include "myprintf.h"
#include "basic.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

/*
 * fast log2 implementation
 * from https://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
 */

static const i32 tab32[32] = {
        0,  9,  1, 10, 13, 21,  2, 29,
        11, 14, 16, 18, 22, 25,  3, 30,
        8, 12, 20, 28, 15, 17, 24,  7,
        19, 27, 23,  6, 26,  5,  4, 31};

static i32 log2_32 (u32 value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return tab32[(u32)(value*0x07C4ACDD) >> 27];
}

int myprintf(const char *format, ...){
    static const char *lower_hex = "0123456789abcdef", *upper_hex = "0123456789ABCDEF";
    va_list list;
    va_start(list, format);
    int cnt = 0;
    static char buf[100];
    for(const char *it = format; *it ; ++it){
        if(*it == '%'){
            ++it;
            switch (*it) {
                case '%':
                    fputc('%', stdout);
                    ++cnt;
                    break;
                case 'd':
                case 'i': {
                    const i32 base = 10;
                    i32 n = va_arg(list, i32);
                    if(n < 0) {
                        fputc('-', stdout);
                        n = -n;
                        ++cnt;
                    }
                    i32 stk = 0;
                    while(n){
                        buf[stk++] = (n%base) + '0';
                        n /= base;
                        ++cnt;
                    }
                    for(i32 i = stk-1 ; i >= 0 ; --i){
                        fputc(buf[i], stdout);
                    }
                    break;
                }
                case 'x': {
                    const i32 base = 16;
                    i32 n = va_arg(list, i32);
                    if(n < 0) {
                        fputc('-', stdout);
                        n = -n;
                        ++cnt;
                    }
                    i32 stk = 0;
                    while(n){
                        buf[stk++] = lower_hex[(n%base)];
                        n /= base;
                        ++cnt;
                    }
                    for(i32 i = stk-1 ; i >= 0 ; --i){
                        fputc(buf[i], stdout);
                    }
                    break;
                }
                case 'X': {
                    const i32 base = 16;
                    i32 n = va_arg(list, i32);
                    if(n < 0) {
                        fputc('-', stdout);
                        n = -n;
                        ++cnt;
                    }
                    i32 stk = 0;
                    while(n){
                        buf[stk++] = upper_hex[(n%base)];
                        n /= base;
                        ++cnt;
                    }
                    for(i32 i = stk-1 ; i >= 0 ; --i){
                        fputc(buf[i], stdout);
                    }
                    break;
                }
                case 's': {
                    const char *s = va_arg(list, char *);
                    for(; *s ; ++s){
                        ++cnt;
                        fputc(*s, stdout);
                    }
                    break;
                }
                // null terminator
                case 0:
                    --it; // prevent invalid memory access
                    break;
                default:
                    // parse fail => do nothing
                    fputc('%', stdout);
                    fputc(*it, stdout);
                    cnt += 2;
                    break;
            }
        }
        else{
            fputc(*it, stdout);
            ++cnt;
        }
    }
    va_end(list);
    return cnt;
}
