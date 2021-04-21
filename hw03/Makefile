CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=c11 -I./linenoise -D_DEFAULT_SOURCE
LDFLAGS=-lm

# XeLaTeX Settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
XELATEX=/Library/TeX/texbin/xelatex
else
XELATEX=xelatex
endif
BUILD_DIR=$(shell [ -d build ] || mkdir build && echo "build")
TARGETS=hw0301 hw0302 hw0304
hw0301_OBJ=hw0301.o linenoise.o file_selector.o basic.o
hw0302_OBJ=hw0302.o linenoise.o basic.o file_selector.o
hw0303_OBJ=hw0303.o basic.o
hw0304_OBJ=hw0304.o basic.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O3
all: $(TARGETS) 

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan 
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: $(TARGETS)

doc: example.pdf

example.pdf: example.tex
	$(XELATEX) $(LATEXFLAGS) -output-directory=$(BUILD_DIR) $<
	$(XELATEX) $(LATEXFLAGS) -output-directory=$(BUILD_DIR) $<
	cp $(BUILD_DIR)/*.pdf .

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

$(OBJDIR)/linenoise.o : linenoise/linenoise.c
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf $(TARGETS) $(BUILD_DIR) obj
