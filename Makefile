CC = gcc
CFLAGS = -fPIC -g -Wall -Wextra -Werror
LINKED_LIBRARIES = -lm -lfl
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build


$(BUILD_DIR)/compiler: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c) $(LINKED_LIBRARIES) -o $@


