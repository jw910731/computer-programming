CC=gcc
OBJDIR := $(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS := -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L $(shell curl-config --cflags) -I.
LDFLAGS := $(shell curl-config --libs)

# XeLaTeX Settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
XELATEX=/Library/TeX/texbin/xelatex
else
XELATEX=xelatex
endif
BUILD_DIR=$(shell [ -d build ] || mkdir build && echo "build")
TARGETS=hw0501 hw0502 hw0503
hw0501_OBJ=basic.o hw0501.o base64.o
hw0502_OBJ=basic.o hw0502.o
hw0503_OBJ=basic.o hw0503.o cJSON.o cJSON_Utils.o data_fetch_helper.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O3
all: $(TARGETS) 

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan 
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: $(TARGETS)

doc: hw0504.pdf

hw0504.pdf: hw0504.tex
	$(XELATEX) $(LATEXFLAGS) -output-directory=$(BUILD_DIR) $<
	$(XELATEX) $(LATEXFLAGS) -output-directory=$(BUILD_DIR) $<
	cp $(BUILD_DIR)/*.pdf .

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/cJSON.o: cJSON/cJSON.c
	$(CC) $(CFLAGS) -IcJSON -c -o $@ $<

$(OBJDIR)/cJSON_Utils.o: cJSON/cJSON_Utils.c
		$(CC) $(CFLAGS) -IcJSON -c -o $@ $<


clean:
	rm -rf $(TARGETS) $(BUILD_DIR) obj