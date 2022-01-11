CC=gcc
CFLAGS = -Wextra -Wall -g -std=gnu99

OBJS = basic_client.o

basic_client: $(OBJS)
	$(CC) -o basic_client $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) basic_client

.PHONY: clean
