CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=c11
LDFLAGS=-lm 

# XeLaTeX Settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
XELATEX=/Library/TeX/texbin/xelatex
else
XELATEX=xelatex
endif
BUILD_DIR=$(shell [ -d build ] || mkdir build && echo "build")

TARGETS=hw0201 hw0202 hw0203
hw0201_OBJ= basic.o hw0201.o mixed.o
hw0202_OBJ= basic.o hw0202.o
hw0203_OBJ= basic.o hw0203.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O3
all: $(TARGETS) 

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan 
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: $(TARGETS)

#doc: example.pdf

#example.pdf: example.tex
#	$(XELATEX) $(LATEXFLAGS) -output-directory=$(BUILD_DIR) $<
#	$(XELATEX) $(LATEXFLAGS) -output-directory=$(BUILD_DIR) $<
#	cp $(BUILD_DIR)/*.pdf .

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf $(TARGETS) $(BUILD_DIR) obj