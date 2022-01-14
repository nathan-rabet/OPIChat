CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99 -D_GNU_SOURCE
LDFLAGS = -fsanitize=address

OBJS = epoll-server.o connection.o utils/xalloc.o

epoll_server_debug: $(OBJS)
	$(CC) -o epoll_server $(OBJS)

epoll_server: $(OBJS)
	$(CC) $(LDFLAGS) -o epoll_server $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) epoll_server

.PHONY: clean
