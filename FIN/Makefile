CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=c11 $(shell curl-config --cflags)
LDFLAGS=-lm $(shell curl-config --libs)

TARGETS = fin02 fin03 fin01
fin01_OBJ = fin01.o basic.o poly.o
fin02_OBJ = fin02.o basic.o vec.o
fin03_OBJ = fin03.o basic.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O3
all: $(TARGETS) 

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan 
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: $(TARGETS)

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf $(TARGETS) $(BUILD_DIR) obj