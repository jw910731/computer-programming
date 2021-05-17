#include "basic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum FileItemType {
    F_FILE,
    F_DIR
} FileItemType;

typedef struct FileItem {
    int len, cap;
    char *val;
    struct FileItem **children;
    int is_dir;
} FileItem;

void fileitem_del(FileItem *item) {
    free(item->val);
    for (int i = 0; i < item->len; i++) {
        fileitem_del(item->children[i]);
    }
    free(item->children);
    free(item);
}

FileItem *insert(FileItem *top, char *val) {
    if (val == NULL || *val == '\0') return top;
    debug("ins: '%s'\n", val);
    char val_cpy[8192];
    strncpy(val_cpy, val, 8191);
    int n = 0;
    char *f = strtok(val_cpy, "/\\");
    if (f == NULL) return top;
    debug("f: %s\n", f);
    for (int i = 0; i < top->len; i++) {
        if (!strcmp(f, top->children[i]->val)) {
            return insert(top->children[i], val+strlen(f) + 1);
        }
    }
    if (top->len > top->cap - 2) {
        top->cap = (top->cap + 1) * 2;
        top->children = realloc(top->children, top->cap * sizeof(FileItem*));
        memset(top->children + top->len, 0, top->cap - top->len);
    }
    top->children[top->len] = malloc(sizeof(FileItem));
    top->children[top->len]->len = top->children[top->len]->cap = 0;
    top->children[top->len]->children = NULL;
    top->children[top->len]->is_dir = 0;
    top->children[top->len]->val = strndup(f, 8192);
    if (val[strlen(val) - 1] == '/') {
        top->children[top->len]->is_dir = 1;
    }

    return insert(top->children[top->len++], val+strlen(f));
}
void dump(FileItem *item, int indent) {
    if (indent != -1) {
        for (int i = 0; i < indent * 2; i++) {
            printf(" ");
        }
        printf("%s", item->val);
        if (item->is_dir) printf("/");
        printf("\n");
    }
    for (int i = 0; i < item->len; i++) {
        dump(item->children[i], indent+1);
    }
}
int main(int argc, char **argv) {
    if(argc < 2){
        fputs("Insufficient argument.\n"
              "Usage: ./hw0404 <zip file>\n", stderr);
        return 1;
    }
    FileItem *root = malloc(sizeof(FileItem));
    root->cap = root->len = 0;
    root->val = strndup("", 10);
    root->children = NULL;

    char filename[8192];
    strncpy(filename, argv[1], 8191);
    int fd = open(filename, O_RDONLY);
    if (fd < 0)  {
        perror("fail to open file:");
        return 1;
    }

    lseek(fd, 0, SEEK_SET);
    while (1) {
        ssize_t cur = lseek(fd, 0, SEEK_CUR);
        lseek(fd, cur, SEEK_SET);
        lseek(fd, 26, SEEK_CUR);
        uint16_t fname_l = 0, ext_len = 0;
        read(fd, &fname_l, 2);
        read(fd, &ext_len, 2);
        debug("n: %u\n", (unsigned int)fname_l);
        if (!fname_l) break;
        debug("m: %u\n", (unsigned int)ext_len);
        uint32_t comp_size = 0;
        lseek(fd, cur, SEEK_SET);
        lseek(fd, 18, SEEK_CUR);
        read(fd, &comp_size, 4);
        lseek(fd, cur, SEEK_SET);
        lseek(fd, 30, SEEK_CUR);
        debug("compressed_size: %u\n", (unsigned int)comp_size);
        char fname[fname_l + 5];
        fname[fname_l] = '\0';
        read(fd, fname, fname_l);
        debug("fname: %s\n", fname);
        lseek(fd, cur, SEEK_SET);
        lseek(fd, 30+fname_l+ext_len+comp_size, SEEK_CUR);
        int32_t sig;
        insert(root, fname);

    }
    dump(root, -1);
    fileitem_del(root);
    return 0;
}
