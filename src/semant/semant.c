#include "semant/semant.h"

TreeIR make_tree_ir_exp(Tr_Exp exp_ir, Type exp_type) {
    TreeIR new_exp = (TreeIR)checked_malloc(sizeof(struct TreeIR_));
    new_exp->kind = TreeExp;
    new_exp->u.expression.exp_ir = exp_ir;
    new_exp->u.expression.exp_type = exp_type;

    return new_exp;
}

TreeIR make_tree_ir_compound(TreeIR exp1, TreeIR exp2) {
	TreeIR new_compound = (TreeIR)checked_malloc(sizeof(struct TreeIR_));
	new_compound->kind = TreeCompound;
	new_compound->u.compound.exp1 = exp1;
	new_compound->u.compound.exp2 = exp2;

	return new_compound;
}

Environment make_standard_var_env(SemanticAnalyzer sem) {
	Environment standard_var_env = make_environment(10, Var_Env);
	Tr_Level outermost_level = sem->outermost_level;

	standard_var_env = insert_symbol(standard_var_env, 
		make_symbol("flush",
			make_function_entry(
				outermost_level,
				make_new_temporary_namedlabel("flush"),
				NULL,
				sem->builtin_void_type
			)
		)
	);
	Symbol string_symbol =  make_symbol("s", 
		make_var_entry(
			NULL,
			sem->builtin_string_type
		)
	);
	Symbol int_symbol = make_symbol("i",
		make_var_entry(
			NULL,
			sem->builtin_int_type
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("print",
			make_function_entry(
				outermost_level,
				make_new_temporary_namedlabel("print"),
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("getchar"),
				NULL,
				sem->builtin_string_type
			)
		)
	);

	standard_var_env = insert_symbol(standard_var_env,
		make_symbol("ord",
			make_function_entry(
				outermost_level,
				make_new_temporary_namedlabel("ord"),
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("chr"),
				make_type_list(
					 make_name_type(
					 	int_symbol,
						int_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("size"),
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("not"),
				make_type_list(
					make_name_type(
						int_symbol,
						int_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("exit"),
				make_type_list(
					make_name_type(
						int_symbol,
						int_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("concat"),
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry.variable_type
					),
					make_type_list(
						make_name_type(
							string_symbol,
							string_symbol->environment_entry->u.var_entry.variable_type
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
				outermost_level,
				make_new_temporary_namedlabel("substring"),
				make_type_list(
					make_name_type(
						string_symbol,
						string_symbol->environment_entry->u.var_entry.variable_type
					),
					make_type_list(
						make_name_type(
							int_symbol,
							int_symbol->environment_entry->u.var_entry.variable_type
						),
						make_type_list(
							make_name_type(
								int_symbol,
								int_symbol->environment_entry->u.var_entry.variable_type
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
				NULL,
				sem->builtin_string_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("int",
			make_var_entry(
				NULL,
				sem->builtin_int_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("integer",
			make_var_entry(
				NULL,
				sem->builtin_int_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("char",
			make_var_entry(
				NULL,
				sem->builtin_char_type
			)
		)
	);
	standard_type_env = insert_symbol(standard_type_env,
		make_symbol("nil",
			make_var_entry(
				NULL,
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
		sem->scope_head->escape_environment = make_environment(DEFAULT_CAPACITY, Escape_Env);
		sem->scope_head->current_level = sem->outermost_level;
		sem->scope_head->parent = NULL;
		return sem;
	}
	

	Scope new_scope = (Scope)checked_malloc(sizeof(struct Scope_));

	new_scope->escape_environment = clone_environment(sem->scope_head->escape_environment);	
	new_scope->var_environment = clone_environment(sem->scope_head->var_environment);
	new_scope->type_environment = clone_environment(sem->scope_head->type_environment);
	new_scope->parent = sem->scope_head;
	new_scope->current_level = sem->scope_head->current_level;

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
	
	new_semantic_analyzer->builtin_char_type = make_char_type();
	new_semantic_analyzer->builtin_void_type = make_void_type();
	new_semantic_analyzer->builtin_nil_type = make_nil_type();
	new_semantic_analyzer->builtin_error_type = make_error_type();
	new_semantic_analyzer->outermost_level = Tr_outermost();
	new_semantic_analyzer->ir_root = NULL;

	return new_semantic_analyzer;
}
TreeIR check_field_exp(A_Exp field, SemanticAnalyzer sem) {
	A_Field actual_field = field->u.field_exp.field;
	Scope current_scope = peek_scope(sem);
	switch (actual_field->kind) {
		case Subscript_Field: {
			string id = actual_field->u.subscript_field.id;
			A_Exp location_exp = actual_field->u.subscript_field.loc;

			Symbol structure_symbol = get_symbol(current_scope->var_environment, id);
			if (structure_symbol == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER",
					field->position->line_pos,
					field->position->col_pos,
					"UNDEFINED ARRAY",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			if (structure_symbol->environment_entry->kind != Var_Entry) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					field->position->line_pos,
					field->position->col_pos,
					"UNDEFINED ARRAY",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			
			Type environment_type = actual_type(structure_symbol->environment_entry->u.var_entry.variable_type);
			

			if (environment_type->kind == Array_Type) {
				
				TreeIR location = check_exp(location_exp, sem);
				
				if (location->u.expression.exp_type->kind == Error_Type)
					return location;
				
				Type element_type = actual_type(environment_type->u.array_type);
				int element_size = type_cost(element_type);
				Tr_Level current_level = current_scope->current_level;
				Tr_Access variable_access = structure_symbol->environment_entry->u.var_entry.access;

				return make_tree_ir_exp(
					Tr_translateArrayAccess(
						Tr_translateVariableAccess(
							variable_access,
							current_level,
							type_cost(environment_type)
						),
						location->u.expression.exp_ir,
						element_size
					), 
					environment_type->u.array_type
				);
			}

			
			return make_tree_ir_exp(
				NULL,
				sem->builtin_error_type
			);
			
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			Type actual_field_type = actual_type(type_symbol->environment_entry->u.var_entry.variable_type);

			return make_tree_ir_exp(NULL, actual_field_type);
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			Type record_symbol_type = actual_type(record_symbol->environment_entry->u.var_entry.variable_type);
			if (record_symbol_type->kind != Record_Type) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Expected record type, but found something else",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			TypeList field_info = record_symbol_type->u.record_type.types;
			A_FieldList record_def_fields = actual_field->u.record_field.record_def_fields;
			TypeList new_header = NULL;
			TypeList current_type = NULL;
			int found = FALSE;
			int field_pos = 0;
			IRList itemlist = NULL;
			IRList itemtail = NULL;
			while (record_def_fields != NULL) {
				A_Field current_field = record_def_fields->field;
				string id = current_field->u.item_field.id;

				TreeIR item = check_exp(current_field->u.item_field.value, sem);
				if (item->u.expression.exp_type->kind == Error_Type)
					return item;

				
				while (field_info != NULL) {
					Type field_type = field_info->type;
					if (strcmp(field_type->u.field_type.name, id) == 0) {
						if (field_type->kind == Error_Type)
							return make_tree_ir_exp(NULL, field_type);

						if (match_types(field_type->u.field_type.type, item->u.expression.exp_type) == FALSE) {
							report_error(
								TypeError,
								"(PLACEHOLDER)",
								current_field->position->line_pos,
								current_field->position->col_pos,
								"Type mismatch in record fields",
								panic_mode
							);
							return make_tree_ir_exp(NULL, sem->builtin_error_type);
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
					
					append_node(Tr_makeIRList(item->u.expression.exp_ir, NULL), &(itemlist), &(itemtail));
					field_pos++;
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			return make_tree_ir_exp(Tr_translateRecord(itemlist, field_pos + 1), make_record_type(new_header));
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			Type record = actual_type(record_symbol->environment_entry->u.var_entry.variable_type);
			if (record->kind != Record_Type) {
				report_error(
					SyntaxError,
					"(PLACEHOLDER)",
					actual_field->position->line_pos,
					actual_field->position->col_pos,
					"Expected record",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			TypeList fields = record->u.record_type.types;
			Tr_Exp record_access = Tr_translateVariableAccess(
				record_symbol->environment_entry->u.var_entry.access,
				current_scope->current_level,
				type_cost(record)
			);

			int field_pos = 0;
			while (fields != NULL) {
				Type actual_field_type = fields->type;
				if (strcmp(actual_field_type->u.field_type.name, right_id->u.id_exp.identifier) == 0) 
					return make_tree_ir_exp(
						Tr_translateFieldReference(record_access, field_pos), 
						actual_field_type->u.field_type.type
					);
				field_pos++;
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

			return make_tree_ir_exp(NULL, sem->builtin_error_type);
		}
		default: return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}
	return make_tree_ir_exp(NULL, sem->builtin_error_type);
}
TreeIR check_literals(A_Exp literal, SemanticAnalyzer sem) {
	Scope current_scope = peek_scope(sem);
	switch (literal->kind) {
		case Num_Exp: return make_tree_ir_exp(Tr_translateInteger(literal->u.num_exp.value), sem->builtin_int_type);
		case Bool_Exp: return make_tree_ir_exp(Tr_translateInteger(literal->u.bool_exp.boolean), sem->builtin_int_type);
		case Char_Exp: return make_tree_ir_exp(Tr_translateCharacter(literal->u.char_exp.character), sem->builtin_char_type);
		case String_Exp: return  make_tree_ir_exp(Tr_translateString(literal->u.string_exp.text), sem->builtin_string_type);
		case NIL_Exp: return make_tree_ir_exp(NULL, sem->builtin_nil_type);
		case ID_Exp: {
			string id = literal->u.id_exp.identifier;
			
			Symbol symbol = get_symbol(current_scope->var_environment, id);
			if (symbol == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"UNDEFINED INSTANCE",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			Type actual_variable_type = actual_type(symbol->environment_entry->u.var_entry.variable_type);

			return make_tree_ir_exp(
				Tr_translateVariableAccess(
					symbol->environment_entry->u.var_entry.access, 
					current_scope->current_level,
					type_cost(actual_variable_type)
				), 
				actual_variable_type
			);
		}
		case Array_Exp: {
			string type_id = literal->u.array_exp.type_id;
			TreeIR init = check_exp(literal->u.array_exp.init, sem);
			TreeIR size = check_exp(literal->u.array_exp.size, sem);
			if (init == NULL || size == NULL)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);

			if (init->u.expression.exp_type->kind == Error_Type || 
			size->u.expression.exp_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);

			Symbol array_type_symbol = get_symbol(current_scope->type_environment, type_id);
			

			if (array_type_symbol == NULL) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Unknown type",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			Type array_type = array_type_symbol->environment_entry->u.var_entry.variable_type;

			if (match_types(array_type->u.array_type, init->u.expression.exp_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Array type mismatch",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			if (match_types(size->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Expected integer for array construction",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			

			return make_tree_ir_exp(
				Tr_translateArray(init->u.expression.exp_ir, 
				type_cost(array_type), size->u.expression.exp_ir), 
				array_type
			);



		}
		case Field_Exp: {
			TreeIR field_exp_ir = check_field_exp(literal, sem);
			if (field_exp_ir == NULL)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);

			if (field_exp_ir->u.expression.exp_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);

			if (match_types(field_exp_ir->u.expression.exp_type, sem->builtin_void_type) == TRUE) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"UNDEFINED FIELD",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			return field_exp_ir; 
		}
		case Callee_Exp: {
			string function_name = literal->u.callee_exp.id;
			Symbol function = get_symbol(current_scope->var_environment, function_name);
			
			if (function == NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Function does not exist",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			

			A_ExpList argument = literal->u.callee_exp.args;
			TypeList parameter = function->environment_entry->u.function_entry.parameters;
			Type return_type = function->environment_entry->u.function_entry.return_type;
			int error_found = FALSE;

			IRList IR_arguments_head = NULL;
			IRList IR_arguments_tail = NULL;

			TempLabel function_label = function->environment_entry->u.function_entry.label;
			Tr_Level function_level = function->environment_entry->u.function_entry.level;

			while (TRUE) {
				if (argument == NULL || parameter == NULL)
					break;
				TreeIR arg_ir = check_exp(argument->exp, sem);
				if (arg_ir == NULL || arg_ir->u.expression.exp_type->kind == Error_Type)
					error_found = TRUE;
				else if (match_types(actual_type(arg_ir->u.expression.exp_type), actual_type(parameter->type)) == FALSE) {
					printf("\n ARG TYPE: %d\n", actual_type(arg_ir->u.expression.exp_type)->kind);
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
				
				append_node(
					Tr_makeIRList(arg_ir->u.expression.exp_ir, NULL),
					&(IR_arguments_head),
					&(IR_arguments_tail)
				);

				argument = argument->next;
				parameter = parameter->next;
			}
			if (error_found == TRUE)
				return make_tree_ir_exp(NULL,  sem->builtin_error_type);
			if (argument !=  NULL) {
				report_error(
					UnknownError,
					"(PLACEHOLDER)",
					literal->position->line_pos,
					literal->position->col_pos,
					"Fewer arguments",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
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
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			return make_tree_ir_exp(
				Tr_translateCallee(function_label, current_scope->current_level, function_level, IR_arguments_head), 
				return_type
			);
		}
		default: return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}
	// ?
	return make_tree_ir_exp(NULL, sem->builtin_error_type);
}
TreeIR check_strict_op(A_Exp strict_op_exp, SemanticAnalyzer sem) {
	if (strict_op_exp->kind == Unary_Exp) {
		TreeIR operand_type = check_exp(strict_op_exp->u.unary_exp.exp, sem);
		if (match_types(operand_type->u.expression.exp_type, sem->builtin_int_type) == TRUE) {
			return make_tree_ir_exp(NULL, actual_type(operand_type->u.expression.exp_type));
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
			return make_tree_ir_exp(NULL, sem->builtin_error_type);
		}
	}
	else if (strict_op_exp->kind == Op_Exp) {
		A_Op operation = strict_op_exp->u.op_exp.op;
		TreeIR left = check_exp(strict_op_exp->u.op_exp.exp1, sem);
		TreeIR right = check_exp(strict_op_exp->u.op_exp.exp2, sem);
		printf("\nLEFT TYPE %d\n", actual_type(left->u.expression.exp_type)->kind);
		printf("\n RIGHT TYPE %d\n", actual_type(right->u.expression.exp_type)->kind);

		if (operation == OP_MOD) {
			if (match_types(left->u.expression.exp_type, sem->builtin_int_type) == FALSE)  {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->position->line_pos,
					strict_op_exp->u.op_exp.exp1->position->col_pos,
					"INVALID LEFT OPERAND (must be INT)",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			if (match_types(right->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp2->position->line_pos,
					strict_op_exp->u.op_exp.exp2->position->col_pos,
					"INVALID OPERAND (must be INT)",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			return make_tree_ir_exp(Tr_translateBasicOperation(left->u.expression.exp_ir, right->u.expression.exp_ir, operation), sem->builtin_int_type);
		}
		else if (operation == OP_LSHIFT || operation == OP_RSHIFT || operation == OP_AND || operation == OP_OR) {
			if (match_types(left->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->position->line_pos,
					strict_op_exp->u.op_exp.exp1->position->col_pos,
					"INVALID LEFT OPERAND (must be INT)",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			if (match_types(right->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACEHOLDER)",
					strict_op_exp->u.op_exp.exp1->position->line_pos,
					strict_op_exp->u.op_exp.exp1->position->col_pos,
					"INVALID RIGHT OPERAND (must be only INT)",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			return make_tree_ir_exp(Tr_translateBasicOperation(left->u.expression.exp_ir, right->u.expression.exp_ir, operation), actual_type(left->u.expression.exp_type));
		}
		
	}

	return make_tree_ir_exp(NULL, sem->builtin_error_type);
}
TreeIR check_compar_op(A_Exp compar_op_expression, SemanticAnalyzer sem) {
	TreeIR left = check_exp(compar_op_expression->u.op_exp.exp1, sem);
	TreeIR right = check_exp(compar_op_expression->u.op_exp.exp2, sem);
	A_Op op = compar_op_expression->u.op_exp.op;

	printf("\n LEFT TYPE KIND: %d \n", left->u.expression.exp_type->kind);
	printf("\n RIGHT TYPE KIND: %d \n", right->u.expression.exp_type->kind);
	if (left->u.expression.exp_type->kind == Error_Type || right->u.expression.exp_type->kind == Error_Type) 
		return make_tree_ir_exp(NULL, sem->builtin_error_type);
	
	if (match_types(left->u.expression.exp_type, sem->builtin_void_type) == TRUE || 
	match_types(right->u.expression.exp_type, sem->builtin_void_type) == TRUE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			compar_op_expression->position->line_pos,
			compar_op_expression->position->col_pos,
			"Must be a valid type",
			panic_mode
		);
		return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}
	if (match_types(left->u.expression.exp_type, right->u.expression.exp_type) == FALSE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			compar_op_expression->position->line_pos,
			compar_op_expression->position->col_pos,
			"Type mismatch",
			panic_mode
		);
		return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}
	return make_tree_ir_exp(Tr_translateComparison(left->u.expression.exp_ir, op, right->u.expression.exp_ir) ,sem->builtin_int_type);
}
TreeIR check_overload_op(A_Exp overload_op_exp, SemanticAnalyzer sem) {
	TreeIR left = check_exp(overload_op_exp->u.op_exp.exp1, sem);
	TreeIR right = check_exp(overload_op_exp->u.op_exp.exp2, sem);
	A_Op operation = overload_op_exp->u.op_exp.op;

	if (left->u.expression.exp_type->kind == Error_Type || 
	right->u.expression.exp_type->kind ==  Error_Type)
		return make_tree_ir_exp(NULL, sem->builtin_error_type);

	if (match_types(left->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
		report_error(
			TypeError,
			"(PLACE HOLDER)",
			overload_op_exp->position->line_pos,
			overload_op_exp->position->col_pos,
			"Left operand must be either INT or real",
			panic_mode
		);
		return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}

	if (match_types(right->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
		report_error(
			TypeError,
			"(PLACE HOLDER)",
			overload_op_exp->position->line_pos,
			overload_op_exp->position->col_pos,
			"Right operand must either be INT or real",
			panic_mode
		);
		return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}

	return make_tree_ir_exp(
		Tr_translateBasicOperation(left->u.expression.exp_ir, right->u.expression.exp_ir, operation), 
		sem->builtin_int_type
	);

}

TreeIR check_exp(A_Exp expression, SemanticAnalyzer sem) {
	if (expression == NULL)
		return make_tree_ir_exp(NULL, sem->builtin_void_type);
	switch(expression->kind) {
		case Unary_Exp: {
			A_Op op_kind = expression->u.unary_exp.op;
			Op_Class operation_class = op_class(op_kind);
			TreeIR result = NULL;
			if (operation_class == STRICT_OP || op_kind == OP_SUB)
				result = check_strict_op(expression, sem);
			else
				result = make_tree_ir_exp(NULL, sem->builtin_void_type);
			printf("\n Type checked unary operation\n");

			return result;
		}
		case Op_Exp: {
			A_Op op_kind = expression->u.op_exp.op;
			Op_Class operation_class = op_class(op_kind);
			TreeIR result = NULL;
			if (operation_class == OVERLOAD_OP)
				result = check_overload_op(expression, sem);
			else if (operation_class == STRICT_OP)
				result =  check_strict_op(expression, sem);
			else if (operation_class == COMPAR_OP)
				result = check_compar_op(expression, sem);
			else 
				result = make_tree_ir_exp(NULL, sem->builtin_void_type);
			printf("\n Type checked binary operation\n");
			return result;
		}
		case Seq_Exp: {
			A_ExpList current_explist = expression->u.seq_exp.exp_list;
			if (current_explist == NULL)
				return make_tree_ir_exp(NULL, sem->builtin_void_type);

			TreeIR current_exp_ir = NULL;
			IRList head = NULL;
			IRList tail = NULL;

			ExpLoc old_value = sem->currently_checking_in;
			Type sequence_type = NULL;
			while (current_explist != NULL) {
				sem->currently_checking_in = Block;
				current_exp_ir = check_exp(current_explist->exp, sem);
				if (current_exp_ir->u.expression.exp_type->kind == Error_Type)
					return make_tree_ir_exp(NULL, sem->builtin_error_type);

				sequence_type = current_exp_ir->u.expression.exp_type;

				append_node(
					Tr_makeIRList(current_exp_ir->u.expression.exp_ir, NULL),
					&(head),
					&(tail)
				);
				current_explist = current_explist->next;
			}
			sem->currently_checking_in = old_value;
			
			printf("\n TYPE CHECKED SEQ EXP\n");
			return make_tree_ir_exp(Tr_lowerSequence(head), sequence_type); 
		}
		case If_Exp: {
		
			A_Exp current_exp  = expression;
			
			TreeIR current_cond = check_exp(current_exp->u.if_exp.cond, sem);
			
			if (current_cond->u.expression.exp_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			


			if (match_types(current_cond->u.expression.exp_type, sem->builtin_int_type) == FALSE
			) {
					report_error(
						TypeError,
						"(PLACE HOLDER)",
						expression->position->line_pos,
						expression->position->col_pos,
						"Operand of type boolean must be IF conditional",
						panic_mode
					);
					return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
		
			sem->currently_checking_in = Block;
			TreeIR then_block = check_exp(current_exp->u.if_exp.then, sem);
			sem->currently_checking_in = If;
			
			Type then_type = actual_type(then_block->u.expression.exp_type);

			if (current_exp->u.if_exp.else_block == NULL) {
				return make_tree_ir_exp(
					Tr_translateIfStatement(
						current_cond->u.expression.exp_ir,
						then_block->u.expression.exp_ir,
						NULL
					), 
					then_type
				);
			}

		
			sem->currently_checking_in = Block;
			TreeIR else_block = check_exp(current_exp->u.if_exp.else_block, sem);
			sem->currently_checking_in = If;

			Type else_type = actual_type(else_block->u.expression.exp_type);
		

			printf("\n THEN TYPE: %d\n", then_type->kind);
			printf("\n ELSE TYPE: %d\n", else_type->kind);
			
			if (else_type->kind == Error_Type || then_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			if (match_types(else_type, then_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Type mismatch in IF expression",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

			printf("\n TYPE CHECKED IF\n");

			return make_tree_ir_exp(Tr_translateIfStatement(
					current_cond->u.expression.exp_ir, 
					then_block->u.expression.exp_ir, 
					else_block->u.expression.exp_ir
				), 
				then_type
			);
		}
		case Break_Exp: {
			if (sem->currently_checking_in != While && sem->currently_checking_in != For) {
				report_error(
					SyntaxError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"BREAK NOT IN FOR OR WHILE LOOP",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			return make_tree_ir_exp(Tr_translateBreak(), sem->builtin_void_type);
		}
		case Continue_Exp: {
			if (sem->currently_checking_in != While && sem->currently_checking_in != For) {
				report_error(
					SyntaxError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"CONTINUE NOT IN FOR OR WHILE LOOP",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			return make_tree_ir_exp(Tr_translateContinue(), sem->builtin_void_type);
		}
		case While_Exp: {
			TreeIR condition = check_exp(expression->u.while_exp.cond, sem);
			
			ExpLoc old_val = sem->currently_checking_in;
			sem->currently_checking_in = While;
			TreeIR block = check_exp(expression->u.while_exp.block, sem);
			sem->currently_checking_in = old_val;

			if (condition->u.expression.exp_type->kind == Error_Type || block->u.expression.exp_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);

			if (match_types(condition->u.expression.exp_type, sem->builtin_int_type) == FALSE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Operand of type boolean must be WHILE conditional",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}

		
			
			if (block->u.expression.exp_type->kind == Error_Type || 
				match_types(block->u.expression.exp_type, sem->builtin_void_type) == FALSE) {
					report_error(
						TypeError,
						"(PLACEHOLDER)",
						expression->position->line_pos,
						expression->position->col_pos,
						"Block must be void",
						panic_mode
					);
					return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
				
			printf("\nTYPE CHECKED WHILE\n");
	
			return make_tree_ir_exp(
				Tr_translateWhileLoop(
					condition->u.expression.exp_ir, 
					block->u.expression.exp_ir
				), 
				sem->builtin_void_type
			);
		}
		case Assign_Exp: {
			TreeIR id_ir = check_exp(expression->u.assign_exp.identifier, sem);
			TreeIR value_ir = check_exp(expression->u.assign_exp.val, sem);
			if (id_ir->u.expression.exp_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			if (value_ir->u.expression.exp_type->kind == Error_Type)
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			
			if (match_types(id_ir->u.expression.exp_type, value_ir->u.expression.exp_type) != TRUE) {
				report_error(
					TypeError,
					"(PLACE HOLDER)",
					expression->position->line_pos,
					expression->position->col_pos,
					"Type mismatch",
					panic_mode
				);
				return make_tree_ir_exp(NULL, sem->builtin_error_type);
			}
			
			return make_tree_ir_exp(Tr_translateAssignment(id_ir->u.expression.exp_ir, value_ir->u.expression.exp_ir), sem->builtin_void_type);
		}
		case For_Exp: {
			string low_id = expression->u.for_exp.low_id->u.id_exp.identifier;
			TreeIR low_ir = check_exp(expression->u.for_exp.low, sem);
			if (low_ir->u.expression.exp_type->kind == Error_Type)
				return low_ir;

			Symbol low_symbol = get_symbol(sem->scope_head->var_environment, low_id);
			Tr_Access low_access = NULL;
			if (low_symbol == NULL) {
				bool escape = true;
				Symbol escape_symbol = get_symbol(sem->scope_head->escape_environment, low_id);
				if (escape_symbol != NULL)
					escape = escape_symbol->environment_entry->u.escape_entry.escapes;

				low_access = Tr_allocLocal(sem->scope_head->current_level, escape);
				low_symbol = make_symbol(low_id, make_var_entry(low_access, low_ir->u.expression.exp_type));
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
					return make_tree_ir_exp(NULL, sem->builtin_error_type);
				}
				low_access = low_symbol->environment_entry->u.var_entry.access;
				low_symbol->environment_entry->u.var_entry.variable_type = low_ir->u.expression.exp_type;
			}
			Tr_Exp low_id_ir = Tr_translateVariableAccess(low_access, sem->scope_head->current_level, type_cost(low_ir->u.expression.exp_type));


			TreeIR high_ir = check_exp(expression->u.for_exp.high, sem);
			if (high_ir->u.expression.exp_type->kind == Error_Type)
				return high_ir;
			
			ExpLoc old_val = sem->currently_checking_in;
			sem->currently_checking_in = For;
			TreeIR block_ir = check_exp(expression->u.for_exp.block, sem);
			sem->currently_checking_in = old_val;

			if (block_ir->u.expression.exp_type->kind == Error_Type)
				return block_ir;

			return make_tree_ir_exp(
				Tr_translateForLoop(
					low_id_ir, 
					low_ir->u.expression.exp_ir,
					high_ir->u.expression.exp_ir,
					block_ir->u.expression.exp_ir
				), 
				sem->builtin_void_type
			);
		}
		case Let_Exp: {
			printf("\n FOUND LET EXP\n");
			sem = begin_scope(sem);
			sem->scope_head->escape_environment = analyze_escapes(
				expression, 
				sem->scope_head->escape_environment, 
				0
			);
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
					current_type_symbol->environment_entry->u.var_entry.variable_type = handle_type_def(
						current_type_symbol, 
						declaration, 
						sem
					);
				
				}
				current_declaration = current_declaration->next;
			}
			current_declaration = expression->u.let_exp.declarations;
			while (current_declaration != NULL) {
				A_Dec declaration = current_declaration->dec;
				if (declaration->kind == Type_Dec) {
					string id = declaration->u.type_dec.name;
					Symbol symbol = get_symbol(sem->scope_head->type_environment, id);

					
					resolve_type(symbol->environment_entry->u.var_entry.variable_type, sem, declaration->position);
				}
				current_declaration = current_declaration->next;
			}
			current_declaration = expression->u.let_exp.declarations;
			
			
			// IR Translation integration for variable declarations
			IRList ir_head = NULL;
			IRList ir_tail = NULL; 

			while (current_declaration != NULL) {
				A_Dec declaration = current_declaration->dec;
				if (declaration->kind == Simple_Var_Dec) {
					TreeIR variable_dec = handle_simple_variable(declaration, sem);
					append_node(
						Tr_makeIRList(variable_dec->u.expression.exp_ir, NULL), 
						&(ir_head),
						&(ir_tail)
					);
				}
				else if (declaration->kind == Field_Var_Dec) {
					TreeIR variable_dec = handle_field_variable(declaration, sem);
					append_node(
						Tr_makeIRList(variable_dec->u.expression.exp_ir, NULL),
						&(ir_head),
						&(ir_tail)
					);
				}
				else if (declaration->kind == Func_Dec) {
					handle_function(declaration, sem);
				}
				current_declaration = current_declaration->next;
			}

			// IR Translation for code fragments
			current_declaration = expression->u.let_exp.declarations;
			while (current_declaration != NULL) {
				if (current_declaration->dec->kind == Func_Dec) 
					sem = process_body(current_declaration->dec, sem);
				
				current_declaration = current_declaration->next;
			}

			ExpLoc old_val = sem->currently_checking_in;
			sem->currently_checking_in = Let;
			TreeIR result = check_exp(expression->u.let_exp.block, sem);
			sem->currently_checking_in = old_val;

			sem = end_scope(sem);
			printf("\n Finished semantic analysis at let environment: %p\n", expression);
			return result;
			
		}
		default:
			return check_literals(expression, sem);
	}
	return make_tree_ir_exp(NULL, sem->builtin_error_type);
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
			return_type = return_symbol->environment_entry->u.var_entry.variable_type;
	}
	
	function_symbol->environment_entry->u.function_entry.return_type = return_type;

	sem = begin_scope(sem);
	TypeList parameters = function_symbol->environment_entry->u.function_entry.parameters;
	
	// Frame analysis stuff
	
	Tr_Level new_level = NULL;
	BoolList escapees_head = NULL;
	BoolList current_escapee = NULL;
	while (parameters != NULL) {
		Type parameter = parameters->type;
		string parameter_name = parameter->u.field_type.name;

		
		
	
		
		bool escape = true;
		Symbol escape_symbol = get_symbol(
			sem->scope_head->escape_environment,
			parameter_name
		);

		if (escape_symbol != NULL) {
			escape = escape_symbol->environment_entry->u.escape_entry.escapes;
			BoolList new_node = new_boollist(escape, NULL);

			if (escapees_head == NULL) {
				escapees_head = new_node;
				current_escapee = escapees_head;
			}
			else {
				current_escapee->next = new_node;
				current_escapee = current_escapee->next;
			}
		}

		parameters = parameters->next;
	}
	Tr_Level parent = sem->scope_head->current_level;
	function_symbol->environment_entry->u.function_entry.label = make_new_temporary_namedlabel(
		function_symbol->name
	);

	new_level = Tr_new_level(parent, function_symbol->environment_entry->u.function_entry.label, escapees_head);
	function_symbol->environment_entry->u.function_entry.level = new_level;

	sem->scope_head->current_level = new_level;
	
	Tr_AccessList tr_parameters = Tr_params(new_level);
	parameters = function_symbol->environment_entry->u.function_entry.parameters;
	
	while (parameters != NULL && tr_parameters != NULL) {
		Type parameter = parameters->type;
		string parameter_name = parameter->u.field_type.name;
		Type parameter_type = parameters->type;
		Tr_Access param_access = tr_parameters->tree_access;

		Symbol arg_symbol = make_symbol(
			parameter_name,
			make_var_entry(param_access, parameter_type->u.field_type.type)
		);

		sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, arg_symbol);
		parameters = parameters->next;
		tr_parameters = tr_parameters->next;
	}

	TreeIR block = check_exp(declaration->u.func_dec.block, sem);
	Type actual_function_type = actual_type(function_symbol->environment_entry->u.function_entry.return_type);

	if (actual_function_type->kind != Error_Type) {
		Type actual_block_type = actual_type(block->u.expression.exp_type);
		if (match_types(actual_block_type, actual_function_type) == FALSE) {
			report_error(
				TypeError,
				"(PLACEHOLDER)",
				declaration->position->line_pos,
				declaration->position->col_pos,
				"Type mismatch with function return type",
				panic_mode
			);
		}
	}
	Tr_makeFunction(new_level, block->u.expression.exp_ir, tr_parameters);

	sem = end_scope(sem);
	return sem;
}
TreeIR handle_simple_variable(A_Dec declaration, SemanticAnalyzer sem) {
	Symbol simple_var_symbol = get_symbol(sem->scope_head->var_environment, declaration->u.simple_var_dec.id);
	Symbol simple_var_escape_symbol = get_symbol(sem->scope_head->escape_environment, declaration->u.simple_var_dec.id);
	bool escape = true;
	if (simple_var_escape_symbol != NULL)
		escape = simple_var_escape_symbol->environment_entry->u.escape_entry.escapes;
	
	Tr_Access simple_access = Tr_allocLocal(sem->scope_head->current_level, escape);
	

	TreeIR result = check_exp(declaration->u.simple_var_dec.val, sem);
	simple_var_symbol->environment_entry->u.var_entry.variable_type = result->u.expression.exp_type;
	simple_var_symbol->environment_entry->u.var_entry.access = simple_access;
	
	Type simple_var_type = actual_type(simple_var_symbol->environment_entry->u.var_entry.variable_type);
	return make_tree_ir_exp(Tr_translateAssignment(
				Tr_translateVariableAccess(
					simple_var_symbol->environment_entry->u.var_entry.access, 
					sem->scope_head->current_level,
					type_cost(simple_var_type)
				),
				result->u.expression.exp_ir
			),
			sem->builtin_void_type
		);
}
TreeIR handle_field_variable(A_Dec declaration, SemanticAnalyzer sem) {
	A_Field dec_field = declaration->u.field_var_dec.field;
	Symbol field_symbol = get_symbol(sem->scope_head->var_environment, dec_field->u.ty_field.id);
	Symbol field_escape_symbol = get_symbol(sem->scope_head->escape_environment, dec_field->u.ty_field.id);
	bool escape = true;
	if (field_escape_symbol != NULL) 
		escape = field_escape_symbol->environment_entry->u.escape_entry.escapes;
	
	Tr_Access var_access =  Tr_allocLocal(sem->scope_head->current_level, escape);
	
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
		field_res = type_symbol->environment_entry->u.var_entry.variable_type;

	TreeIR value_result = check_exp(declaration->u.field_var_dec.val, sem);
	if (match_types(actual_type(field_res), actual_type(value_result->u.expression.exp_type)) == FALSE) {
		report_error(
			TypeError,
			"(PLACEHOLDER)",
			declaration->position->line_pos,
			declaration->position->col_pos,
			"Mismatched types",
			panic_mode
		);
		field_symbol->environment_entry->u.var_entry.variable_type = sem->builtin_error_type;
		return make_tree_ir_exp(NULL, sem->builtin_error_type);
	}
	field_symbol->environment_entry->u.var_entry.access = var_access;
	field_symbol->environment_entry->u.var_entry.variable_type = field_res;
		


	return make_tree_ir_exp(
		Tr_translateAssignment(
			Tr_translateVariableAccess(
				field_symbol->environment_entry->u.var_entry.access,
				sem->scope_head->current_level,
				type_cost(field_res)
			),
			value_result->u.expression.exp_ir
		),
		field_res
	);
}
void handle_function(A_Dec declaration, SemanticAnalyzer sem) {
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
			actual_result_type = make_field_type(id, arg_type_symbol->environment_entry->u.var_entry.variable_type);
			
		}
		Symbol argument_symbol = make_symbol(args->field->u.ty_field.id, make_var_entry(NULL, actual_result_type));
		


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
			function_symbol->environment_entry->u.function_entry.return_type = return_symbol->environment_entry->u.var_entry.variable_type;

	}
	else
		function_symbol->environment_entry->u.function_entry.return_type = sem->builtin_void_type;
	Type return_type = function_symbol->environment_entry->u.function_entry.return_type;
	resolve_type(return_type, sem, declaration->position);
	
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
		result = make_array_type(resulting_symbol->environment_entry->u.var_entry.variable_type);
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
		result = make_name_type(current_symbol, resulting_symbol->environment_entry->u.var_entry.variable_type);
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
				resulting_symbol->environment_entry->u.var_entry.variable_type
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

			Symbol type_header = make_symbol(id, make_var_entry(NULL, sem->builtin_error_type));
			
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

			Symbol function_header = make_symbol(id, make_function_entry(NULL, NULL, NULL, NULL));
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
			Symbol simple_var_header = make_symbol(id, make_var_entry(NULL, sem->builtin_error_type));
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
				Symbol field_var_header = make_symbol(id, make_var_entry(NULL, sem->builtin_error_type));
				sem->scope_head->var_environment = insert_symbol(sem->scope_head->var_environment, field_var_header);
			}
			break;
		}
	}
	return sem;
}

TreeIR process_statement(A_Stm stm, SemanticAnalyzer sem) {
	if (stm == NULL)
		return NULL;
	sem->currently_checking_in = Global;
	switch (stm->kind) {
		case Exp_Stm: {
			sem->ir_root = check_exp(stm->u.exp_stm.expression, sem);
			break;
		}
		case Decl_Stm: break;
		case Compound_Stm: {
			
			TreeIR left_res = process_statement(stm->u.compound_stm.stm1, sem);
			TreeIR right_res = process_statement(stm->u.compound_stm.stm2, sem);
			if (left_res == NULL)
				return NULL;
			if (right_res == NULL)
				return NULL;
			return make_tree_ir_compound(left_res, right_res);
			
		}
	}

	return sem->ir_root;
	
}
TreeIR semantic_main(SemanticAnalyzer sem) {
	A_Stm compound_stm = sem->parser->root;
	if (compound_stm == NULL)
		printf("\n Fuck\n");
	sem = begin_scope(sem);
	printf("\n MADE STANDARD ENV\n");
	

	int iter = 1;
	TreeIR root = process_statement(compound_stm, sem);
	return root;
}

