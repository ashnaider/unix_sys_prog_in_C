CFLAGS = -Wall

FILES = main.o func.o module.o print.o wordfunc.o

prog: $(FILES)
	$(CC) -o $@ $^

vpath %.c .

$(FILES): main.c func.c module.c print.c wordfunc.c
	$(CC) $^ -c

.PHONY: clean

clean:
	rm prog *.o
