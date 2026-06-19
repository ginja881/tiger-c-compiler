#include "semant/symbol.h"

EnvEntry make_function_entry(TypeList parameters, Type return_type) {
	EnvEntry new_function_entry = (EnvEntry)checked_malloc(sizeof(struct EnvEntry_));
	new_function_entry->kind = Function_Entry;
	new_function_entry->u.function_entry.parameters = parameters;
	new_function_entry->u.function_entry.return_type = return_type;

	return new_function_entry;
}

EnvEntry make_var_entry(Type raw_type) {
	EnvEntry new_var_entry = (EnvEntry)checked_malloc(sizeof(struct EnvEntry_));
	new_var_entry->kind = Var_Entry;
	new_var_entry->u.var_entry = raw_type;
	return new_var_entry;
}

EnvEntry make_escape_entry(void) {
	EnvEntry new_escape_entry = (EnvEntry)checked_malloc(sizeof(struct EnvEntry_));
	new_escape_entry->kind = Escape_Entry;
	new_escape_entry->escapes = false;
	return new_escape_entry;

}



Symbol make_symbol(string name, EnvEntry environment_entry) {
	Symbol  new_symbol = checked_malloc(sizeof(struct Symbol_));
	new_symbol->name = strdup(name);
	new_symbol->environment_entry = environment_entry;
	new_symbol->next = NULL;

	return new_symbol;
}
SymbolTable make_symbol_table(size_t capacity) {
	SymbolTable new_symbol_table = checked_malloc(sizeof(struct SymbolTable_));
	new_symbol_table->size = 0;
	new_symbol_table->capacity = capacity;
	new_symbol_table->symbols = calloc(capacity, sizeof(Symbol));
	
	return new_symbol_table;
}
Environment make_environment(size_t capacity, Env_Kind kind) {
	Environment new_environment = checked_malloc(sizeof(struct Environment_));
	
	new_environment->table = make_symbol_table(capacity); 
	new_environment->kind = kind;
	return new_environment;
}


size_t hash(size_t capacity, string key) {
	size_t hash_code = 0;
	for (size_t i = 0; i < strlen(key); i++)
		hash_code += (size_t)((int)key[i] * HASH_CONSTANT * i);
	return hash_code % capacity;
}

Symbol get_symbol(Environment environment, string name) {
	size_t hash_value = hash(environment->table->capacity, name);
	Symbol chosen_symbol = environment->table->symbols[hash_value];

	if (chosen_symbol ==  NULL)
		return NULL;

	Symbol current_symbol = chosen_symbol;
	while (current_symbol != NULL) {
		if (strcmp(current_symbol->name, name) == 0)
			return current_symbol;

		current_symbol = current_symbol->next;
	}

	return NULL;
}

Environment resize_environment(Environment environment, size_t new_capacity) {
	SymbolTable new_symbol_table = make_symbol_table(new_capacity);
	new_symbol_table->size = environment->table->size;

	for (size_t i = 0; i < environment->table->capacity; i++) {
		Symbol current_symbol = environment->table->symbols[i];
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
	environment->table = new_symbol_table;
	return environment;
}


Environment insert_symbol(Environment environment, Symbol new_symbol) {
	double load_factor = (double) environment->table->size / environment->table->capacity;
	if (load_factor >= LOAD_FACTOR_THRESHOLD)
		environment = resize_environment(environment, environment->table->capacity * 2);

	size_t hash_value = hash(environment->table->capacity, new_symbol->name);
	Symbol chosen_symbol = environment->table->symbols[hash_value];
	
	environment->table->size++;

	if (chosen_symbol == NULL) {
		environment->table->symbols[hash_value] = new_symbol;
		return environment;
	}

	new_symbol->next = chosen_symbol;
	environment->table->symbols[hash_value] = new_symbol;

	return environment;
}

Environment delete_symbol(Environment environment, string name) {
	size_t hash_value = hash(environment->table->capacity, name);
	Symbol chosen_symbol = environment->table->symbols[hash_value];

	if (chosen_symbol == NULL) 
		return environment;
	
	Symbol current_symbol = chosen_symbol;
	Symbol previous_symbol = current_symbol;

	while (current_symbol != NULL) {
		if (strcmp(current_symbol->name, name) == 0) {
			
			current_symbol = current_symbol->next;
			previous_symbol->next = current_symbol;

			environment->table->size--;

			break;
		} 
		
		previous_symbol = current_symbol;
		current_symbol = current_symbol->next;
	}

	return environment;
}
Symbol make_symbol_chain(Symbol symbol) {
	if (symbol == NULL)
		return NULL;
	
	Symbol new_symbol = NULL;
	Symbol header_symbol = NULL;
	Symbol current_symbol = symbol;

	while (current_symbol != NULL) {
		Symbol next_symbol = make_symbol(
			current_symbol->name,
			current_symbol->environment_entry
		);
		
		if (header_symbol == NULL) {
			header_symbol = next_symbol;
			new_symbol = header_symbol;
		}
		else {
			new_symbol->next = next_symbol;
			new_symbol = new_symbol->next;
		}
		current_symbol = current_symbol->next;
	}


	return header_symbol;
}
Environment clone_environment(Environment environment) {
	if (environment == NULL)
		return NULL;

	Environment clone_environment = make_environment(environment->table->capacity, environment->kind);
	clone_environment->table->size = environment->table->size;

	for (size_t i = 0; i < environment->table->capacity; i++) {
		Symbol current_symbol = environment->table->symbols[i];

		clone_environment->table->symbols[i] = make_symbol_chain(current_symbol);
	}
	
	return clone_environment;
}
