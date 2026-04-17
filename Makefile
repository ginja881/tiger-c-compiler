SHELL = /bin/bash
CC = gcc
LEXICAL_GENERATOR = lex
LEAK_CHECKER = valgrind
CFLAGS = -fPIC -g -Wall -Wno-unused-function -Wextra -Werror
LINKED_LIBRARIES = -lm 
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
LOG_DIR = logs

UTIL = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
LEXICAL_DEPENDENCIES = $(SRC_DIR)/lex/lex.yy.c $(SRC_DIR)/lex/tokens.c
OVERALL_DEPENDENCIES = $(LEXICAL_DEPENDENCIES)

$(BUILD_DIR)/compiler: $(SRC_DIR)/main.c 
	@if [ ! -d $(BUILD_DIR) ]; \
		then mkdir -p $(BUILD_DIR); \
	fi
	@echo Updating lexical specification.....
	@$(LEXICAL_GENERATOR) -o $(SRC_DIR)/lex/lex.yy.c $(SRC_DIR)/lex/tiger_lexer.lex
	@echo Done!!

	@$(CC) $(CFLAGS) -I$(SRC_DIR) $(UTIL) $(OVERALL_DEPENDENCIES) $(LINKED_LIBRARIES) $(SRC_DIR)/main.c -o $@
	@echo Build done! Executable at $@

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	@if [ ! -d $(BUILD_DIR)/$(TEST_DIR) ]; then \
		mkdir -p $(BUILD_DIR)/$(TEST_DIR); \
		mkdir -p $(BUILD_DIR); \
	fi;
	@$(CC) $(CFLAGS) -I$(SRC_DIR) $(UTIL) $(OVERALL_DEPENDENCIES) $(LINKED_LIBRARIES) $< -o $@
	@echo Build done! Test at $@

clean: 
	@if [ -d $(BUILD_DIR) ]; \
		then rm -r $(BUILD_DIR); \
	fi;
	@echo CLEANED

debug: $(BUILD_DIR)/compiler
	@valgrind --leak-check=ful --show-leak-kinds=all $<  $(TEST_DIR)/source_files/merge.tig
