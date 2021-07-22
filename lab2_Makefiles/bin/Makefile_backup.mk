OBJS = main.o func.o print.o module.o wordfunc.o

INCL_DIR = ../include
SRC_DIR = ../src

TARGET = program

CFLAGS = -Wall -I$(INCL_DIR)

# vpath %.h ../include
# vpath %.c ../src

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJS):
	$(CC) -c $(SRC_DIR)/* $(CFLAGS)

clean:
	rm $(OBJS) $(TARGET)

.PHONY: clean
