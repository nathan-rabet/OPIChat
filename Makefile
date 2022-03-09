RM=rm -rf

CLIENT_EXE=client_main
SERVER_EXE=server_main

CC=gcc
CFLAGS = -Wextra -Wall -Werror -pedantic -g -std=c99
CFLAGS += -D_GNU_SOURCE
CFLAGS += -Iinclude

BUILD = build


LDFLAGS = -fsanitize=address -pthread

SRC = src/server/room.c src/server/commands/handle_send_room.c src/server/commands/handle_join_room.c src/server/commands/handle_create_room.c src/server/commands/handle_broadcast.c src/server/commands/handle_ping.c src/server/commands/handle_delete_room.c src/server/commands/handle_leave_room.c src/server/commands/handle_login.c src/server/commands/handle_list_rooms.c src/server/commands/handle_list_users.c src/server/commands/handle_send_dm.c src/server/commands/handle_profile.c src/server/client.c src/server/epoll/epoll_handler.c src/server/epoll/init_server_socket.c src/client/read_from_server.c src/client/read_from_stdin.c src/client/init_client_socket.c src/common/message/message_command_parameters.c src/common/message/message_parser.c src/common/message/message_serializer.c src/common/message/message_free.c src/common/message/message_init.c src/common/utils/type_check.c src/common/utils/xalloc.c src/common/utils/logger.c src/common/utils/my_itoa.c src/common/safe_io.c
OBJS = $(SRC:%.c=$(BUILD)/%.o)

TEST_SRC = tests/unit_testing/logger_test.c tests/unit_testing/room_test.c tests/unit_testing/epoll_test.c tests/unit_testing/message_serializer_test.c tests/unit_testing/message_parser_test.c tests/unit_testing/commands_test.c tests/unit_testing/read_from_stdin_test.c tests/unit_testing/client_test.c tests/unit_testing/safe_io_test.c
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
check: tests
	./$(BUILD)/tests_suite

check_asan:
	$(MAKE) CFLAGS="$(CFLAGS) -DDEBUG" tests
	./$(BUILD)/tests_suite

tests: CFLAGS += -DDEBUG
tests: LDFLAGS = "-pthread"
tests: $(TEST_OBJS) $(OBJS)
	$(CC) $(TEST_LDFLAGS) $(LDFLAGS) -o $(BUILD)/tests_suite $^

test_main: CFLAGS += -DDEBUG
test_main: $(OBJS) $(BUILD)/tests/test_main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o test_main $^

# object files
$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(ADD_COMPIL) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD) opichat_client opichat_server

.PHONY: clean check

# basics
basic_client:
	$(MAKE) -C basic_client

basic_server:
	$(MAKE) -C basic_client

epoll_server:
	$(MAKE) -C epoll_server
