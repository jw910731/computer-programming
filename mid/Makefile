CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
LDFLAGS=-lm

TARGETS=mid01 mid03 mid04
mid01_OBJ=mid01.o
mid02_OBJ=basic_math.o mid02.o
mid03_OBJ=mid03.o
mid04_OBJ=mid04.o

.PHONY: all

all: $(TARGETS) 

.SECONDEXPANSION:
$(TARGETS): $$($$@_OBJ)

%.o: $@.c

clean:
	rm -rf $(TARGETS) $(foreach targ,$(TARGETS),$(foreach obj, $($(targ)_OBJ), $(obj)))