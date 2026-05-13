#include "symbol.h"

Symbol make_symbol(string name, string value, Type type, SymbolKind symbol_kind) {
	Symbol  new_symbol = checked_malloc(sizeof(struct Symbol_));
	new_symbol->name = strdup(name);
	new_symbol->value = strdup(value);
	new_symbol->type = type;
	new_symbol->symbol_kind = symbol_kind;
	new_symbol->next = NULL;

	return new_symbol;
}

SymbolTable make_symbol_table(size_t capacity) {
	SymbolTable new_symbol_table = checked_malloc(sizeof(struct SymbolTable_));
	new_symbol_table->size = 0;
	new_symbol_table->capacity = capacity;
	new_symbol_table->symbols = calloc(capacity,  sizeof(Symbol));
	return new_symbol_table;
}

void free_symbol(Symbol symbol) {
	free((void*)symbol->name);
	free((void*)symbol->value);
	free((void*)symbol->type);
	free((void*)symbol);
}

void free_symbol_table(SymbolTable symbol_table) {
	if (symbol_table->size > 0) {
		for (size_t i = 0; i < symbol_table->capacity; i++) {
			Symbol current_symbol = symbol_table->symbols[i];
			if (current_symbol == NULL)
				continue;
		
			while (current_symbol != NULL) {
				Symbol old_symbol = current_symbol;
				current_symbol = current_symbol->next;
				free_symbol(old_symbol);
			}
		}
	}
	free((void*)symbol_table->symbols);
	free((void*)symbol_table);
}

size_t hash(size_t capacity, string key) {
	size_t hash_code = 0;
	for (size_t i = 0; i < strlen(key); i++)
		hash_code += (size_t)((int)key[i] * HASH_CONSTANT * i);
	return hash_code % capacity;
}

Symbol get(SymbolTable symbol_table, string name) {
	size_t hash_value = hash(symbol_table->capacity, name);
	Symbol chosen_symbol = symbol_table->symbols[hash_value];

	if (chosen_symbol ==  NULL) {
		return NULL;
	}
	Symbol current_symbol = chosen_symbol;
	while (current_symbol != NULL) {
		if (strcmp(current_symbol->name, name) == 0)
			return current_symbol;

		current_symbol = current_symbol->next;
	}

	return NULL;
}

SymbolTable resize(SymbolTable symbol_table, size_t new_capacity) {
	SymbolTable new_symbol_table = make_symbol_table(new_capacity);
	new_symbol_table->size = symbol_table->size;
	for (size_t i = 0; i < symbol_table->capacity; i++) {
		Symbol current_symbol = symbol_table->symbols[i];
		while (current_symbol != NULL) {
			Symbol next = current_symbol->next;
			current_symbol->next = NULL;

			size_t new_hash = hash(new_capacity, current_symbol->name);
			Symbol selected_symbol = new_symbol_table->symbols[new_hash];
			if (selected_symbol == NULL) {
				new_symbol_table->symbols[new_hash] = current_symbol;
			}
			else {
				current_symbol->next = selected_symbol;
				new_symbol_table->symbols[new_hash] = current_symbol;
			}
			
			current_symbol = next;
		}
	}

	return new_symbol_table;
}


SymbolTable insert(SymbolTable symbol_table, Symbol new_symbol) {
	double load_factor = (double) symbol_table->size / symbol_table->capacity;
	if (load_factor >= LOAD_FACTOR_THRESHOLD)
		symbol_table = resize(symbol_table, symbol_table->capacity * 2);

	size_t hash_value = hash(symbol_table->capacity, name);
	Symbol chosen_symbol = symbol_table->symbols[hash_value];
	
	symbol_table->size++;

	if (chosen_symbol == NULL) {
		symbol_table->symbols[hash_value] = new_symbol;
		return symbol_table;
	}

	new_symbol->next = chosen_symbol;
	symbol_table->symbols[hash_value] = new_symbol;

	return symbol_table;
}

SymbolTable delete(SymbolTable symbol_table, string name) {
	size_t hash_value = hash(symbol_table->capacity, name);
	Symbol chosen_symbol = symbol_table->symbols[hash_value];

	if (chosen_symbol == NULL) 
		return symbol_table;
	
	Symbol current_symbol = chosen_symbol;
	Symbol previous_symbol = current_symbol;

	while (current_symbol != NULL) {
		if (strcmp(current_symbol->name, name) == 0) {
			Symbol old_symbol = current_symbol;
			
			current_symbol = current_symbol->next;
			previous_symbol->next = current_symbol;
			free_symbol(old_symbol);

			symbol_table->size--;

			break;
		}
		
		previous_symbol = current_symbol;
		current_symbol = current_symbol->next;
	}

	return symbol_table;
}
