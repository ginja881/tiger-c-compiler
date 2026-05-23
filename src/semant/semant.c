#include "semant/semant.h"

Exp_Ty make_exp_ty(Tr_Exp main_exp, Type exp_type) {
    Exp_Ty new_exp;
    new_exp.main_exp = main_exp;
    new_exp.exp_type = exp_type;

    return new_exp;
}

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
Scope peek_scope(SemanticAnalyzer sem) {
	return sem->scope_head;
}

SemanticAnalyzer make_semantic_analyzer(Parser parser) {
	SemanticAnalyzer new_semantic_analyzer = (SemanticAnalyzer)checked_malloc(sizeof(struct SemanticAnalyzer_));
	new_semantic_analyzer->parser = parser;
	new_semantic_analyzer->scope_head = NULL;
	new_semantic_analyzer->checking_for_value = TRUE;

	builtin_string_type = make_string_type();
	builtin_int_type = make_int_type();
	builtin_real_type = make_real_type();
	builtin_boolean_type = make_boolean_type();
	builtin_char_type = make_char_type();
	builtin_void_type = make_void_type();
	builtin_nil_type = make_nil_type();
	
	return new_semantic_analyzer;
}

Exp_Ty check_literals(A_Exp literal, SemanticAnalyzer sem) {
	switch (literal->kind) {
		case Real_Exp: return make_exp_ty(NULL, builtin_real_type);
		case Num_Exp: return make_exp_ty(NULL, builtin_int_type);
		case Bool_Exp: return make_exp_ty(NULL, builtin_boolean_type);
		case Char_Exp; return make_exp_ty(NULL, builtin_char_type);
		case String_Exp: return  make_exp_ty(NULL, builtin_string_type);
		case NIL_Exp: return make_exp_ty(NULL, builtin_nil_type);
		case Array_Exp: {
			string type_id = literal->u.array_exp.type_id;
			A_Exp init = literal->u.array_exp.init;
			A_Exp size = literal->u.array_exp.size;
			if (get_symbol(sem->scope_head->type_environment, type_id) == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->pos->line_pos,
					literal->pos->col_pos,
					"UNDEFINED TYPE",
					panic_mode
				);
				return make_exp_ty(NULL, Void_Type);
			}
			if (size->kind != Num_Exp) {
				report_error(
					TypeError, 
					"(PLACEHOLDER)", 
					literal->pos->line_pos, 
					literal->pos->col_pos, 
					"SIZE OF ARRAY MUST BE INT",
					panic_mode
				);
				return make_exp_ty(NULL, builtin_void_type);
			}
			return make_exp_ty(NULL



		}
		default: return make_exp_ty(builtin_void_type);
	}
	return make_exp_ty(builtin_void_type);
}
Exp_Ty check_strict_op(A_Exp strict_op_exp, SemanticAnalyzer sem) {
	if (strict_op_exp->kind == Unary_Exp) {
		Exp_Ty operand_type = check_exp(strict_op_exp->u.unary_exp.exp, sem);
		if (match(operand_type, builtin_int_type) == TRUE || match(operand_type, builtin_real_type) == TRUE) {
			return make_exp_ty(NULL, operand_type);
		}
		else {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				strict_op_exp->pos->line_pos,
				strict_op_exp->pos->col_pos,
				"INVALID UNARY OPERAND",
				panic_mode
			);
			return make_exp_ty(NULL, builtin_void_type);
		}
	}
	else if (strict_op_exp->kind == Op_Exp) {
		A_Op operation = strict_op_exp->u.op_exp.op;
		Exp_Ty left = check_exp(strict_op_exp->u.op_exp.exp1, sem);
		Exp_Ty right = check_exp(strict_op_exp->u.op_exp.exp2, sem);

		if (operation == OP_MOD) {
			if (match(left, builtin_int_type) == FALSE)  {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->pos->line_pos,
					strict_op_exp->u.op_exp.exp1->pos->col_pos,
					"INVALID LEFT OPERAND (must be INT)",
					panic_mode
				);
				return make_exp_ty(NULL, builtin_void_type);
			}

			if (match(right, builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp2->pos->line_pos,
					strict_op_exp->u.op_exp.exp2->pos->col_pos,
					"INVALID OPERAND (must be INT)",
					panic_mode
				);
				return make_exp_ty(NULL, builtin_void_type);
			}

			return make_exp_ty(NULL, builtin_int_type);
		}
		else if (operation == OP_LSHIFT || operation == OP_AND || operation == OP_OR || operation == OP_RSHIFT) {
			if (match(left, builtin_int_type) == FALSE || match(left, builtin_real_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->op_exp.exp1->pos->line_pos,
					strict_op_exp->op_exp.exp1->pos->col_pos,
					"INVALID LEFT OPERAND (must be INT or REAL)",
					panic_mode
				);
				return make_exp_ty(NULL, builtin_void_type);
			}

			if (match(right, builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->op_exp.exp1->pos->line_pos,
					strict_op_exp->op_exp.exp1->pos->col_pos,
					"INVALID RIGHT OPERAND (must be only INT)",
					panic_mode
				);
				return make_exp_ty(NULL, builtin_void_type);
			}
			return make_exp_ty(NULL, left);
		}
	}

	return make_exp_ty(NULL, void_type);
}
Exp_Ty check_compar_op(A_Exp compar_op_expression, SemanticAnalyzer sem) {
	Exp_Ty left = check_exp(compar_op_expression->u.op_exp.exp1, sem);
	Exp_Ty right = check_exp(compar_op_expression->u.op_exp.exp2, sem);

	if (match(left, builtin_void_type) == TRUE || match(right, builtin_void_type) == TRUE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			compar_op_expression->pos->line_pos,
			compar_op_expression->pos->col_pos,
			"Must be a valid type",
			panic_mode
		);
		return make_exp_ty(NULL, builtin_void_type);
	}
	return make_exp_ty(NULL, builtin_boolean_type);
}
Exp_Ty check_exp(A_Exp expression, SemanticAnalyzer sem) {
	switch(expression->kind) {
		case Unary_Exp: {
			A_Op op_kind = expression->u.unary_exp.op;
			Op_Class operation_class = op_class(op_kind);

			if (operation_class == STRICT_OP)
				return check_strict_op(expression, sem);
			
		}
		case Op_Exp: {
			A_Op op_kind = expression->u.op_exp.op;
			Op_Class operation_class = op_class(op_kind);

			if (operation_class == OVERLOAD_OP)
				return check_strict_op(expression, sem);
			else if (operation_class == STRICT_OP)
				return check_overload_op(expression, sem);
			else if (operation_class == COMPAR_OP)
				return check_compar_op(expression, sem);

		}
		case Seq_Exp: {
			A_ExpList current_explist = expression->u.seq_exp.exp_list;
			while (current_explist->exp != NULL) {
				Exp_Ty current_exp_ty = check_exp(current_explist->exp, sem);
				current_explist = current_explist->next;
			}
			return make_exp_ty(NULL, builtin_void_type);
		}
		case If_Exp: {
			A_Exp current_exp  = expression;
			while (current_exp != NULL) {
				Exp_Ty current_cond = check_exp(current_exp->u.if_exp.cond, sem);
				if (match(current_cond->exp_type, builtin_boolean_type) == FALSE) {
					report_error(
						TypeError,
						"(PLACE HOLDER)",
						expression->pos->line_pos,
						expression->pos->col_pos,
						"Operand of type boolean must be IF conditional",
						panic_mode
					);
					break;
				}
				Exp_Ty current_block = check_exp(current_exp->u.if_exp.then, sem);

				current_exp = current_exp->u.if_exp.else_block;
			}
			return make_exp_ty(NULL, builtin_void_type);
		}
		case While_Exp: {
			Exp_Ty condition = check_exp(current_exp->u.while_exp.cond, sem);
			Exp_Ty block = check_exp(current_exp->u.while_exp.block, sem);

			if (match(condition->exp_type, builtin_boolean_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->pos->line_pos,
					expression->pos->col_pos,
					"Operand of type boolean must be WHILE conditional",
					panic_mode
				);
			}
			
			return make_exp_ty(NULL, builtin_void_type);
		}
		
		default:
			return check_literals(expression, sem);
	}
}

void semantic_main(SemanticAnalyzer sem) {

	A_Stm compound_stm = sem->parser->root;
	sem = begin_scope(sem, make_standard_var_env(sem), make_standard_type_env(sem));

	while (compound_stm != NULL && compound_stm->kind == Compound_Stm) {
		sem->checking_for_value = TRUE;

		A_Stm current_stm = compound_stm->u.compound_stm.left;
		if (current_stm->kind == Exp_Stm) 
			Exp_Ty exp = check_exp(current_stm->u.exp_stm->expression, sem);
	
	}
	
}
_
