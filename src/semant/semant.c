#include "semant/semant.h"

Environment make_standard_var_env(void) {
	Environment standard_var_env = make_environemnt(10, Var_Env);
	
	standard_var_env = insert(standard_var_env, 
		make_symbol("flush",
			make_function_entry(
				make_type_list(
					builtin_void_type
				)
			)
		)
	);
	string_symbol =  make_symbol("s", 
		make_var_entry(
			builtin_string_type
		)
	);
	int_symbol = make_symbol("i",
		make_var_entry(
			builtin_int_type
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("print",
			make_function_entry(
				make_type_list(
					make_field_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				builtin_void_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env, 
		make_symbol("getchar",
			make_function_entry(
				make_type_list(
					builtin_void_type,
					NULL
				),
				builtin_string_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("ord",
			make_function_entry(
				make_type_list(
					make_field_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				builtin_int_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("chr",
			make_function_entry(
				make_type_list(
					 make_field_type(
					 	int_symbol,
						int_symbol->environment_entry->u.var_entry
					 ),
					 NULL
				),
				builtin_char_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("size",
			make_function_entry(
				make_type_list(
					make_field_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				builtin_int_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("not",
			make_function_entry(
				make_type_list(
					make_field_type(
						int_symbol,
						int_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				builtin_int_type
			)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("exit",
			make_function_entry(
				make_type_list(
					make_field_type(
						int_symbol,
						int_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				builtin_void_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("concat",
			make_function_entry(
				make_type_list(
					make_field_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					make_type_list(
						make_field_type(
							string_symbol,
							string_symbol->environment_entry->u.var_entry
						),
						NULL
					)
				),
				builtin_string_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("substring",
			make_function_entry(
				make_type_list(
					make_field_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					make_type_list(
						make_field_type(
							int_symbol,
							int_symbol->environment_entry->u.var_entry
						),
						make_type_list(
							make_field_type(
								int_symbol,
								int_symbol->environment_entry->u.var_entry
							),
							NULL
						)
					)
				),
				builtin_string_type
			)
		)
	);

	return standard_var_env;

	
}

Environment make_standard_type_env(void) {
	standard_type_env = make_environment(DEFAULT_CAPACITY, Type_Env);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("string",
			builtin_string_type
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("int",
			builtin_int_type
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("integer",
			builtin_int_type
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("char",
			builtin_char_type
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("nil",
			builtin_nil_type
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("bool",
			builtin_boolean_type
		)
	);
	return standard_type_env;
}

SemanticAnalyzer begin_scope(SemanticAnalyzer sem, Environment var_env, Environment type_env) {

	Scope new_scope = (Scope)checked_malloc(sizeof(struct Scope_));
	new_scope->var_environment = var_env;
	new_scope->type_environment = type_env;
	new_scope->child = sem->scope_head;
	sem->scope_head = new_scope;

	return sem;
}

SemanticAnalyzer end_scope(SemanticAnalyzer sem) {
	assert(sem->scope_head);
	sem->scope_head = sem->scope_head->child;

	return sem;
}
Semanticanalyzer peek_scope(SemanticAnalyzer sem) {
	return sem->scope_head;
}

SemanticAnalyzer make_semantic_analyzer(Parser parser) {
	SemanticAnalyzer new_semantic_analyzer = (SemanticAnalyzer)checked_malloc(sizeof(struct SemanticAnalyzer_));
	new_semantic_analyzer->parser = parser;
	new_semantic_analyzer->scope_head = NULL;
	
	builtin_string_type = make_string_type();
	builtin_int_type = make_int_type();
	builtin_real_type = make_real_type();
	builtin_boolean_type = make_boolean_type();
	builtin_char_type = make_char_type();
	builtin_void_type = make_void_type();
	builtin_nil_type = make_nil_type();

	return new_semantic_analyzer;
}

void semantic_main(SemanticAnalyzer sem) {

	A_Stm compound_stm = sem->parser->root;
	sem = begin_scope(sem, make_standard_var_env(sem), make_standard_type_env(sem));

	while (compound_stm != NULL && compound_stm->kind == Compound_Stm) {
		A_Stm current_stm = compound_stm->u.compound_stm.left;
		if (current_stm->kind == Exp_Stm) 
			Tr_Exp exp = check_exp(current_stm->u.exp_stm->expression, sem);

	}
	
}
_
