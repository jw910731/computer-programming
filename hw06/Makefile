CC=gcc
OBJDIR:=$(shell [ -d obj ] || mkdir obj && echo "obj")
CFLAGS=-Wall -Wextra -std=gnu11
LDFLAGS=-lm 

TARGETS=hw0601 hw0602 hw0603# hw0604 hw0605
hw0601_OBJ= hw0601.o basic.o
hw0602_OBJ= hw0602.o basic.o undo.o queue.o
hw0603_OBJ= hw0603.o basic.o rotate.o
hw0604_OBJ= hw0604.o basic.o
hw0605_OBJ= hw0605.o basic.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O2 
all: $(TARGETS) 

debug: CFLAGS:=$(CFLAGS) -g -DDEBUG -fsanitize=leak -fsanitize=undefined
debug: LDFLAGS:=$(LDFLAGS) -fsanitize=address -lubsan -lasan 
debug: $(TARGETS)

dev: CFLAGS:=$(CFLAGS) -g -DDEBUG
dev: LDFLAGS:=$(LDFLAGS) 
dev: $(TARGETS)

.SECONDEXPANSION:
$(TARGETS): $$(patsubst %, $(OBJDIR)/%, $$($$@_OBJ))
	$(CC) $(filter %.o, $^) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	rm -rf $(TARGETS) obj