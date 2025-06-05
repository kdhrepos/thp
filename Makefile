
CC = gcc

APPS = thp thp_malloc

all: $(APPS)

%: %.c
	$(CC) -o $@ $<

run:
	./$(APPS)
