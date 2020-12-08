CC=gcc
CFLAGS=-Wall -Wextra -std=c11
LDFLAGS=-lm 

TARGETS=hw0501 hw0502 hw0503 hw0504 hw0505
hw0501_OBJ=hw0501.o basic.o sort.o
hw0502_OBJ=hw0502.o basic.o matrix.o
hw0503_OBJ=hw0503.o basic.o polynominal.o
hw0504_OBJ=hw0504.o basic.o analyze.o
hw0505_OBJ=hw0505.o basic.o minesweeper.o terminal.o queue.o

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
$(TARGETS): $$($$@_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: $@.c

clean:
	rm -rf $(TARGETS) $(foreach targ,$(TARGETS),$(foreach obj, $($(targ)_OBJ), $(obj)))