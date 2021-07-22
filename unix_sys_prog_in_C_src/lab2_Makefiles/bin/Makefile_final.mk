include_dir = ../include
src_dir = ../src

TARGET = all
CC = gcc

source_dirs = $(include_dir) $(src_dir)

src_search_wildcards = $(addsuffix /*.c,$(src_dir))

$(TARGET): $(notdir $(patsubst %.c,%.o,$(wildcard $(src_search_wildcards))))
	$(CC) $^ -o $@

vpath %.h $(include_dir)
vpath %.c $(src_dir)

%.o: %.c
	$(CC) -c -MD $(addprefix -I,$(include_dir)) $<

include $(wildcard *.d)


.PHONY: clean

clean:
	rm $(TARGET) *.d *.o
