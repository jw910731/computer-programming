#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "basic.h"

#define PI (acos(-1))

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

struct pix24_t{
    uint8_t b;
    uint8_t g;
    uint8_t r;
}__attribute__ ((__packed__));

int main() {
    FILE *inFile = NULL, *outFile = NULL;
    char nameBuf[1024];

    printf("Please input the output BMP file name: ");
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

    i32 angle = 0, ret = -2;

    do{
        if(ret != -2){
            puts("Input Error!");
        }
        printf("Angle (0-90): ");
        ret = scanf("%d", &angle);
        if(angle < 0 || angle > 90){
            ret = 0;
        }
        while(getchar() != '\n');
    }while(ret != 1);

    double rad_angle = (angle/180.0) * PI;

    struct bmpHeader inHeader, outHeader;
    fread( &inHeader, 54, 1, inFile );
    memcpy(&outHeader, &inHeader, sizeof(struct bmpHeader));

    size_t header_len = sizeof(struct bmpHeader), bm_len = 0;
    fseek(outFile, sizeof(struct bmpHeader), SEEK_CUR);

    // TODO: Process picture

    outHeader.size = header_len + bm_len;
    outHeader.bitmap_size = bm_len;

    rewind(outFile);
    fwrite(&outHeader, 1, sizeof(struct bmpHeader), outFile);

    fclose(inFile);
    fclose(outFile);
    return 0;
}

