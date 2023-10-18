CC=gcc
CFLAGS=-Wall -ansi
LDFLAGS=-Wall -ansi
SRCS = Verify_my_vote.c
OBJS = Verify_my_vote.o
TARGET = Verify_my_vote

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)