CC = gcc

CP = cp -rf

SRC_TREE = ../..

SRC_DIR = ${SRC_TREE}/src

BIN_DIR = ${SRC_TREE}/bin

INC_DIR = ${SRC_TREE}/include

#LIB_DIR = ${SRC_TREE}/lib
LIB_DIR =

PREFIX = /usr/local

EXE_NAME = tab2spaces

CFLAGS = -g -Wall -iquote "${INC_DIR}"

#LDFLAGS = -rdynamic
LDFLAGS =

#LIBS = -l m -L ${LIB_DIR} -l dl
#LIBS = -l m -L ${LIB_DIR}
LIBS = -l m

all: $(EXE_NAME)

$(EXE_NAME): tab2spaces.c
	$(CC) -o $(EXE_NAME) tab2spaces.c

install: all
	$(CP) $(EXE_NAME) $(PREFIX)/bin
	$(CP) $(EXE_NAME).sh $(PREFIX)/bin

clean:
	rm -rf *~ *.bak *.o $(EXE_NAME)

