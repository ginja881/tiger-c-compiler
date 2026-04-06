CC = gcc
CFLAGS = -fPIC -g -Wall -Wno-unused-function -Wextra -Werror
LINKED_LIBRARIES = -lm 
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build


$(BUILD_DIR)/compiler: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(SRC_DIR)/lex/lex.yy.c $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c) $(SRC_DIR))/lex/tokens.c $(LINKED_LIBRARIES) $(SRC_DIR)/main.c -o $@


