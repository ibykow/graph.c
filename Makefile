INC_FLAGS = -Iinc -Itests
CFLAGS = -Wall $(INC_FLAGS)
CC = cc

all:
	$(CC) src/*.c $(CFLAGS) -O3 -o bin/test

clean:
	rm -rf bin/*

test:
	clear && $(CC) src/*.c tests/test.c -Wall -Iinc -O3 -o bin/test && bin/test

debug:
	clear && $(CC) src/*.c tests/test.c -Wall -Iinc -O0 -g -o bin/test && lldb bin/test
