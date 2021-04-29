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

    // input rotation angle
    i32 angle = 0, ret = -2;
    do{
        if(ret != -2){
            puts("Input Error!");
        }
        printf("Angle (1-90): ");
        ret = scanf("%d", &angle);
        if(angle <= 0 || angle > 90){
            ret = 0;
        }
        while(getchar() != '\n');
    }while(ret != 1);

    if(angle == 90){
        return 0;
    }

    double rad_angle = (angle/180.0) * PI;

    // setup outBmpHeader
    struct bmpHeader inHeader, outHeader;
    fread( &inHeader, 54, 1, inFile );
    memcpy(&outHeader, &inHeader, sizeof(struct bmpHeader));
    outHeader.width += (i32)(outHeader.height / tan(rad_angle)) + 1; // add addition width related to height

    size_t header_len = sizeof(struct bmpHeader), bm_len = 0;
    fseek(outFile, sizeof(struct bmpHeader), SEEK_CUR);

    // setup white dummy buffer
    struct pix24_t *white_buf = malloc(sizeof(struct pix24_t) * outHeader.width);
    memset(white_buf, 0xff, sizeof(struct pix24_t)*outHeader.width);

    size_t buf_siz = ((sizeof(struct pix24_t) * inHeader.width + 3)>>2)<<2;
    struct pix24_t *row_buf = malloc(buf_siz);
    size_t indent_siz = (4 - ((sizeof(struct pix24_t) * outHeader.width) % 4)) % 4;
    for(i32 i = 0 ; i < inHeader.height ; ++i){
        fread(row_buf, 1, buf_siz, inFile);
        i32 white_len = floor(i / tan(rad_angle));
        bm_len += fwrite(white_buf, 1, sizeof(struct pix24_t) * white_len, outFile); // write white section
        bm_len += fwrite(row_buf, 1, sizeof(struct pix24_t) * inHeader.width, outFile); // write picture section
        // add indent and extra white to remained area
        bm_len += fwrite(white_buf, 1, sizeof(struct pix24_t) * (outHeader.width - inHeader.width - white_len) + indent_siz, outFile);
    }
    free(row_buf);
    free(white_buf);

    outHeader.size = header_len + bm_len;
    outHeader.bitmap_size = bm_len;

    rewind(outFile);
    fwrite(&outHeader, 1, sizeof(struct bmpHeader), outFile);

    fclose(inFile);
    fclose(outFile);
    return 0;
}

