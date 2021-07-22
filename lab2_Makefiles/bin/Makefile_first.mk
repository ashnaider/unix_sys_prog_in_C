OBJS = main.o func.o print.o module.o wordfunc.o

INCL_DIR = ../include
SRC_DIR = ../src

TARGET = program_hui

CFLAGS = -Wall -I$(INCL_DIR)

# VPATH=$(INCL_DIR) $(SRC_DIR)

vpath %.h $(INCL_DIR)
vpath %.c $(SRC_DIR)

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $^


$(OBJS): defs.h


.PHONY: clean

clean:
	rm $(OBJS) $(TARGET)
