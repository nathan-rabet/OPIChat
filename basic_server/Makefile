CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99 -D_GNU_SOURCE

OBJS = basic_server.o

basic_server: $(OBJS)
	$(CC) -o basic_server $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) basic_server

.PHONY: clean
