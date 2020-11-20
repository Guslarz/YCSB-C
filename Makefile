CC=g++
CFLAGS=-std=c++11 -g -Wall -pthread -I./ \
       -I./leveldb/include -I./lsm_nvm/include \
       -L./leveldb/build -L./lsm_nvm/out-static
LDFLAGS=-lpthread -ltbb -lleveldb -lnovelsm -lnuma
SUBDIRS=core db db/utils
SUBSRCS=$(wildcard core/*.cc) $(wildcard db/*.cc) $(wildcard db/utils/*.cc)
OBJECTS=$(SUBSRCS:.cc=.o)
EXEC=ycsbc

all: $(SUBDIRS) $(EXEC)
	make -C lsm_nvm
	mkdir -p leveldb/build
	cd leveldb/build
	cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
	cd ../..

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

