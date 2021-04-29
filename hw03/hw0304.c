#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic.h"

struct bmpHeader{
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
}__attribute__ ((__packed__));

struct pixelMask{
    uint32_t r, g, b, a;
}__attribute__ ((__packed__));

struct pix24_t{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__ ((__packed__));

struct pix32_t{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a : 5;
    uint8_t unused : 3;
}__attribute__ ((__packed__));

union pix_union{
    struct pix24_t b24;
    struct pix32_t b32;
}__attribute__ ((__packed__));

int main() {
    FILE *inFile = NULL, *outFile = NULL;
    char nameBuf[1024];

    printf("Please input the input BMP file name: ");
    fgets_n(nameBuf, 1024, stdin);
    inFile = fopen(nameBuf, "r");
    if(inFile == NULL){
        perror("Error opening file");
        return 1;
    }

    printf("Please input the output BMP file name: ");
    fgets_n(nameBuf, 1024, stdin);
    outFile = fopen(nameBuf, "w+");
    if(outFile == NULL){
        perror("Error opening file");
        return 1;
    }

    struct bmpHeader inHeader, outHeader;
    fread( &inHeader, 54, 1, inFile );
    memcpy(&outHeader, &inHeader, sizeof(struct bmpHeader));
    if(inHeader.bpp != 24){
        puts("Input picture is not 24 bit color depth.");
        return 1;
    }

    i32 alpha = 0, ret = -2;

    do{
        if(ret != -2){
            puts("Input Error!");
        }
        printf("Alpha (0-31): ");
        ret = scanf("%d", &alpha);
        if(alpha < 0 || alpha > 31){
            ret = 0;
        }
        while(getchar() != '\n');
    }while(ret != 1);

    size_t header_len = sizeof(struct bmpHeader), bm_len = 0;
    fseek(outFile, sizeof(struct bmpHeader), SEEK_CUR);

    struct pixelMask mask = {.a = 0x1f000000, .r = 0x00ff0000, .g = 0x0000ff00, .b = 0x000000ff};
    header_len += fwrite(&mask, 1, sizeof(mask), outFile);

    outHeader.offset = header_len;
    outHeader.bpp = 32;
    outHeader.compression = 6;

    union pix_union pixel;

    while(1){
        fread(&pixel.b24, 1, sizeof(struct pix24_t), inFile);
        if(feof(inFile)) break;
        pixel.b32.a = alpha;
        bm_len += fwrite(&pixel.b32, 1, sizeof(struct pix32_t), outFile);
    }

    outHeader.size = header_len + bm_len;
    outHeader.bitmap_size = bm_len;

    rewind(outFile);
    fwrite(&outHeader, 1, sizeof(struct bmpHeader), outFile);

    fclose(inFile);
    fclose(outFile);
    return 0;
}
