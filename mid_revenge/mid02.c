#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "basic.h"
#include "vec.h"

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

struct subpix_t {
    struct pix_t p;
    struct vec_t pos;
    struct subpix_t *next;
};

struct picture_t {
    struct pix_t *blob;
    int32_t w, h;
};

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) : (a) : (b))

inline static void print_vec(struct vec_t v) { printf("(%lf, %lf)\n", v.x, v.y); }

// Rotate vector relative to mid point
struct vec_t rotate_vec(const struct picture_t *p, struct vec_t v, fp deg) {
    const struct vec_t mid = {p->w / 2.0, p->h / 2.0};
    const struct vec_t trans = {v.x - mid.x, v.y - mid.y};
    return add(mid, rotate(trans, deg));
}

// Helper to generate offset vector
struct vec_t off_vec_gen(const struct picture_t *pic, fp deg) {
    struct vec_t lt = {pic->w, pic->h}, lb = {pic->w, 0}, rt = {0, pic->h},
                 rb = {0, 0};
    lt = rotate_vec(pic, lt, deg);
    lb = rotate_vec(pic, lb, deg);
    rt = rotate_vec(pic, rt, deg);
    rb = rotate_vec(pic, rb, deg);
    print_vec(lt);
    print_vec(lb);
    print_vec(rt);
    print_vec(rb);
    return (struct vec_t){ceil(fabs(MIN(rb.x, lt.x))),
                          ceil(fabs(MIN(lb.y, rt.y)))};
}

int main() {
    const fp PI = acos(-1);
    static char filename[1024];
    printf("Please enter the file name: ");
    fgets_n(filename, 1024, stdin);
    i32 in_deg;
    int ret = -127;
    do {
        if (ret != -127) {
            printf("Input Error, please input again.");
        }
        printf("Rotation angle (int , 0-360): ");
        ret = scanf("%d", &in_deg);
        if (ret == 1 && (in_deg < 0 || in_deg > 360))
            ret = 0;
        while (getchar() != '\n')
            ;
    } while (ret != 1);
    fp rot_deg = PI * (360 - in_deg) / 180.0;

    FILE *inBmp = NULL, *outBmp = NULL;

    if ((inBmp = fopen(filename, "rb")) == NULL) {
        printf("File could not be opened!\n");
        return 1;
    }

    if ((outBmp = fopen("output.bmp", "wb")) == NULL) {
        printf("Output file could not be opened!\n");
        return 1;
    }
    // read write header
    struct bmpHeader header, outHeader;
    fread(&header, 54, 1, inBmp);
    memcpy(&outHeader, &header, sizeof(header));

    // calc info
    struct picture_t inPic = {.w = abs(header.width),
                              .h = abs(header.height),
                              .blob =
                                  calloc(abs(header.width) * abs(header.height),
                                         sizeof(struct pix_t))};
    i32 ipad_siz = (((24 * inPic.w + 31) / 32) * 4) - (3 * inPic.w);

    // read data by row
    for (i32 i = 0; i < inPic.h; ++i) {
        fread(inPic.blob + (inPic.w * i), inPic.w, sizeof(struct pix_t), inBmp);
        // skip padding byts
        fseek(inBmp, ipad_siz, SEEK_CUR);
    }

    // calc out bmp info
    i32 san_oh = ceil(fabs(rotate(sub((struct vec_t){.x = 0, .y = inPic.h},
                                      (struct vec_t){inPic.w, 0}),
                                  rot_deg)
                               .y))+1,
        san_ow = ceil(fabs(rotate(sub((struct vec_t){.x = 0, .y = 0},
                                      (struct vec_t){inPic.w, inPic.h}),
                                  rot_deg)
                               .x))+1;
    printf("%d %d\n", san_ow, san_oh);
    struct vec_t offset_vec = off_vec_gen(&inPic, rot_deg);
    printf("(%lf, %lf)\n", offset_vec.x, offset_vec.y);
    struct picture_t outPic = {
        .w = san_ow,
        .h = san_oh,
        .blob = calloc(san_oh * san_ow, sizeof(struct pix_t))};
    // update data to header
    outHeader.width = outPic.w;
    outHeader.height = outPic.h;
    fwrite(&outHeader, sizeof(outHeader), 1, outBmp);
    // clear whole picture to white
    memset(outPic.blob, 0xff, outPic.w * outPic.h * sizeof(struct pix_t));

    struct subpix_t **subpix_arr = calloc(outPic.h * outPic.w, sizeof(struct subpix_t*));

    // rotate picture
    for (i32 i = 0; i < inPic.h; ++i) {
        for (i32 j = 0; j < inPic.w; ++j) {
            struct vec_t rot = add(offset_vec, rotate_vec(&inPic, (struct vec_t){j, i}, rot_deg));
            i32 ix = round(rot.x), iy = round(rot.y);
            struct subpix_t **l =  &subpix_arr[outPic.w*iy+ix];
            while(*l != NULL){
                l = &(*l)->next;
            }
            *l = calloc(1, sizeof(struct subpix_t));
            (*l)->pos = rot; // record position
            (*l)->p = inPic.blob[inPic.w * i + j]; // copy pixel data
            (*l)->next = NULL;
        }
    }

    // subpixel
    for(i32 i = 0 ; i < outPic.h ; ++i){
        for(i32 j = 0 ; j < outPic.w ; ++j){
            const struct subpix_t *l = subpix_arr[outPic.w * i + j];
            fp sb = 0, sr = 0, sg = 0;
            i32 cnt = 0;
            while(l != NULL) {
                sb += l->p.b;
                sg += l->p.g;
                sr += l->p.r;
                l = l->next;
                ++cnt;
            }
            if(cnt > 0){
                outPic.blob[outPic.w*i + j] = (struct pix_t){.b = round(sb/cnt), .g = round(sg/cnt), .r = round(sr/cnt)};
            }
        }
    }

    i32 opad_siz = (((24 * san_ow + 31) / 32) * 4) - (3 * san_ow);

    // setup padding
    static const byte padding[4] = {0};

    // write pixel by buf with rotate access
    for (i32 i = 0; i < san_oh; ++i) {
        fwrite(outPic.blob + (outPic.w * i), outPic.w, sizeof(struct pix_t),
               outBmp);
        // fill in padding
        fwrite(padding, opad_siz, 1, outBmp);
    }

    // free resources
    for(i32 i = 0 ; i < outPic.h ; ++i){
        for(i32 j = 0 ; j < outPic.w ; ++j){
            if(subpix_arr[i*outPic.w+j] != NULL){ // not null -> free list
                for(struct subpix_t *it = subpix_arr[i*outPic.w+j], *tmp ; it != NULL ; it = tmp){
                    tmp = it->next;
                    free(it);
                }
            }
        }
    }
    free(subpix_arr);
    free(inPic.blob);
    free(outPic.blob);
    fclose(inBmp);
    fclose(outBmp);
    return 0;
}