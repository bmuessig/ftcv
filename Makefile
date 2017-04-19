CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=ftcv.c recv.c send.c proto.c utils.c buffers.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ftcv.elf

#CFLAGS:=-v -da -Q

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(EXECUTABLE)
