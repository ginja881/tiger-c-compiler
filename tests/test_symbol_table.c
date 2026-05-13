


#include "util.h"
#include "lex/tokens.h"
#include "semant/symbol.h"

Lexer lexer;
int panic_mode;

int main(void) {
	char* keys = "abcdefghijklmnopqrstuvwxyz";
	size_t keys_length = strlen(keys);
	string strings[keys_length];

	SymbolTable symbol_table = make_symbol_table(DEFAULT_CAPACITY);
	for (size_t i = 0; i < keys_length; i++) {
		char key = keys[i];
		printf("\n Adding %c as key \n", key);
		
		char tmp[2] = {key, '\0'};
		string new_key = String(tmp);
		strings[i] = new_key;
		symbol_table = insert(symbol_table, new_key, new_key);
	}

	for (size_t j = 0; j < keys_length; j++) {
	        string key = strings[j];
		Symbol result = get(symbol_table, key);
		if (result == NULL) {
			fprintf(
				stderr,
				"(Test Failed) symbol table returned NULL for key %c",
				key[0]
			);
			_Exit(-1);
		}
		if (strcmp(result->value, key) != 0) {
			fprintf(
				stderr,
				"(Test Failed) symbol table did NOT return %c as value for key %c",
				result->value[0],
				key[0]
			);
		}

		symbol_table = delete(symbol_table, key);
		
	
	}
	printf("\nTest succeeded\n");
	free_symbol_table(symbol_table);
	
	return EXIT_SUCCESS;


}
