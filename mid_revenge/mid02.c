#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define MAX(a, b) ((a > b) ? (a) : (b))

inline static void print_vec(struct vec_t v) {
    printf("(%lf, %lf)\n", v.x, v.y);
}

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
    return (struct vec_t){ceil(fabs(MIN(MIN(lt.x, lb.x), MIN(rt.x, rb.x)))),
                          ceil(fabs(MIN(MIN(lt.y, lb.y), MIN(rt.y, rb.y))))};
}

inline static fp area(struct vec_t a, struct vec_t b, struct vec_t c) {
    return fabs((a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) /
                2.0);
}

bool ck_Picture(const struct picture_t *pic, i32 x, i32 y, fp deg,
                const struct vec_t off) {
    const struct vec_t pos = {x, y};
    struct vec_t pts[4] = {{0, pic->h}, {0, 0}, {pic->w, 0}, {pic->w, pic->h}};
    for (i32 i = 0; i < 4; ++i) {
        pts[i] = add(off, rotate_vec(pic, pts[i], deg));
    }
    for (i32 i = 0; i < 4; ++i) {
        if (cross(sub(pts[(i + 1) % 4], pts[i]), sub(pos, pts[i])) < 0) {
            return false;
        }
    }
    return true;
}

#define SQ(a) ((a) * (a))
fp dist3d(const struct pix_t a, const struct pix_t b) {
    return sqrt(SQ(a.b - b.b) + SQ(a.g - b.g) + SQ(a.r - b.r));
}

struct pix_t add3d(const struct pix_t a, const struct pix_t b) {
    return (struct pix_t){a.b + b.b, a.g + b.g, a.r + b.r};
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
    fp rot_deg = PI * in_deg / 180.0;

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
    struct vec_t diag[2] = {rotate(sub((struct vec_t){.x = 0, .y = inPic.h},
                                       (struct vec_t){inPic.w, 0}),
                                   rot_deg),
                            rotate(sub((struct vec_t){.x = 0, .y = 0},
                                       (struct vec_t){inPic.w, inPic.h}),
                                   rot_deg)};
    i32 san_oh = ceil(MAX(fabs(diag[0].y), fabs(diag[1].y))) + 1,
        san_ow = ceil(MAX(fabs(diag[0].x), fabs(diag[1].x))) + 1;
    printf("ow: %d oh: %d\n", san_ow, san_oh);
    struct vec_t offset_vec = off_vec_gen(&inPic, rot_deg);
    fputs("Offset Vector: ", stdout);
    print_vec(offset_vec);
    struct picture_t outPic = {
        .w = san_ow,
        .h = san_oh,
        .blob = calloc(san_oh * san_ow, sizeof(struct pix_t))};
    // update data to header
    outHeader.width = outPic.w;
    outHeader.height = outPic.h;
    // clear whole picture to white
    memset(outPic.blob, 0xff, outPic.w * outPic.h * sizeof(struct pix_t));

    bool *vis_map = calloc(outPic.w * outPic.h, sizeof(bool));

    // rotate picture
    for (i32 i = 0; i < inPic.h; ++i) {
        for (i32 j = 0; j < inPic.w; ++j) {
            struct vec_t rot = add(
                offset_vec, rotate_vec(&inPic, (struct vec_t){j, i}, rot_deg));
            i32 ix = round(rot.x), iy = round(rot.y);
            outPic.blob[outPic.w * iy + ix] = inPic.blob[inPic.w * i + j];
            vis_map[outPic.w * iy + ix] = true;
        }
    }
    // blend the blank pixel
    for (i32 i = 0; i < outPic.h; ++i) {
        for (i32 j = 0; j < outPic.w; ++j) {
            if (ck_Picture(&inPic, j, i, rot_deg, offset_vec) &&
                !vis_map[outPic.w * i + j]) {
                static const i32 dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
                // calc average
                i32 cnt = 0;
                struct pix_t avg_clr = {0, 0, 0};
                for (i32 k = 0; k < 4; ++k) {
                    if (((i + dx[k]) >= 0 && (i + dx[k]) < outPic.h &&
                         j + dy[k] >= 0 && j + dy[k] < outPic.w) &&
                        vis_map[outPic.w * (i + dx[k]) + j + dy[k]]) {
                        avg_clr = add3d(
                            outPic.blob[outPic.w * (i + dx[k]) + j + dy[k]],
                            avg_clr);
                        ++cnt;
                    }
                }
                avg_clr = (struct pix_t){.b = avg_clr.b / cnt,
                                         .g = avg_clr.g / cnt,
                                         .r = avg_clr.r / cnt};
                i32 x, y;
                fp dist = 65535.0;
                // get color nearest point
                for (i32 k = 0; k < 4; ++k) {
                    if (((i + dx[k]) >= 0 && (i + dx[k]) < outPic.h &&
                         j + dy[k] >= 0 && j + dy[k] < outPic.w) &&
                        vis_map[outPic.w * (i + dx[k]) + j + dy[k]]) {
                        const fp clr_dist = dist3d(
                            avg_clr,
                            outPic.blob[outPic.w * (i + dx[k]) + j + dy[k]]);
                        if (clr_dist < dist) {
                            dist = clr_dist;
                            x = j + dy[k];
                            y = i + dx[k];
                        }
                    }
                }
                outPic.blob[outPic.w * i + j] = outPic.blob[outPic.w * y + x];
            }
        }
    }

    i32 opad_siz = (((24 * san_ow + 31) / 32) * 4) - (3 * san_ow);

    // setup padding
    static const byte padding[4] = {0};

    outHeader.size =
        outPic.w * outPic.h * sizeof(struct pix_t) + sizeof(header);
    outHeader.bitmap_size = 0;

    fwrite(&outHeader, sizeof(outHeader), 1, outBmp);

    // write pixel by buf with rotate access
    for (i32 i = 0; i < san_oh; ++i) {
        fwrite(outPic.blob + (outPic.w * i), outPic.w, sizeof(struct pix_t),
               outBmp);
        // fill in padding
        fwrite(padding, opad_siz, 1, outBmp);
    }

    // free resources
    free(vis_map);
    free(inPic.blob);
    free(outPic.blob);
    fclose(inBmp);
    fclose(outBmp);
    return 0;
}