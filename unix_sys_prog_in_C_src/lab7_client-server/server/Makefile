all: server

BIN_DIR = bin

server: bin
	gcc -o $(BIN_DIR)/server ../utils.c -I../ server.c


bin: 
	mkdir $(BIN_DIR)


.PHONY: all, clean

clean:
	rm -rf $(BIN_DIR)

