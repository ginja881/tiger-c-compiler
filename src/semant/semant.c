#include "semant/semant.h"

Exp_Ty make_exp_ty(Tr_Exp main_exp, Type exp_type) {
    Exp_Ty new_exp = (Exp_Ty)checked_malloc(sizeof(struct Exp_Ty_));
    new_exp->main_exp = main_exp;
    new_exp->exp_type = exp_type;

    return new_exp;
}

Environment make_standard_var_env(SemanticAnalyzer sem) {
	Environment standard_var_env = make_environment(10, Var_Env);
	
	standard_var_env = insert_symbol(standard_var_env, 
		make_symbol("flush",
			make_function_entry(
				NULL,
				sem->builtin_void_type
			)
		)
	);
	Symbol string_symbol =  make_symbol("s", 
		make_var_entry(
			sem->builtin_string_type
		)
	);
	Symbol int_symbol = make_symbol("i",
		make_var_entry(
			sem->builtin_int_type
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("print",
			make_function_entry(
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				sem->builtin_void_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env, 
		make_symbol("getchar",
			make_function_entry(
				NULL,
				sem->builtin_string_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("ord",
			make_function_entry(
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				sem->builtin_int_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("chr",
			make_function_entry(
				make_type_list(
					 make_name_type(
					 	int_symbol,
						int_symbol->environment_entry->u.var_entry
					 ),
					 NULL
				),
				sem->builtin_string_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("size",
			make_function_entry(
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				sem->builtin_int_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("not",
			make_function_entry(
				make_type_list(
					make_name_type(
						int_symbol,
						int_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				sem->builtin_int_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("exit",
			make_function_entry(
				make_type_list(
					make_name_type(
						int_symbol,
						int_symbol->environment_entry->u.var_entry
					),
					NULL
				),
				sem->builtin_void_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("concat",
			make_function_entry(
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					make_type_list(
						make_name_type(
							string_symbol,
							string_symbol->environment_entry->u.var_entry
						),
						NULL
					)
				),
				sem->builtin_string_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("substring",
			make_function_entry(
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry
					),
					make_type_list(
						make_name_type(
							int_symbol,
							int_symbol->environment_entry->u.var_entry
						),
						make_type_list(
							make_name_type(
								int_symbol,
								int_symbol->environment_entry->u.var_entry
							),
							NULL
						)
					)
				),
				sem->builtin_string_type
			)
		)
	);

	return standard_var_env;

	
}

Environment make_standard_type_env(SemanticAnalyzer sem) {
	Environment standard_type_env = make_environment(DEFAULT_CAPACITY, Type_Env);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("string",
			make_var_entry(
				sem->builtin_string_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("int",
			make_var_entry(
				sem->builtin_int_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("integer",
			make_var_entry(
				sem->builtin_int_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("char",
			make_var_entry(
				sem->builtin_char_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("nil",
			make_var_entry(
				sem->builtin_nil_type
			)
		)
	);
	
	return standard_type_env;
}

SemanticAnalyzer begin_scope(SemanticAnalyzer sem) {
	if (sem->scope_head == NULL) {
		sem->scope_head = (Scope)checked_malloc(sizeof(struct Scope_));
		sem->scope_head->var_environment = make_standard_var_env(sem);
		sem->scope_head->type_environment = make_standard_type_env(sem);
		sem->scope_head->parent = NULL;
		return sem;
	}
	Scope new_scope = (Scope)checked_malloc(sizeof(struct Scope_));

	
	new_scope->var_environment = clone_environment(sem->scope_head->var_environment);
	new_scope->type_environment = clone_environment(sem->scope_head->type_environment);
	new_scope->parent = sem->scope_head;
	sem->scope_head = new_scope;

	return sem;
}

SemanticAnalyzer end_scope(SemanticAnalyzer sem) {
	assert(sem->scope_head);
	sem->scope_head = sem->scope_head->parent;

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

	new_semantic_analyzer->builtin_string_type = make_string_type();
	new_semantic_analyzer->builtin_int_type = make_int_type();
	new_semantic_analyzer->builtin_real_type = make_real_type();

	new_semantic_analyzer->builtin_char_type = make_char_type();
	new_semantic_analyzer->builtin_void_type = make_void_type();
	new_semantic_analyzer->builtin_nil_type = make_nil_type();
	new_semantic_analyzer->builtin_error_type = make_error_type();

	return new_semantic_analyzer;
}
Exp_Ty check_field_exp(A_Exp field, SemanticAnalyzer sem) {
	A_Field actual_field = field->u.field_exp.field;
	switch (actual_field->kind) {
		case Subscript_Field: {
			string id = actual_field->u.subscript_field.id;
			A_Exp location_exp = actual_field->u.subscript_field.loc;

			Symbol structure_symbol = get_symbol(sem->scope_head->var_environment, id);
			if (structure_symbol == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER",
					field->position->line_pos,
					field->position->col_pos,
					"UNDEFINED RECORD/ARRAY",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			if (structure_symbol->environment_entry->kind != Var_Entry) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					field->position->line_pos,
					field->position->col_pos,
					"UNDEFINED RECORD/ARRAY",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			Type environment_type = structure_symbol->environment_entry->u.var_entry;

			if (environment_type->kind == Array_Type) {
				
				Exp_Ty location = check_exp(location_exp, sem);

				if (location->exp_type->kind == Error_Type)
					return location;
				
				return make_exp_ty(NULL, environment_type->u.array_type);
			}
			return make_exp_ty(NULL, sem->builtin_error_type);
			
		}
		case Ty_Field: {
			string type_id = actual_field->u.ty_field.type;


			Symbol type_symbol = get_symbol(sem->scope_head->type_environment, type_id);
			if (type_symbol == NULL) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Type does not exist",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			return make_exp_ty(NULL, actual_type(type_symbol->environment_entry->u.var_entry));
		}
		case Record: {
			string id = actual_field->u.record_field.type_id;

			Symbol record_symbol = get_symbol(sem->scope_head->type_environment, id);

			if (record_symbol == NULL) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos, 
					"Record type does not exist",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			Type record_symbol_type = record_symbol->environment_entry->u.var_entry;
			if (record_symbol_type->kind != Record_Type) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Expected record type, but found something else",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			TypeList field_info = record_symbol_type->u.record_type.types;
			A_FieldList record_def_fields = actual_field->u.record_field.record_def_fields;
			TypeList new_header = NULL;
			TypeList current_type = NULL;
			int found = FALSE;
			while (record_def_fields != NULL) {
				A_Field current_field = record_def_fields->field;
				string id = current_field->u.item_field.id;

				Exp_Ty item = check_exp(current_field->u.item_field.value, sem);
				if (item->exp_type->kind == Error_Type)
					return item;

				TypeList field_info = record_symbol_type->u.record_type.types;
				 
				while (field_info != NULL) {
					Type field_type = field_info->type;
					if (strcmp(field_type->u.field_type.name, id) == 0) {
						if (field_type->kind == Error_Type)
							return make_exp_ty(NULL, field_type);

						if (match_types(field_type->u.field_type.type, item->exp_type) == FALSE) {
							report_error(
								TypeError,
								"(PLACEHOLDER)",
								current_field->position->line_pos,
								current_field->position->col_pos,
								"Type mismatch in record fields",
								panic_mode
							);
							return make_exp_ty(NULL, sem->builtin_error_type);
						}
						found = TRUE;
						break;
					}
					field_info = field_info->next;
				}

				if (found == TRUE) {
					if (new_header == NULL) {
						new_header = make_type_list(field_info->type, NULL);
						current_type = new_header;
					}
					else {
						current_type->next = make_type_list(field_info->type, NULL);
						current_type = current_type->next;
					}
				}

				record_def_fields = record_def_fields->next;
				found = FALSE;
			}

			if (new_header == NULL) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Incorrect definition",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			return make_exp_ty(NULL, make_record_type(new_header));
		}
		case Ref_Field: {
			A_Exp left_id = actual_field->u.ref_field.left_id;
			A_Exp right_id = actual_field->u.ref_field.right_id;

			Symbol record_symbol = get_symbol(sem->scope_head->var_environment, left_id->u.id_exp.identifier);
			if (record_symbol == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Undefined record",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			if (record_symbol->environment_entry->kind != Var_Entry) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Expected variable, but got functtion",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			Type record = actual_type(record_symbol->environment_entry->u.var_entry);
			if (record->kind != Record_Type) {
				report_error(
					SyntaxError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Expected record",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			TypeList fields = record->u.record_type.types;

			while (fields != NULL) {
				Type actual_field_type = fields->type;
				if (strcmp(actual_field_type->u.field_type.name, right_id->u.id_exp.identifier) == 0) 
					return make_exp_ty(NULL, actual_field_type->u.field_type.type);

				fields = fields->next;	
			}

			report_error(
				TypeError,
				"(PLACEHOLDER)",
				actual_field->position->line_pos,
				actual_field->position->col_pos,
				"Field does not exist in record",
				panic_mode
			);

			return make_exp_ty(NULL, sem->builtin_error_type);
		}
		default: return make_exp_ty(NULL, sem->builtin_error_type);
	}
	return make_exp_ty(NULL, sem->builtin_error_type);
}
Exp_Ty check_literals(A_Exp literal, SemanticAnalyzer sem) {
	switch (literal->kind) {
		case Real_Exp: return make_exp_ty(NULL, sem->builtin_real_type);
		case Num_Exp: return make_exp_ty(NULL, sem->builtin_int_type);
		case Bool_Exp: return make_exp_ty(NULL, sem->builtin_int_type);
		case Char_Exp: return make_exp_ty(NULL, sem->builtin_char_type);
		case String_Exp: return  make_exp_ty(NULL, sem->builtin_string_type);
		case NIL_Exp: return make_exp_ty(NULL, sem->builtin_nil_type);
		case ID_Exp: {
			string id = literal->u.id_exp.identifier;
			Symbol symbol = get_symbol(sem->scope_head->var_environment, id);
			if (symbol == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"UNDEFINED INSTANCE",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			if (symbol->environment_entry->kind != Var_Entry) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"UNEXPECTED INSTANCE THAT IS NOT VARIABLE",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			return make_exp_ty(NULL, actual_type(symbol->environment_entry->u.var_entry));
		}
		case Array_Exp: {
			string type_id = literal->u.array_exp.type_id;
			Exp_Ty init = check_exp(literal->u.array_exp.init, sem);
			Exp_Ty size = check_exp(literal->u.array_exp.size, sem);
			if (init->exp_type->kind == Error_Type || size->exp_type->kind == Error_Type)
				return make_exp_ty(NULL, sem->builtin_error_type);

			Symbol array_type_symbol = get_symbol(sem->scope_head->type_environment, type_id);
			

			if (array_type_symbol == NULL) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Unknown type",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			Type array_type = array_type_symbol->environment_entry->u.var_entry;

			if (match_types(array_type->u.array_type, init->exp_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Array type mismatch",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			if (match_types(size->exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Expected integer for array construction",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			

			return make_exp_ty(NULL, array_type);



		}
		case Field_Exp: {
			Exp_Ty field_exp_ty = check_field_exp(literal, sem);
			if (field_exp_ty->exp_type->kind == Error_Type)
				return make_exp_ty(NULL, sem->builtin_error_type);

			if (match_types(field_exp_ty->exp_type, sem->builtin_void_type) == TRUE) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"UNDEFINED FIELD",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			return field_exp_ty; 
		}
		case Callee_Exp: {
			string function_name = literal->u.callee_exp.id;
			Symbol function = get_symbol(sem->scope_head->var_environment, function_name);
			
			if (function == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Function does not exist",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			else if (function->environment_entry->kind == Var_Entry) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Variable found instead of function",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			

			A_ExpList argument = literal->u.callee_exp.args;
			TypeList parameter = function->environment_entry->u.function_entry.parameters;
			Type return_type = function->environment_entry->u.function_entry.return_type;
			int error_found = FALSE;
			while (TRUE) {
				if (argument == NULL || parameter == NULL)
					break;
				Exp_Ty arg_type = check_exp(argument->exp, sem);
				if (arg_type->exp_type->kind == Error_Type)
					error_found = TRUE;
				else if (match_types(actual_type(arg_type->exp_type), actual_type(parameter->type)) == FALSE) {
					printf("\n ARG TYPE: %d\n", actual_type(arg_type->exp_type)->kind);
					printf("\n PARAM TYPE: %d\n", actual_type(parameter->type)->kind);
					report_error(
						UnknownError,
						"(PLACEHOLDER)",
						argument->exp->position->line_pos,
						argument->exp->position->col_pos,
						"Mismatched argument type",
						panic_mode
					);
					error_found = TRUE;
				}
				argument = argument->next;
				parameter = parameter->next;
			}
			if (error_found == TRUE)
				return make_exp_ty(NULL,  sem->builtin_error_type);
			if (argument !=  NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Fewer arguments",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			if (parameter != NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Fewer parameters",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			return make_exp_ty(NULL, return_type);
		}
		default: return make_exp_ty(NULL, sem->builtin_error_type);
	}
	return make_exp_ty(NULL, sem->builtin_void_type);
}
Exp_Ty check_strict_op(A_Exp strict_op_exp, SemanticAnalyzer sem) {
	if (strict_op_exp->kind == Unary_Exp) {
		Exp_Ty operand_type = check_exp(strict_op_exp->u.unary_exp.exp, sem);
		if (match_types(operand_type->exp_type, sem->builtin_int_type) == TRUE || match_types(operand_type->exp_type, sem->builtin_real_type) == TRUE) {
			return make_exp_ty(NULL, actual_type(operand_type->exp_type));
		}
		else {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				strict_op_exp->position->line_pos,
				strict_op_exp->position->col_pos,
				"INVALID UNARY OPERAND",
				panic_mode
			);
			return make_exp_ty(NULL, sem->builtin_error_type);
		}
	}
	else if (strict_op_exp->kind == Op_Exp) {
		A_Op operation = strict_op_exp->u.op_exp.op;
		Exp_Ty left = check_exp(strict_op_exp->u.op_exp.exp1, sem);
		Exp_Ty right = check_exp(strict_op_exp->u.op_exp.exp2, sem);
		printf("\nLEFT TYPE %d\n", actual_type(left->exp_type)->kind);
		printf("\n RIGHT TYPE %d\n", actual_type(right->exp_type)->kind);

		if (operation == OP_MOD) {
			if (match_types(left->exp_type, sem->builtin_int_type) == FALSE)  {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->position->line_pos,
					strict_op_exp->u.op_exp.exp1->position->col_pos,
					"INVALID LEFT OPERAND (must be INT)",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			if (match_types(right->exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp2->position->line_pos,
					strict_op_exp->u.op_exp.exp2->position->col_pos,
					"INVALID OPERAND (must be INT)",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			return make_exp_ty(NULL, sem->builtin_int_type);
		}
		else if (operation == OP_LSHIFT || operation == OP_RSHIFT || operation == OP_AND || operation == OP_OR) {
			if (match_types(left->exp_type, sem->builtin_int_type) == FALSE && 
			match_types(left->exp_type, sem->builtin_real_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->position->line_pos,
					strict_op_exp->u.op_exp.exp1->position->col_pos,
					"INVALID LEFT OPERAND (must be INT or REAL)",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			if (match_types(right->exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->position->line_pos,
					strict_op_exp->u.op_exp.exp1->position->col_pos,
					"INVALID RIGHT OPERAND (must be only INT)",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			return make_exp_ty(NULL, actual_type(left->exp_type));
		}
		
	}

	return make_exp_ty(NULL, sem->builtin_error_type);
}
Exp_Ty check_compar_op(A_Exp compar_op_expression, SemanticAnalyzer sem) {
	Exp_Ty left = check_exp(compar_op_expression->u.op_exp.exp1, sem);
	Exp_Ty right = check_exp(compar_op_expression->u.op_exp.exp2, sem);
	printf("\n LEFT TYPE KIND: %d \n", left->exp_type->kind);
	printf("\n RIGHT TYPE KIND: %d \n", right->exp_type->kind);
	if (left->exp_type->kind == Error_Type || right->exp_type->kind == Error_Type) 
		return make_exp_ty(NULL, sem->builtin_error_type);
	
	if (match_types(left->exp_type, sem->builtin_void_type) == TRUE || match_types(right->exp_type, sem->builtin_void_type) == TRUE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			compar_op_expression->position->line_pos,
			compar_op_expression->position->col_pos,
			"Must be a valid type",
			panic_mode
		);
		return make_exp_ty(NULL, sem->builtin_error_type);
	}
	if (match_types(left->exp_type, right->exp_type) == FALSE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			compar_op_expression->position->line_pos,
			compar_op_expression->position->col_pos,
			"Type mismatch",
			panic_mode
		);
		return make_exp_ty(NULL, sem->builtin_error_type);
	}
	return make_exp_ty(NULL, sem->builtin_int_type);
}
Exp_Ty check_overload_op(A_Exp overload_op_exp, SemanticAnalyzer sem) {
	Exp_Ty left = check_exp(overload_op_exp->u.op_exp.exp1, sem);
	Exp_Ty right = check_exp(overload_op_exp->u.op_exp.exp2, sem);
	
	if (left->exp_type->kind == Error_Type || right->exp_type->kind ==  Error_Type)
		return make_exp_ty(NULL, sem->builtin_error_type);

	if (match_types(left->exp_type, sem->builtin_real_type) == FALSE &&
	match_types(left->exp_type, sem->builtin_int_type) == FALSE) {
		report_error(
			TypeError,
			"(PLACE HOLDER)",
			overload_op_exp->position->line_pos,
			overload_op_exp->position->col_pos,
			"Left operand must be either INT or real",
			panic_mode
		);
		return make_exp_ty(NULL, sem->builtin_error_type);
	}

	if (match_types(right->exp_type, sem->builtin_real_type) == FALSE &&
	match_types(right->exp_type, sem->builtin_int_type) == FALSE) {
		report_error(
			TypeError,
			"(PLACE HOLDER)",
			overload_op_exp->position->line_pos,
			overload_op_exp->position->col_pos,
			"Right operand must either be INT or real",
			panic_mode
		);
		return make_exp_ty(NULL, sem->builtin_error_type);
	}
	if (right->exp_type->kind == Real_Type || left->exp_type->kind == Real_Type)
		return make_exp_ty(NULL, sem->builtin_real_type);
	return make_exp_ty(NULL, sem->builtin_int_type);

}

Exp_Ty check_exp(A_Exp expression, SemanticAnalyzer sem) {
	if (expression == NULL)
		return make_exp_ty(NULL, sem->builtin_void_type);
	switch(expression->kind) {
		case Unary_Exp: {
			A_Op op_kind = expression->u.unary_exp.op;
			Op_Class operation_class = op_class(op_kind);
			Exp_Ty result = NULL;
			if (operation_class == STRICT_OP || op_kind == OP_SUB)
				result = check_strict_op(expression, sem);
			else
				result = make_exp_ty(NULL, sem->builtin_void_type);
			printf("\n Type checked unary operation\n");

			return result;
		}
		case Op_Exp: {
			A_Op op_kind = expression->u.op_exp.op;
			Op_Class operation_class = op_class(op_kind);
			Exp_Ty result = NULL;
			if (operation_class == OVERLOAD_OP)
				result = check_overload_op(expression, sem);
			else if (operation_class == STRICT_OP)
				result =  check_strict_op(expression, sem);
			else if (operation_class == COMPAR_OP)
				result = check_compar_op(expression, sem);
			else 
				result = make_exp_ty(NULL, sem->builtin_void_type);
			printf("\n Type checked binary operation\n");
			return result;
		}
		case Seq_Exp: {
			A_ExpList current_explist = expression->u.seq_exp.exp_list;
			if (current_explist == NULL)
				return make_exp_ty(NULL, sem->builtin_void_type);

			Exp_Ty current_exp_ty = NULL;
			while (current_explist->next != NULL) {
				current_exp_ty = check_exp(current_explist->exp, sem);
				current_explist = current_explist->next;
			}
			current_exp_ty = check_exp(current_explist->exp, sem);
			
			printf("\n TYPE CHECKED SEQ EXP\n");
			return current_exp_ty;
		}
		case If_Exp: {
			A_Exp current_exp  = expression;
			
			Exp_Ty current_cond = check_exp(current_exp->u.if_exp.cond, sem);

			if (current_cond->exp_type->kind == Error_Type)
				return make_exp_ty(NULL, sem->builtin_error_type);
			


			if (match_types(current_cond->exp_type, sem->builtin_int_type) == FALSE
			) {
					report_error(
						TypeError,
						"(PLACE HOLDER)",
						expression->position->line_pos,
						expression->position->col_pos,
						"Operand of type boolean must be IF conditional",
						panic_mode
					);
					return make_exp_ty(NULL, sem->builtin_error_type);
			}
			Exp_Ty then_block = check_exp(current_exp->u.if_exp.then, sem);
			if (current_exp->u.if_exp.else_block == NULL)
				return make_exp_ty(NULL, sem->builtin_void_type);


			Exp_Ty else_block = check_exp(current_exp->u.if_exp.else_block, sem);
			Type else_type = actual_type(else_block->exp_type);
			Type then_type = actual_type(then_block->exp_type);

			printf("\n THEN TYPE: %d\n", then_type->kind);
			printf("\n ELSE TYPE: %d\n", else_type->kind);
			
			if (else_type->kind == Error_Type || then_type->kind == Error_Type)
				return make_exp_ty(NULL, sem->builtin_error_type);
			if (match_types(else_type, then_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Type mismatch in IF expression",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}

			printf("\n TYPE CHECKED IF\n");
			return make_exp_ty(NULL, then_type);
		}
		case While_Exp: {
			Exp_Ty condition = check_exp(expression->u.while_exp.cond, sem);
			
			Exp_Ty block = check_exp(expression->u.while_exp.block, sem);
			if (condition->exp_type->kind == Error_Type || block->exp_type->kind == Error_Type)
				return make_exp_ty(NULL, sem->builtin_error_type);

			if (match_types(condition->exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Operand of type boolean must be WHILE conditional",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			if (match_types(block->exp_type, sem->builtin_void_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Block must be void",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			printf("\nTYPE CHECKED WHILE\n");
	
			return make_exp_ty(NULL, sem->builtin_void_type);
		}
		case Assign_Exp: {
			Exp_Ty id_type = check_exp(expression->u.assign_exp.identifier, sem);
			Exp_Ty value_type = check_exp(expression->u.assign_exp.val, sem);
			if (id_type->exp_type->kind == Error_Type || value_type->exp_type->kind == Error_Type)
				return make_exp_ty(NULL, sem->builtin_error_type);

			if (match_types(id_type->exp_type, value_type->exp_type) != TRUE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Type mismatch",
					panic_mode
				);
				return make_exp_ty(NULL, sem->builtin_error_type);
			}
			
			return make_exp_ty(NULL, sem->builtin_void_type);
		}
		case For_Exp: {
			string low_id = expression->u.for_exp.low_id->u.id_exp.identifier;
			Exp_Ty low_type = check_exp(expression->u.for_exp.low, sem);
			if (low_type->exp_type->kind == Error_Type)
				return low_type;

			Symbol low_symbol = get_symbol(sem->scope_head->var_environment, low_id);
			if (low_symbol == NULL) {
				low_symbol = make_symbol(low_id, make_var_entry(low_type->exp_type));
				sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, low_symbol);
			}
			else {
				if (low_symbol->environment_entry->kind == Function_Entry) {
					report_error(
						TypeError,
						"(PLACE HOLDER)",
						expression->position->line_pos,
						expression->position->col_pos,
						"Expected variable, but got function",
						panic_mode
					);
					return make_exp_ty(NULL, sem->builtin_error_type);
				}
				
				low_symbol->environment_entry->u.var_entry = low_type->exp_type;
			}
	
			Exp_Ty high_type = check_exp(expression->u.for_exp.high, sem);
			if (high_type->exp_type->kind == Error_Type)
				return high_type;

			Exp_Ty block_type = check_exp(expression->u.for_exp.block, sem);
			if (block_type->exp_type->kind == Error_Type)
				return block_type;

			return make_exp_ty(NULL, sem->builtin_void_type);
		}
		case Let_Exp: {
			printf("\n FOUND LET EXP\n");
			sem = begin_scope(sem);
			
			A_DecList current_declaration = expression->u.let_exp.declarations;

			while (current_declaration != NULL) {
				sem = precheck_decs(current_declaration->dec, sem);
				current_declaration = current_declaration->next;
			}

			current_declaration = expression->u.let_exp.declarations;


			while (current_declaration != NULL) {
				A_Dec declaration = current_declaration->dec;
				if (declaration->kind == Type_Dec) {
					string id = declaration->u.type_dec.name;
					Symbol current_type_symbol = get_symbol(sem->scope_head->type_environment, id);
					current_type_symbol->environment_entry->u.var_entry = handle_type_def(current_type_symbol, declaration, sem);
				}
				current_declaration = current_declaration->next;
			}
			current_declaration = expression->u.let_exp.declarations;
			while (current_declaration != NULL) {
				A_Dec declaration = current_declaration->dec;
				if (declaration->kind == Type_Dec) {
					string id = declaration->u.type_dec.name;
					Symbol symbol = get_symbol(sem->scope_head->type_environment, id);

					
					resolve_type(symbol->environment_entry->u.var_entry, sem, declaration->position);
				}
				current_declaration = current_declaration->next;
			}
			current_declaration = expression->u.let_exp.declarations;
			
			while (current_declaration != NULL) {
				A_Dec declaration = current_declaration->dec;
				if (declaration->kind == Simple_Var_Dec) {
					sem = handle_simple_variable(declaration, sem);
				}
				else if (declaration->kind == Field_Var_Dec) {
					sem = handle_field_variable(declaration, sem);
				}
				else if (declaration->kind == Func_Dec) {
					sem = handle_function(declaration, sem);
				}
				current_declaration = current_declaration->next;
			}
			current_declaration = expression->u.let_exp.declarations;
			while (current_declaration != NULL) {
				if (current_declaration->dec->kind == Func_Dec) {
					sem = process_body(current_declaration->dec, sem);
				}
				current_declaration = current_declaration->next;
			}
			Exp_Ty result = check_exp(expression->u.let_exp.block, sem);
			sem = end_scope(sem);
			printf("\n Finished semantic analysis at let environment: %p\n", expression);
			return result;
			
		}
		default:
			return check_literals(expression, sem);
	}
	return make_exp_ty(NULL, sem->builtin_error_type);
}
SemanticAnalyzer process_body(A_Dec declaration, SemanticAnalyzer sem) {
	Symbol function_symbol = get_symbol(sem->scope_head->var_environment, declaration->u.func_dec.name);
	
	if (function_symbol == NULL) {
		report_error(
			UnknownError,
			"(PLACEHOLDER)",
			declaration->position->line_pos,
			declaration->position->col_pos,
			"Type does not exist",
			panic_mode
		);
		return sem;
	}

	if (function_symbol->environment_entry->kind == Var_Entry) {
		report_error(
			UnknownError,
			"(PLACEHOLDER)",
			declaration->position->line_pos,
			declaration->position->col_pos,
			"Expected function, but got variable",
			panic_mode
		);
		return sem;
	}
	
	string return_type_name = declaration->u.func_dec.type;
	Type return_type = sem->builtin_void_type;
	if (return_type_name[0] != '\0') {
		Symbol return_symbol = get_symbol(sem->scope_head->type_environment, return_type_name);

		if (return_symbol == NULL) {
			report_error(
				UnknownError,
				"(PLACEHOLDER)",
				declaration->position->line_pos,
				declaration->position->col_pos,
				"Expected type",
				panic_mode
			);
		}
		else
			return_type = return_symbol->environment_entry->u.var_entry;
	}
	
	function_symbol->environment_entry->u.function_entry.return_type = return_type;

	sem = begin_scope(sem);
	TypeList parameters = function_symbol->environment_entry->u.function_entry.parameters;
	
	while (parameters != NULL) {
		Type parameter = parameters->type;
		
		
		Symbol arg_symbol = make_symbol(
			parameter->u.field_type.name,
			make_var_entry(parameter->u.field_type.type)
		);
		sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, arg_symbol);
		printf("\nINSERTED SYMBOL %s with TYPE %d\n", arg_symbol->name, arg_symbol->environment_entry->u.var_entry->kind);
		parameters = parameters->next;
	}
	
	Exp_Ty block = check_exp(declaration->u.func_dec.block, sem);
	if (match_types(block->exp_type, actual_type(function_symbol->environment_entry->u.function_entry.return_type)) == FALSE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			declaration->position->line_pos,
			declaration->position->col_pos,
			"Type mismatch with function return type",
			panic_mode
		);
	}
	sem = end_scope(sem);
	return sem;
}
SemanticAnalyzer handle_simple_variable(A_Dec declaration, SemanticAnalyzer sem) {
	Symbol simple_var_symbol = get_symbol(sem->scope_head->var_environment, declaration->u.simple_var_dec.id);



	Exp_Ty result = check_exp(declaration->u.simple_var_dec.val, sem);
	simple_var_symbol->environment_entry->u.var_entry = result->exp_type;

	return sem;
}
SemanticAnalyzer handle_field_variable(A_Dec declaration, SemanticAnalyzer sem) {
	A_Field dec_field = declaration->u.field_var_dec.field;
	Symbol field_symbol = get_symbol(sem->scope_head->var_environment, dec_field->u.ty_field.id);
	
	
	
	Type field_res = NULL;

	string type_id = dec_field->u.ty_field.type;

	Symbol type_symbol = get_symbol(sem->scope_head->type_environment, type_id);

	if (type_symbol == NULL) {
		report_error(
			TypeError,
			"(PLACE HOLDER)",
			declaration->position->line_pos, 
			declaration->position->col_pos,
			"Type does not exist",
			panic_mode
		);
		field_res = sem->builtin_error_type;
	}
	else 
		field_res = type_symbol->environment_entry->u.var_entry;

	Exp_Ty value_result = check_exp(declaration->u.field_var_dec.val, sem);
	if (match_types(actual_type(field_res), actual_type(value_result->exp_type)) == FALSE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			declaration->position->line_pos,
			declaration->position->col_pos,
			"Mismatched types",
			panic_mode
		);
		field_symbol->environment_entry->u.var_entry = sem->builtin_error_type;
		return sem;
	}

	field_symbol->environment_entry->u.var_entry = field_res;
	return sem;
}
SemanticAnalyzer handle_function(A_Dec declaration, SemanticAnalyzer sem) {
	Symbol function_symbol = get_symbol(sem->scope_head->var_environment, declaration->u.func_dec.name);
	TypeList header_parameters = NULL;
	TypeList parameters = NULL; 
	A_FieldList args = declaration->u.func_dec.args;
	

	while (args != NULL) {
		
		Type actual_result_type = sem->builtin_error_type;

		string arg_type_id = args->field->u.ty_field.type;
		string id = args->field->u.ty_field.id;

		Symbol arg_type_symbol = get_symbol(sem->scope_head->type_environment, arg_type_id);

		if (arg_type_symbol == NULL) {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				declaration->position->line_pos,
				declaration->position->col_pos,
				"Type does not exist",
				panic_mode
			);
		}
		else {
			actual_result_type = make_field_type(id, arg_type_symbol->environment_entry->u.var_entry);
			
		}
		Symbol argument_symbol = make_symbol(args->field->u.ty_field.id, make_var_entry(actual_result_type));
		


		if (header_parameters == NULL) {
			
			header_parameters = make_type_list(actual_result_type, NULL);
			parameters = header_parameters;
		}
		else {
			parameters->next = make_type_list(actual_result_type, NULL);
			parameters = parameters->next;
		}

		args = args->next;
	}
	function_symbol->environment_entry->u.function_entry.parameters = header_parameters;
	
	if (declaration->u.func_dec.type[0] != '\0') {
		Symbol return_symbol = get_symbol(sem->scope_head->type_environment, declaration->u.func_dec.type);

		if (return_symbol == NULL) {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				declaration->position->line_pos,
				declaration->position->col_pos,
				"Unknown Type",
				panic_mode
			);
			function_symbol->environment_entry->u.function_entry.return_type = sem->builtin_error_type;

		}
		else 
			function_symbol->environment_entry->u.function_entry.return_type = return_symbol->environment_entry->u.var_entry;

	}
	else
		function_symbol->environment_entry->u.function_entry.return_type = sem->builtin_void_type;
	Type return_type = function_symbol->environment_entry->u.function_entry.return_type;
	resolve_type(return_type, sem, declaration->position);
	
	return sem;
}
Type handle_type_def(Symbol current_symbol, A_Dec current_declaration, SemanticAnalyzer sem) {
	Environment type_environment = sem->scope_head->type_environment;

	A_Field current_dec_field = current_declaration->u.type_dec.def_type_field;
	Symbol resulting_symbol = NULL;
	Type result = NULL;
	if (current_dec_field->kind == Array_Field) {
		resulting_symbol = get_symbol(type_environment, current_dec_field->u.array_field.type_id);
		if (resulting_symbol == NULL) {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				current_declaration->position->line_pos,
				current_declaration->position->col_pos,
				"Type does not exist",
				panic_mode
			);
			return sem->builtin_error_type;
		}
		result = make_array_type(resulting_symbol->environment_entry->u.var_entry);
	}
	else if (current_dec_field->kind == Ty_Field) {
		resulting_symbol = get_symbol(type_environment, current_dec_field->u.ty_field.type);
		if (resulting_symbol == NULL) {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				current_declaration->position->line_pos,
				current_declaration->position->col_pos,
				"Type does not exist",
				panic_mode

			);
			return sem->builtin_error_type;
		}
		result = make_name_type(current_symbol, resulting_symbol->environment_entry->u.var_entry);
	}
	else if (current_dec_field->kind == Ty_Record) {
		A_FieldList current_field = current_dec_field->u.type_record_field.record_type_fields;
		TypeList header = NULL;
		TypeList type_list = NULL;
		while (current_field != NULL) {
			A_Field ty_field = current_field->field;
			Symbol resulting_symbol = get_symbol(type_environment, ty_field->u.ty_field.type);
			if (resulting_symbol == NULL) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					current_declaration->position->line_pos,
					current_declaration->position->col_pos,
					"Type does not exist",
					panic_mode
				);
				return sem->builtin_error_type;
			}
			Type field_type = make_field_type(
				ty_field->u.ty_field.id,
				resulting_symbol->environment_entry->u.var_entry
			);
			if (header == NULL)  {
				header = make_type_list(field_type, NULL);
				type_list = header;
			}
			else  {
				type_list->next = make_type_list(field_type, NULL);
				type_list = type_list->next;
			}
			current_field = current_field->next;

		}

		result = make_record_type(header);
		TypeList field = result->u.record_type.types;
		while (field != NULL) {
			
			if (field->type->u.field_type.type->kind == Error_Type)
				field->type->u.field_type.type = result;

			field = field->next;
		}
		


	}

	return result;
}
void resolve_type(Type result_type, SemanticAnalyzer sem, A_Pos position) {
	Environment type_environment = sem->scope_head->type_environment;
	if (result_type == NULL)
		return;

	if (result_type->state == Resolved || result_type->kind == Array_Type || result_type->kind == Record_Type)
		return;
	if (result_type->state == Visiting) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			position->line_pos,
			position->col_pos,
			"Type is cyclic",
			panic_mode
		);
		return;
	}
	result_type->state = Visiting;
	if (result_type->kind == Name_Type) {

		resolve_type(result_type->u.name_type.type, sem, position);
	}

	result_type->state = Resolved;
	
	
}

SemanticAnalyzer precheck_decs(A_Dec declaration, SemanticAnalyzer sem) {
	
	switch(declaration->kind) {
		case Type_Dec: {
			A_Field dec_field = declaration->u.type_dec.def_type_field;

			string id = declaration->u.type_dec.name;
			if (get_symbol(sem->scope_head->type_environment, id) != NULL) 
			{
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					declaration->position->line_pos,
					declaration->position->col_pos,
					"Type already exists",
					panic_mode
				);
				return sem;
			}

			Symbol type_header = make_symbol(id, make_var_entry(sem->builtin_error_type));
			
			sem->scope_head->type_environment = insert_symbol(sem->scope_head->type_environment, type_header);
			break;
		}
		case Func_Dec: {
			string id = declaration->u.func_dec.name;
			if (get_symbol(sem->scope_head->var_environment, id) != NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					declaration->position->line_pos,
					declaration->position->col_pos,
					"Name taken by function/variable",
					panic_mode
				);
				return sem;
			}

			Symbol function_header = make_symbol(id, make_function_entry(NULL, NULL));
			sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, function_header);
			break;
		}
		case  Simple_Var_Dec: {
			string id = declaration->u.simple_var_dec.id;
			if (get_symbol(sem->scope_head->var_environment, id) != NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					declaration->position->line_pos,
					declaration->position->col_pos,
					"Name taken by function/variable",
					panic_mode
				);
				return sem;
			}
			Symbol simple_var_header = make_symbol(id, make_var_entry(sem->builtin_error_type));
			sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, simple_var_header);
			break;
		}
		case Field_Var_Dec: {
			A_Field field = declaration->u.field_var_dec.field;
			if (field->kind == Ty_Field) {
				string id = field->u.ty_field.id;
				if (get_symbol(sem->scope_head->var_environment, id) !=  NULL) {
					report_error(
						UnknownError,
						"(PLACEHOLDER)",
						declaration->position->line_pos,
						declaration->position->col_pos,
						"Name taken by function/variable",
						panic_mode
					);
					return sem;
				}
				Symbol field_var_header = make_symbol(id, make_var_entry(sem->builtin_error_type));
				sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, field_var_header);
			}
			break;
		}
	}
	return sem;
}

void process_statement(A_Stm stm, SemanticAnalyzer sem) {
	if (stm == NULL)
		return;
	switch (stm->kind) {
		case Compound_Stm: {
			printf("\n FOUND COMPOUND\n");
			process_statement(stm->u.compound_stm.stm1, sem);
			process_statement(stm->u.compound_stm.stm2, sem);
			break;
		}
		case Exp_Stm: {
			printf("\n FOUND EXP\n");
			check_exp(stm->u.exp_stm.expression, sem);
			break;
		}
		case Decl_Stm: {
			printf("\n FOUND DEC\n");
			break;
		}
	}
}
void semantic_main(SemanticAnalyzer sem) {
	A_Stm compound_stm = sem->parser->root;
	if (compound_stm == NULL)
		printf("\n Fuck\n");
	
	sem = begin_scope(sem);
	printf("\n MADE STANDARD ENV\n");
	
	process_statement(compound_stm, sem);
	
}

