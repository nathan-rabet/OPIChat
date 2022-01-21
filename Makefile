CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99 
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Iinclude

BUILD = build

LDFLAGS = -fsanitize=address

SRC = $(shell find src -name '*.c')
OBJS = $(SRC:%.c=$(BUILD)/%.o)

TEST_SRC = $(shell find tests/unit_testing -name '*.c')
TEST_OBJS = $(TEST_SRC:.c=.o)
TEST_LDFLAGS = -lcriterion

DYN_LIB = libmalloc.so

# OPIchat
all: opichat_server opichat_client
	
# Compile server and client
opichat_server: $(OBJS) src/server_main.o
	$(CC) $(LDFLAGS) -o opichat_server $^

opichat_client: $(OBJS) src/client_main.o
	$(CC) $(LDFLAGS) -o opichat_client $^

# Run test suite
check:
	$(MAKE) CFLAGS="$(CFLAGS) -DDEBUG" LDFLAGS="" tests
	./$(BUILD)/tests_suite

tests: $(TEST_OBJS) $(OBJS)
	$(CC) $(TEST_LDFLAGS) $(LDFLAGS) -o $(BUILD)/tests_suite $^

test_main: $(OBJS) tests/test_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BUILD)/test_main $^

# dynamic libraries
$(BUILD)/%.so: src/library/%.o $(BUILD)
	$(CC) -shared -fPIC -o $@ $<

# object files
$(BUILD)/%.o: %.c $(BUILD)
	mkdir -p $(dir $@)
	$(CC) $(ADD_COMPIL) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD)

.PHONY: clean check check_no_asan

# basics
basic_client:
	$(MAKE) -C basic_client

basic_server:
	$(MAKE) -C basic_client

epoll_server:
	$(MAKE) -C epoll_server
