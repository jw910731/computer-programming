CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=gnu11
LDFLAGS=-lm 

# XeLaTeX Settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
XELATEX=/Library/TeX/texbin/xelatex
else
XELATEX=xelatex
endif
BUILD_DIR=build
LATEXFLAGS=-output-directory=$(BUILD_DIR)

TARGETS=hw0601 hw0602 hw0603 hw0604 hw0605
hw0601_OBJ= hw0601.o basic.o
hw0602_OBJ= hw0602.o basic.o undo.o queue.o
hw0603_OBJ= hw0603.o basic.o geometry.o
hw0604_OBJ= hw0604.o basic.o geometry.o
hw0605_OBJ= hw0605.o basic.o cardutil.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O2 
all: $(TARGETS) 

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan 
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: LDFLAGS:=$(LDFLAGS) 
dev: $(TARGETS)

doc: hw0606.pdf

hw0606.pdf: hw0606.tex
	mkdir -p $(BUILD_DIR)
	$(XELATEX) $(LATEXFLAGS) $<
	$(XELATEX) $(LATEXFLAGS) $<
	cp $(BUILD_DIR)/*.pdf .
	rm -rf $(BUILD_DIR)

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf $(TARGETS) obj