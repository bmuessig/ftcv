CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=ftcv.c recv.c send.c utils.c buffers.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ftcv

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.o:
    $(CC) $(CFLAGS) $< -o $@
