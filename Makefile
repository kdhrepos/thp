
CC=gcc

PROG=thp

default:
	$(CC) -o $(PROG) $(PROG).c

run:
	./$(PROG)
