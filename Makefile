CFLAGS=-Wall -g -pedantic

PROGS=self-linker.c

self-linker: self-linker.c
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(PROGS)