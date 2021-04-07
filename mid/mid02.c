#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic.h"

struct bmpHeader {
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    int32_t hres;
    int32_t vres;
    uint32_t used;
    uint32_t important;
} __attribute__((__packed__));

struct pix_t {
    byte b, g, r;
} __attribute__((__packed__));

// helper

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// @param i is height index
// @param j is width index
inline static i32 row_calc(i32 i, i32 j, i32 h, i32 w) {
    return MIN(w - j, h - i);
}

int main() {
    static char filename[1024];
    printf("Please enter the file name: ");
    fgets_n(filename, 1024, stdin);
    FILE *inBmp = NULL, *outBmp = NULL;

    if ((inBmp = fopen(filename, "rb")) == NULL) {
        printf("File could not be opened!\n");
        return 1;
    }

    if ((outBmp = fopen("output.bmp", "wb")) == NULL) {
        printf("Outpu file could not be opened!\n");
        return 1;
    }
    // read write header
    struct bmpHeader header, outHeader;
    fread(&header, 54, 1, inBmp);
    memcpy(&outHeader, &header, sizeof(header));

    // calc info
    i32 san_w = abs(header.width), san_h = abs(header.height);
    i32 ipad_siz = (((24 * san_w + 31) / 32) * 4) - (3 * san_w);

    // modify and write output header
    outHeader.height = san_h + san_w;
    outHeader.width = san_h + san_w;
    fwrite(&outHeader, sizeof(outHeader), 1, outBmp);

    // read pixel to buf
    struct pix_t *pix_buf = calloc(san_w * san_h, sizeof(struct pix_t));
    static byte junk[4];
    for (i32 i = 0; i < san_h; ++i) {
        for (i32 j = 0; j < san_w; ++j) {
            struct pix_t *pBuf = NULL;
            pBuf = pix_buf + (san_w * i + j);
            fread(pBuf, sizeof(struct pix_t), 1, inBmp);
        }
        fread(junk, ipad_siz, 1, inBmp);
    }

    // calc out bmp info
    i32 san_ow = outHeader.width, san_oh = outHeader.height;
    i32 opad_siz = (((24 * san_ow + 31) / 32) * 4) - (3 * san_ow);

    // setup padding
    static const byte padding[4] = {0};

    static const struct pix_t white = {.r = 0xff, .g = 0xff, .b = 0xff};
    i32 ix = 0, jx = san_w - 1;
    // write pixel by buf with rotate access
    for (i32 i = 0; i < san_oh; ++i) {
        i32 row_len = row_calc(ix, jx, san_h, san_w);
        i32 indent = (san_ow - row_len) / 2;
        // row write
        for (i32 j = 0; j < san_ow; ++j) {
            const struct pix_t *pBuf = NULL;
            if (j < indent && (j - indent) < row_len) { // in white area
                pBuf = &white;
            } else { // in contentful area
                pBuf = pix_buf + ((ix + j) * san_w + (jx - j));
            }
            fwrite(pBuf, sizeof(struct pix_t), 1, outBmp);
        }
        // fill in padding
        fwrite(padding, opad_siz, 1, outBmp);
        // mv idx
        if (jx <= 0) {
            ++ix;
        } else {
            --jx;
        }
    }
    fclose(inBmp);
    fclose(outBmp);
    free(pix_buf);
    return 0;
}