#include "semant/env.h"

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
	return new_type_entry;
}

EnvEntry make_array_type(Type element_type, size_t size) {
	EnvEntry new_array_entry = (EnvEntry)checked_malloc(sizeof(struct EnvEntry_));
	new_array_entry->kind = Array_Entry;
	new_array_entry->u.array_entry.element_type = element_type;
	new_array_entry->u.array_entry.size = size;

	return new_array_entry;
}

EnvEntry make_record_entry(TypeList fields) {
	EnvEntry new_record_entry = (EnvEntry)checked_malloc(sizeof(struct EnvEntry_));
	new_record_entry->kind = Record_Entry;
	new_record_entry->u.record_entry.fields = fields;

	return new_record_entry;
}

Symbol make_symbol(string name, EnvEntry environment_entry) {
	Symbol  new_symbol = checked_malloc(sizeof(struct Symbol_));
	new_symbol->name = strdup(name);
	new_symbol->environment_entry = environment_entry;
	new_symbol->next = NULL;

	return new_symbol;
}

Environment make_environment(size_t capacity, Env_Kind kind) {
	Environment new_environment = checked_malloc(sizeof(struct Environment_));
	new_environment->size = 0;
	new_environment->capacity = capacity;
	new_environment->symbols = calloc(capacity,  sizeof(Symbol));
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
	size_t hash_value = hash(environment->capacity, name);
	Symbol chosen_symbol = environment->symbols[hash_value];

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
	Environment new_environment = make_environment(new_capacity);
	new_environment->size = environment->size;
	for (size_t i = 0; i < environment->capacity; i++) {
		Symbol current_symbol = environment->symbols[i];
		while (current_symbol != NULL) {
			Symbol next = current_symbol->next;
			current_symbol->next = NULL;

			size_t new_hash = hash(new_capacity, current_symbol->name);
			Symbol selected_symbol = new_environment->symbols[new_hash];
			if (selected_symbol == NULL) {
				new_environment->symbols[new_hash] = current_symbol;
			}
			else {
				current_symbol->next = selected_symbol;
				new_environment->symbols[new_hash] = current_symbol;
			}
			
			current_symbol = next;
		}
	}

	return new_environment;
}


Environment insert_symbol(Environment environment, Symbol new_symbol) {
	double load_factor = (double) environment->size / environment->capacity;
	if (load_factor >= LOAD_FACTOR_THRESHOLD)
		environment = resize_environment(environment, environment->capacity * 2);

	size_t hash_value = hash(environment->capacity, name);
	Symbol chosen_symbol = environment->symbols[hash_value];
	
	environment->size++;

	if (chosen_symbol == NULL) {
		environment->symbols[hash_value] = new_symbol;
		return environment;
	}

	environment->next = chosen_symbol;
	environment->symbols[hash_value] = new_symbol;

	return environment;
}

Environment delete_symbol(Environment environment, string name) {
	size_t hash_value = hash(environment->capacity, name);
	Symbol chosen_symbol = environment->symbols[hash_value];

	if (chosen_symbol == NULL) 
		return environment;
	
	Symbol current_symbol = chosen_symbol;
	Symbol previous_symbol = current_symbol;

	while (current_symbol != NULL) {
		if (strcmp(current_symbol->name, name) == 0) {
			
			current_symbol = current_symbol->next;
			previous_symbol->next = current_symbol;

			environment->size--;

			break;
		}
		
		previous_symbol = current_symbol;
		current_symbol = current_symbol->next;
	}

	return environment;
}
