CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99 -D_GNU_SOURCE

OBJS = basic_client.o

basic_client: $(OBJS)
	$(CC) $(CFLAGS) -o basic_client $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) basic_client

.PHONY: clean
