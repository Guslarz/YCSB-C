CC=g++
CFLAGS=-std=c++11 -c -g -Wall -fgnu-tm
LEVELDB_INCLUDE_PATH=../../libraries/leveldb/include
INCLUDES=-I../ -I$(LEVELDB_INCLUDE_PATH)
SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)

all: $(SOURCES) $(OBJECTS)

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	$(RM) $(OBJECTS)

