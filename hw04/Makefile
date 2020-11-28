CC=gcc
CFLAGS=-Wall -Wextra -std=c11
LDFLAGS=-lm

TARGETS=hw0401 hw0402-1 hw0402-2 hw0403 hw0404 hw0405
hw0401_OBJ=hw0401.o basic.o roman.o
hw0402-1_OBJ=hw0402_1.o hanoi_recurse.o
hw0402-2_OBJ=hw0402_2.o hanoi_iterate.o
hw0403_OBJ=hw0403.o basic.o taylor.o
hw0404_OBJ=hw0404.o basic.o resistor.o
hw0405_OBJ=hw0405.o basic.o guess_number.o

.PHONY: all

all: CFLAGS:=$(CFLAGS) -O2 
all: $(TARGETS) 

dev: CFLAGS:=$(CFLAGS) -g
dev: $(TARGETS)

.SECONDEXPANSION:
$(TARGETS): $$($$@_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: $@.c

clean:
	rm -rf $(TARGETS) $(foreach targ,$(TARGETS),$(foreach obj, $($(targ)_OBJ), $(obj)))