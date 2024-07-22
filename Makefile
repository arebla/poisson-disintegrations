CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: simple_decay decay_chain branched_decay

simple_decay: src/simple_decay.c
	$(CC) $(CFLAGS) -o simple_decay src/simple_decay.c

decay_chain: src/decay_chain.c
	$(CC) $(CFLAGS) -o decay_chain src/decay_chain.c

branched_decay: src/branched_decay.c
	$(CC) $(CFLAGS) -o branched_decay src/branched_decay.c

clean:
	rm -f simple_decay decay_chain branched_decay
