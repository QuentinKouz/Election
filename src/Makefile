CC = gcc
CFLAGS = -w -std=c99 -pedantic -Wall
SOURCES = verify_my_vote.c sha256.c sha256_utils.c csv.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = verify_my_vote

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)