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

TARGETS=main
main_OBJ=main.o basic.o

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

#hw0606.pdf: hw0606.tex
#	mkdir -p $(BUILD_DIR)
#	$(XELATEX) $(LATEXFLAGS) $<
#	$(XELATEX) $(LATEXFLAGS) $<
#	cp $(BUILD_DIR)/*.pdf .
#	rm -rf $(BUILD_DIR)

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf $(TARGETS) obj