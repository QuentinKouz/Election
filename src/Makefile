CC = gcc
CFLAGS = -std=c99 -pedantic -Wall

verify_my_vote: verify_my_vote.c
	$(CC) $(CFLAGS) -o verify_my_vote verify_my_vote.c

.PHONY: clean

clean:
	rm -f verify_my_vote
