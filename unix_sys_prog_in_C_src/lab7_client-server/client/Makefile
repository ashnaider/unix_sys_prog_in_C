all: client

BIN_DIR = bin


client: bin
	gcc -o $(BIN_DIR)/client ../utils.c -I../ client.c

bin: 
	mkdir $(BIN_DIR)


.PHONY: all clean

clean:
	rm -rf $(BIN_DIR)

