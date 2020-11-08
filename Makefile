CC=g++
CFLAGS=-std=c++11 -g -Wall -pthread -I./ -L/usr/local/lib
LDFLAGS=-lpthread -ltbb -lleveldb -lnovelsm -lnuma
SUBDIRS=core db db/utils
SUBSRCS=$(wildcard core/*.cc) $(wildcard db/*.cc) $(wildcard db/utils/*.cc)
OBJECTS=$(SUBSRCS:.cc=.o)
EXEC=ycsbc

all: $(SUBDIRS) $(EXEC)

$(SUBDIRS):
	$(MAKE) -C $@

$(EXEC): $(wildcard *.cc) $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
	$(RM) $(EXEC)

.PHONY: $(SUBDIRS) $(EXEC)

