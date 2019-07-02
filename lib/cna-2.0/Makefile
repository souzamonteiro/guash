CC = gcc

SRC_TREE = ../../

SRC_DIR = ${SRC_TREE}/src

INC_DIR = ${SRC_TREE}/include

CFLAGS = -g -Wall -iquote "${INC_DIR}" -D _LINUX_

all: libcna.so

libcna.so: cna.c
	$(CC) $(CFLAGS) -Wall -export-dynamic -shared -fPIC -o libcna.so cna.c
	
clean:
	rm -rf *.o *.lo *.la *.so .libs
