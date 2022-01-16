CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99 -D_GNU_SOURCE
LDFLAGS = -fsanitize=address

SRC = $(shell find . -name '*.c')
OBJS = $(SRC:.c=.o)

TEST_SRC = $(shell find tests -name '*.c')
TEST_OBJS = $(TEST_SRC:.c=.o)
TEST_LDFLAGS = -fsanitize=address -lcriterion

# OPIchat
all: opichat_server opichat_client
	
opichat_server: $(OBJS) src/opichat_server.o
	$(CC) $(LDFLAGS) -o opichat_server $^

opichat_client: $(OBJS) src/opichat_client.o
	$(CC) $(LDFLAGS) -o opichat_client $^

check: tests
	./tests

tests: $(TEST_OBJS) $(OBJS)
	$(CC) $(TEST_LDFLAGS) -o tests $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) src/*.o

.PHONY: clean check

# basics
basic_client:
	$(MAKE) -C basic_client

basic_server:
	$(MAKE) -C basic_client

epoll_server:
	$(MAKE) -C epoll_server
