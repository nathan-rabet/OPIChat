RM=rm -rf

CLIENT_EXE=client_main
SERVER_EXE=server_main

CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Iinclude

BUILD = build


LDFLAGS = -fsanitize=address -pthread

SRC = $(shell find src -name '*.c' ! -name '$(CLIENT_EXE).c' ! -name '$(SERVER_EXE).c')
OBJS = $(SRC:%.c=$(BUILD)/%.o)

TEST_SRC = $(shell find tests/unit_testing -name '*.c')
TEST_OBJS = $(TEST_SRC:%.c=$(BUILD)/%.o)
TEST_LDFLAGS = -lcriterion

# OPIchat
all: opichat_server opichat_client
	
# Compile server and client
opichat_server: $(OBJS) $(BUILD)/src/server_main.o
	$(CC) $(LDFLAGS) -o opichat_server $^

opichat_client: $(OBJS) $(BUILD)/src/client_main.o
	$(CC) $(LDFLAGS) -o opichat_client $^

# Run test suite
check:
	$(MAKE) CFLAGS="$(CFLAGS) -DDEBUG" LDFLAGS="-pthread" tests
	./$(BUILD)/tests_suite

check_asan:
	$(MAKE) CFLAGS="$(CFLAGS) -DDEBUG" tests
	./$(BUILD)/tests_suite

tests: $(TEST_OBJS) $(OBJS)
	$(CC) $(TEST_LDFLAGS) $(LDFLAGS) -o $(BUILD)/tests_suite $^

test_main: $(OBJS) $(BUILD)/tests/test_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o test_main $^

# object files
$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(ADD_COMPIL) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD) opichat_client opichat_server test_main

.PHONY: clean check

# basics
basic_client:
	$(MAKE) -C basic_client

basic_server:
	$(MAKE) -C basic_client

epoll_server:
	$(MAKE) -C epoll_server
