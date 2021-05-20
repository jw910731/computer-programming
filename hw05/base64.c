//
// Created by jw910731 on 2021/5/20.
//

#include "base64.h"
#include "basic.h"

#include <unistd.h>
#include <fcntl.h>

#define MIN(x, y) (((x) < (y))?(x):(y))

#define BUF_SIZ 1020
static byte rd_buf[BUF_SIZ], wr_buf[2048];

/*
 * Heavily inspired by https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 */

int base64_encode(int infd, int outfd){
    static const char trans[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    while(1){
        // read data
        i64 rd = read(infd, rd_buf, BUF_SIZ);
        // end condition
        if(rd == 0) break;
        if(rd < 0){
            perror("read error");
            return -1;
        }

        // process read in data
        i64 wr_idx = 0;
        for(i64 i = 0; i < rd; i += 3){
            // get available data size
            i32 op = MIN(3, (i32)(rd - i));
            u32 n = 0;
            // encode data to unsigned 32 int
            switch (op) {
                case 3:
                    n |= ((u32)rd_buf[i+2]);
                    __attribute__((fallthrough));
                case 2:
                    n |= ((u32)rd_buf[i+1]) << 8;
                    __attribute__((fallthrough));
                case 1:
                    n |= ((u32)rd_buf[i]) << 16;
                    break;
                case 0:
                default:
                    break;
            }

            // truncate data to 6 bit
            byte enc[4]; // high to low
            for(i32 j = 0 ; j < 4 ; ++j){
                // shift 18 12 6 0
                enc[j] = (byte)((n >> ((3-j)*6)) & 63);
            }

            // output data
            for(i32 j = 0 ; j < op+1 ; ++j){
                wr_buf[wr_idx++] = trans[enc[j]];
            }
            // add padding
            for(i32 j = 4 ; j > op+1 ; --j){
                wr_buf[wr_idx++] = '=';
            }
        }

        // write data
        write(outfd, wr_buf, wr_idx);
    }
    return 0;
}

int base64_decode(int infd, int outfd){
#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66
    // table for decode, 66 means error character
    static const byte decode[] = {
            66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
            66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
            54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
            29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
            66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
            66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
            66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
            66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
            66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
            66,66,66,66,66,66
    };
    while(1){
        // read data
        i64 rd = read(infd, rd_buf, BUF_SIZ);
        // end condition
        if(rd == 0) break;
        if(rd < 0){
            perror("read error");
            return 1;
        }

        // process data
        bool end_flag = true;
        u64 buf = 0;
        i64 wr_idx = 0;
        i32 iter = 0;
        for(i64 i = 0 ; i < rd && end_flag ; ++i){
            byte c = decode[rd_buf[i]];
            switch (c) {
                case WHITESPACE: // skip whitespace
                    break;
                case INVALID: // invalid input, return error
                    return 1;
                case EQUALS: // pad character is end of data
                    end_flag = false;
                    break;
                default:
                    buf = buf << 6 | c;
                    iter++; // increment the number of iteration
                    /* If the buffer is full, split it into bytes */
                    if (iter == 4) {
                        wr_buf[wr_idx++] = (buf >> 16) & 255;
                        wr_buf[wr_idx++] = (buf >> 8) & 255;
                        wr_buf[wr_idx++] = buf & 255;
                        buf = 0;
                        iter = 0;
                    }
                    break;
            }
        }

        // write data
        write(outfd, wr_buf, wr_idx);
    }
    return 0;
#undef WHITESPACE
#undef EQUALS
#undef INVALID
}