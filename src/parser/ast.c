#include "parser/ast.h"

// Constructors
Parser make_parser(void) {
      Parser new_parser = (Parser)checked_malloc(sizeof(struct Parser_));
      new_parser->root = NULL;
      new_parser->current_stm = -1;
      return new_parser;
}

A_Pos make_pos(size_t col_pos, size_t line_pos) {
	A_Pos position = (A_Pos)checked_malloc(sizeof(struct A_Pos_));
	position->col_pos = col_pos;
	position->line_pos = line_pos;
	return position;
}

A_Exp make_id_exp(string id, A_Pos position) {
	A_Exp new_id_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_id_exp->u.id_exp.identifier = strdup(id);
	
	new_id_exp->position = position;
	new_id_exp->kind = ID_Exp;
	return new_id_exp;
}

A_Exp make_num_exp(int num, A_Pos position) {
	A_Exp new_num_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));

	new_num_exp->kind = Num_Exp;
	new_num_exp->position = position;

	new_num_exp->u.num_exp.value = num;
	return new_num_exp;
}

A_Exp make_real_exp(double value, A_Pos position) {
	A_Exp new_real_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));

	new_real_exp->kind = Real_Exp;
	new_real_exp->position = position;

	new_real_exp->u.real_exp.value = value;
	
	return new_real_exp;
}

A_Exp make_nil_exp(A_Pos position) {
	A_Exp new_nil_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_nil_exp->kind = NIL_Exp;
	new_nil_exp->position = position;

	return new_nil_exp;
}

A_Exp make_bool_exp(int boolean, A_Pos position) {
	A_Exp new_bool_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_bool_exp->kind = Bool_Exp;
	new_bool_exp->position = position;
	new_bool_exp->u.bool_exp.boolean = boolean;

	return new_bool_exp;
}

A_Exp make_char_exp(char character, A_Pos position) {
	A_Exp new_char_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_char_exp->kind = Char_Exp;
	new_char_exp->position = position;

	new_char_exp->u.char_exp.character = character;
	
	return new_char_exp;
}

A_Exp make_string_exp(string text, A_Pos position) {
	A_Exp new_string_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_string_exp->kind = String_Exp;
	new_string_exp->position = position;

	new_string_exp->u.string_exp.text = strdup(text);

	return new_string_exp;
}


A_ExpList make_exp_list(A_Exp exp) {
	A_ExpList new_exp_list = (A_ExpList)checked_malloc(sizeof(struct A_ExpList_));
	new_exp_list->exp = exp;
	new_exp_list->next = NULL;
	return new_exp_list;
}

A_Exp make_unary_exp(A_Op op, A_Exp exp, int post_fix, A_Pos position) {
	A_Exp new_unary_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_unary_exp->kind = Unary_Exp;
	new_unary_exp->position = position;

	new_unary_exp->u.unary_exp.op = op;
	new_unary_exp->u.unary_exp.exp = exp;
	new_unary_exp->u.unary_exp.postfix = post_fix;

	return new_unary_exp;

}
A_Exp make_op_exp(A_Op op, A_Exp exp1, A_Exp exp2) {
	A_Exp new_op_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_op_exp->kind = Op_Exp;
        new_op_exp->position = exp1->position;

	new_op_exp->u.op_exp.op = op;
	new_op_exp->u.op_exp.exp1 = exp1;
	new_op_exp->u.op_exp.exp2 = exp2;

	return new_op_exp;
}

A_Exp make_callee_exp(string id, A_ExpList args, A_Pos position) {
	A_Exp callee_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	callee_exp->kind = Callee_Exp;
	callee_exp->position = position;

	callee_exp->u.callee_exp.id = strdup(id);
	callee_exp->u.callee_exp.args = args;

	return callee_exp;
}

A_Exp make_array_exp(string type_id, A_Exp size, A_Exp init, A_Pos position) {
	A_Exp new_array_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_array_exp->kind = Array_Exp;
	new_array_exp->position  = position;


        new_array_exp->u.array_exp.size = size;
	new_array_exp->u.array_exp.type_id = strdup(type_id);
	new_array_exp->u.array_exp.init = init;

	return new_array_exp;
}



A_Field make_subscript_field(string id, A_Exp loc, A_Pos position) {
	A_Field new_field = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_field->kind = Subscript_Field;
	new_field->position = position;

	new_field->u.subscript_field.id = strdup(id);
	new_field->u.subscript_field.loc = loc;
	
	return new_field;
}

A_Field make_type_field(string id,  string type, A_Pos position) {
	A_Field new_field = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_field->kind = Ty_Field;
	new_field->position = position;

	new_field->u.ty_field.id = strdup(id);
	new_field->u.ty_field.type = strdup(type);
	return new_field;
}

A_FieldList make_field_list(A_Field field) {
	A_FieldList new_field_list = (A_FieldList)checked_malloc(sizeof(struct A_FieldList_));
	new_field_list->field = field;
	new_field_list->next = NULL;

	return new_field_list;
}

A_Field make_record(string type_id, A_FieldList field_list, A_Pos position) {
	A_Field new_record = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_record->kind = Record; 
	new_record->position = position;
	new_record->u.record_field.type_id = strdup(type_id);
	new_record->u.record_field.record_def_fields = field_list;
	return new_record;
}
A_Field make_array_field(string type_id, A_Pos position) {
	A_Field new_array_field = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_array_field->kind = Array_Field;
	new_array_field->position = position;

	new_array_field->u.array_field.type_id = strdup(type_id);
	
	return new_array_field;
}
A_Field make_ref_field(A_Exp left_id, A_Exp right_id, A_Pos position) {
	A_Field new_ref_field = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_ref_field->kind = Ref_Field;
	new_ref_field->position = position;
	
	new_ref_field->u.ref_field.left_id = left_id;
	new_ref_field->u.ref_field.right_id = right_id;

	return new_ref_field;
}
A_Field make_item_field(string id, A_Exp value, A_Pos position) {
	A_Field new_item_field = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_item_field->kind = Item_Field;
	new_item_field->position = position;

	new_item_field->u.item_field.id = strdup(id);
	new_item_field->u.item_field.value = value;

	return new_item_field;
}
A_Field make_ty_record(A_FieldList field_list, A_Pos position) {
	A_Field new_ty_record = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_ty_record->kind = Ty_Record;
	new_ty_record->position = position;

	new_ty_record->u.type_record_field.record_type_fields = field_list;
	return new_ty_record;
}
A_Exp make_field_exp(A_Field field) {
	A_Exp new_field_expression = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_field_expression->kind = Field_Exp;
	new_field_expression->position = field->position;

	new_field_expression->u.field_exp.field = field;
	return new_field_expression;
}


A_Dec make_simple_var_dec(string id, A_Exp var_val, A_Pos position) {
	A_Dec new_simple_var_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_simple_var_dec->kind = Simple_Var_Dec;
	new_simple_var_dec->position = position;

	new_simple_var_dec->u.simple_var_dec.id = strdup(id);
	new_simple_var_dec->u.simple_var_dec.val = var_val;

	return new_simple_var_dec;

}
A_Dec make_field_var_dec(A_Field type_field, A_Exp var_val, A_Pos position) {
	A_Dec new_field_var_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_field_var_dec->kind = Field_Var_Dec;
	new_field_var_dec->position = position;

	new_field_var_dec->u.field_var_dec.field = type_field;
	new_field_var_dec->u.field_var_dec.field = type_field;
	new_field_var_dec->u.field_var_dec.val = var_val;

	return new_field_var_dec;
}
A_Dec make_type_dec(string name, A_Field def_type_field, A_Pos position) {
	A_Dec new_type_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_type_dec->kind = Type_Dec;
	new_type_dec->position = position;

	new_type_dec->u.type_dec.name = name;
	new_type_dec->u.type_dec.def_type_field = def_type_field;
        
	return new_type_dec;

}

A_Dec make_func_dec(string name, A_FieldList args, string type, A_Exp block, A_Pos position) {
	A_Dec new_func_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_func_dec->kind = Func_Dec;
	new_func_dec->position = position;

	new_func_dec->u.func_dec.name = strdup(name);
	new_func_dec->u.func_dec.args = args;
	new_func_dec->u.func_dec.type = strdup(type);
	new_func_dec->u.func_dec.block = block;

	return new_func_dec;
}

A_DecList make_dec_list(A_Dec declaration) {
	A_DecList new_dec_list = (A_DecList)checked_malloc(sizeof(struct A_DecList_));
	new_dec_list->dec = declaration;
	new_dec_list->next = NULL;
	return new_dec_list;
}

A_Exp make_while_exp(A_Exp while_cond, A_Exp block, A_Pos position) {
	A_Exp new_while_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_while_exp->kind = While_Exp;
	new_while_exp->position = position;

	new_while_exp->u.while_exp.cond = while_cond;
	new_while_exp->u.while_exp.block = block;
	
	return new_while_exp;
}

A_Exp make_if_exp(A_Exp cond, A_Exp then, A_Exp else_branch, A_Pos position) {
	A_Exp new_if_chain = (A_Exp)checked_malloc(sizeof(struct A_Exp_));

	new_if_chain->kind = If_Exp;
	new_if_chain->position = position;

	new_if_chain->u.if_exp.cond = cond;
	new_if_chain->u.if_exp.then = then;
	new_if_chain->u.if_exp.else_block = else_branch;

	return new_if_chain;
}

A_Exp make_seq_exp(A_ExpList exp_list) {



	A_Exp new_seq_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_seq_exp->kind = Seq_Exp;
	if (exp_list == NULL || exp_list->exp == NULL) {
		new_seq_exp->position = make_pos(0, 0);
		new_seq_exp->u.seq_exp.exp_list = NULL;
	}
	else {
		new_seq_exp->position = exp_list->exp->position;

		new_seq_exp->u.seq_exp.exp_list = exp_list;
	}
	return new_seq_exp;
}

A_Stm make_compound_stm(A_Stm stm1, A_Stm stm2) {
	A_Stm new_compound_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	
	new_compound_stm->kind = Compound_Stm;
	new_compound_stm->position = stm1->position;

	new_compound_stm->u.compound_stm.stm1 = stm1;
	new_compound_stm->u.compound_stm.stm2 = stm2;

	return new_compound_stm;
}

A_Exp make_assign_exp(A_Exp identifier, A_Exp exp) {
	A_Exp new_assign_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_assign_exp->kind = Assign_Exp;
	new_assign_exp->position = identifier->position;

	new_assign_exp->u.assign_exp.identifier = identifier;
	new_assign_exp->u.assign_exp.val = exp;

        return new_assign_exp;
}

A_Stm make_expression_stm(A_Exp exp) {
	if (exp == NULL)
		return NULL;
	A_Stm new_expression_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_expression_stm->kind = Exp_Stm;
	
	new_expression_stm->position = exp->position;

	new_expression_stm->u.exp_stm.expression = exp;

	return new_expression_stm;
}

A_Exp make_for_exp(A_Exp low_id, A_Exp low, A_Exp high, A_Exp block, A_Pos position) {
	A_Exp new_for_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_for_exp->kind = For_Exp;
	new_for_exp->position = position;

	new_for_exp->u.for_exp.low_id = low_id;
	new_for_exp->u.for_exp.low = low;
	new_for_exp->u.for_exp.high = high;
	new_for_exp->u.for_exp.block = block;
	
	return new_for_exp;
}

A_Exp make_let_exp(A_DecList dec_stms, A_Exp block, A_Pos position) {
	A_Exp new_let_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_let_exp->kind = Let_Exp;
	new_let_exp->position = position;

	new_let_exp->u.let_exp.declarations = dec_stms;
	new_let_exp->u.let_exp.block = block;

	return new_let_exp;
}


A_Exp make_break_exp(A_Pos position) {
	A_Exp new_break_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_break_exp->kind = Break_Exp;
	new_break_exp->position = position;

	return new_break_exp;
}
A_Exp make_continue_exp(A_Pos position) {
	A_Exp new_continue_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_continue_exp->kind = Continue_Exp;
	new_continue_exp->position = position;

	return new_continue_exp;
}
A_Stm make_declaration_stm(A_Dec dec) {
	A_Stm new_declaration_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_declaration_stm->kind = Decl_Stm;
	new_declaration_stm->position = dec->position;

	new_declaration_stm->u.decl_stm.dec = dec;

	return new_declaration_stm;
}
A_Op match_op(Token operation) {
      #define X(input_token, op) \
        if (operation->token_type == input_token) return op; 
	OP_MATCH
      #undef X

      
      return OP_INVALID;
}

A_ExpList parse_explist(Lexer lexer, Parser parser, token delimiter) {
	if (match(peek(lexer->queue), R_PAREN) == TRUE)
		return NULL;

	A_Exp current_exp = parse_expression(lexer, parser);
	
	A_ExpList head = make_exp_list(current_exp);
	A_ExpList tail = head;
	Token current_token = peek(lexer->queue);
	while (TRUE) {
		current_token = peek(lexer->queue);

		if (match(current_token, R_PAREN) == TRUE)
			break;

		if (match(current_token, delimiter) == TRUE) {
			eat_token(lexer->queue);
			eat_lines(lexer, parser);
		}

		if (match(peek(lexer->queue), R_PAREN) == TRUE)
			break;

		A_Exp next = parse_expression(lexer, parser);
		if (next == NULL)
			break;

		tail->next = make_exp_list(next);
		tail = tail->next;
	}
	return head;
}
A_Field parse_field(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	A_Pos position = make_pos(current_token->char_pos, current_token->line_pos);
	A_Field current_field = NULL;
	if (match(current_token, ID) == FALSE) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Unrecgonized field"
		);
	}

	string id = strdup(current_token->input);

	eat_token(lexer->queue);
	current_token = peek(lexer->queue);

	if (match(current_token, COLON) == TRUE) {
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);
		if (match(current_token, ID) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must specify ID after : for type fields"
			);
		}
		current_token = peek(lexer->queue);
		string type_id = strdup(current_token->input);
		eat_token(lexer->queue);
		current_field = make_type_field(id, type_id, position);
	}
	else if (match(current_token, EQ) == TRUE) {
		eat_token(lexer->queue);
		
		A_Exp value = parse_expression(lexer, parser);
		current_field = make_item_field(id, value, position);
	}
	else {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Unrecgonized field"
		);
	}

	return current_field;
}
A_FieldList parse_fieldlist(Lexer lexer, Parser parser, token delimiter) {
	if (match(peek(lexer->queue), R_PAREN) == TRUE || match(peek(lexer->queue), R_CURLY_BRCKT) == TRUE)
		return NULL;

	A_Field field = parse_field(lexer, parser);
	if (field == NULL)
		return NULL;

	A_FieldList head = make_field_list(field);
	A_FieldList field_list = head;
	while (TRUE) {
		Token current_token = peek(lexer->queue);
		if (match(current_token, R_CURLY_BRCKT) == TRUE || match(current_token, R_PAREN) == TRUE)
			break;
		if (match(current_token, delimiter) == TRUE) {
			eat_token(lexer->queue);
			eat_lines(lexer, parser);
		} else
			break;

		field = parse_field(lexer, parser);
		if (field == NULL)
			break;

		field_list->next = make_field_list(field);
		field_list = field_list->next;
		eat_lines(lexer, parser);	
	}

	return head;
}

A_Exp parse_primary(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	A_Exp current_exp = NULL;
	if (current_token == NULL)
		return NULL;
	A_Pos position = make_pos(current_token->line_pos, current_token->char_pos);

	printf("\nPrimary Input: %s \n", current_token->input);
	if (match(current_token, NUM) == TRUE) {
		current_exp = make_num_exp(atoi(current_token->input), position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, REAL) == TRUE) {
		current_exp = make_real_exp(atof(current_token->input), position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, STRING_VAL) == TRUE) {
		current_exp = make_string_exp(current_token->input, position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, CHAR) == TRUE) {
		current_exp = make_char_exp(current_token->input[0], position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, BREAK) == TRUE) {
		if (parser->current_stm != While_Exp && parser->current_stm != For_Exp) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"BREAK expressions can only be in For and While Expressions"
			);
		}
		current_exp = make_break_exp(position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, CONTINUE) == TRUE) {
		if (parser->current_stm != While_Exp && parser->current_stm != For_Exp) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"CONTINUE expressions can only be in For and While Expressions"
			);
		}
		current_exp = make_continue_exp(position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, FALSE_VAL) == TRUE) {
		current_exp = make_bool_exp(FALSE, position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, TRUE_VAL) == TRUE) {
		current_exp = make_bool_exp(TRUE, position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, NULL_VAL) == TRUE) {
		current_exp = make_nil_exp(position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, ID) == TRUE) {	
	     	current_exp = make_id_exp(current_token->input, position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, L_PAREN) == TRUE) {
		eat_token(lexer->queue);
		eat_lines(lexer, parser);
		A_ExpList exp_list = parse_explist(lexer, parser, SEMI_COLON);
		if (exp_list == NULL)
			return make_seq_exp(NULL);
		current_exp = make_seq_exp(exp_list);

		current_token = peek(lexer->queue);
		if (match(current_token, R_PAREN) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Unclosed expression"
			);
		}
		eat_token(lexer->queue);
		return current_exp;
	}
	
	return current_exp;
}
A_Exp parse_data_structure(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	if (current_token == NULL)
		return NULL;
	A_Pos position = make_pos(current_token->char_pos, current_token->line_pos);
	// Array Field Production
	if (match(current_token, ARRAY) == TRUE) {
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);
		if (match(current_token, OF) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"OF must follow ARRAY keyword"
			);
		}
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);

		if (match(current_token, ID) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"ID must follow OF"
			);
		}
		string id = strdup(current_token->input);
		eat_token(lexer->queue);
		return make_field_exp(
			make_array_field(
				id,
				position
			)
		);
	
	}
	else if (match(current_token, L_CURLY_BRCKT) == TRUE) {
		eat_token(lexer->queue);
		A_FieldList record_ty_fields = parse_fieldlist(lexer, parser, COMMA);

		current_token = peek(lexer->queue);
		if (match(current_token, R_CURLY_BRCKT) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Unclosed item record"
			);
		}
		eat_token(lexer->queue);
		return make_field_exp(
			make_ty_record(
				record_ty_fields,
				position
			)
		);
		
	}
	else if	(match(current_token, ID) == TRUE) {
		if (current_token->next != NULL && match(current_token->next, L_SQUARE_BRCKT) == TRUE) {
			string id = strdup(current_token->input);
			eat_token(lexer->queue);
			eat_token(lexer->queue);

			A_Exp init = parse_expression(lexer, parser);
			current_token = peek(lexer->queue);

			if (match(current_token, R_SQUARE_BRCKT) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos,
					"Must close with ]"
				);
			}
			eat_token(lexer->queue);
			current_token = peek(lexer->queue);
			if (match(current_token, OF) == FALSE) {
				
				return make_field_exp(
					make_subscript_field(id, init, position)
				);
			}
			eat_token(lexer->queue);
			A_Exp size = parse_term(lexer, parser);
                        return make_array_exp(id, size, init, position);

		}
		
	}
	return parse_primary(lexer, parser);
}
A_Exp parse_unary(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	if (current_token == NULL)
		return NULL;
	printf("\n Unary Input: %s\n", current_token->input);
	A_Op operation = match_op(current_token);
	

	if (operation != OP_INVALID && (operation == OP_INCREMENT || 
	operation == OP_DECREMENT || operation == OP_NOT || operation == OP_SUB)) {
	      eat_token(lexer->queue);
	      A_Exp current_exp = parse_data_structure(lexer, parser);
	      if (current_exp == NULL)
	      	return NULL;
	      current_exp = make_unary_exp(operation, current_exp, FALSE, current_exp->position);
	     
	      return current_exp;
	}
	return parse_data_structure(lexer, parser);
}
A_Exp parse_postfix(Lexer lexer, Parser parser) {
	A_Exp left = parse_unary(lexer, parser);
	if (left == NULL)
		return NULL;

	Token current_token = peek(lexer->queue);
        A_Exp current_exp = left;
	printf("\n Postfix Input: %s\n", current_token->input);

	if (match(current_token, INCREMENT) == TRUE) {
		eat_token(lexer->queue);
		current_exp = make_unary_exp(OP_INCREMENT, current_exp, TRUE, current_exp->position);
	}
	else if (match(current_token, DECREMENT) == TRUE) {
		eat_token(lexer->queue);
		current_exp = make_unary_exp(OP_DECREMENT, current_exp, TRUE, current_exp->position);
	}
        else if (left->kind == ID_Exp) {
	        string id = left->u.id_exp.identifier;
		int discover_reference = TRUE;

		if (match(current_token, L_SQUARE_BRCKT) == TRUE) {
			eat_token(lexer->queue);

			current_token = peek(lexer->queue);
			A_Exp loc_exp = parse_expression(lexer, parser);
			current_token = peek(lexer->queue);
			if (match(current_token, R_SQUARE_BRCKT) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos,
					"Must close with ]"
				);
			}
                        
			eat_token(lexer->queue);
			current_exp = make_field_exp(
				make_subscript_field(id, loc_exp, left->position)
			);
		}
		else if (match(current_token, L_PAREN) == TRUE) {
			eat_token(lexer->queue);
			
			current_token = peek(lexer->queue);
			A_ExpList args = parse_explist(lexer, parser, COMMA);
			current_token = peek(lexer->queue);
			printf("\n Token before R_PAREN CHeck: %s \n", current_token->input);
			if (match(current_token, R_PAREN) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos,
					"Must close callee with )"
				);
			}

			eat_token(lexer->queue);
			current_exp = make_callee_exp(id, args, left->position);
		}
		else if (match(current_token, L_CURLY_BRCKT) == TRUE) {
			eat_token(lexer->queue);

			A_FieldList record_item_fields = parse_fieldlist(lexer, parser, COMMA);
			
			current_token = peek(lexer->queue);

			if(match(current_token, R_CURLY_BRCKT) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos,
					"Must close item record with }"
				);
			}

			eat_token(lexer->queue);
			current_exp = make_field_exp(
				make_record(
					id,
					record_item_fields,
					left->position
				)
			);
			discover_reference = FALSE;
		}
		else if (match(current_token, COLON) == TRUE) {
			eat_token(lexer->queue);
			
			current_token = peek(lexer->queue);
			A_Exp right_exp = parse_primary(lexer, parser);

			if (right_exp->kind != ID_Exp) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos,
					"Unrecognized identifier"
				);
			}

			current_exp = make_field_exp(
				make_type_field(
					id,
					right_exp->u.id_exp.identifier,
					left->position
				)
			);
			discover_reference = FALSE;
		}
		current_token = peek(lexer->queue);
		if (match(current_token, MEMBER_REF) == TRUE && discover_reference == TRUE) {
			eat_token(lexer->queue);

			A_Exp right_id = parse_postfix(lexer, parser);

			current_exp = make_field_exp(
				make_ref_field(
					left,
					right_id,
					left->position
				)
			);
		}
		
               
	}	
	return current_exp;
}
A_Exp parse_bitwise(Lexer lexer, Parser parser) {
	A_Exp left = parse_postfix(lexer, parser);
	if (left == NULL)
		return NULL;
	Token current_token = peek(lexer->queue);
	A_Op op = match_op(current_token);
	while (op != OP_INVALID && (op == OP_LSHIFT || op == OP_RSHIFT || 
	op == OP_COMPAR_NOT_EQ || op == OP_EQ || op == OP_GT || op == OP_GT_EQ || op == OP_LT || op == OP_LT_EQ )) {
		eat_token(lexer->queue);
		A_Exp right = parse_postfix(lexer, parser);
		if (right == NULL) {
			report_error(
				SyntaxError,
				current_token->input, 
				current_token->line_pos,
				current_token->char_pos,
				"Invalid Expression"
			);
		}
		left = make_op_exp(op, left, right);
		current_token = peek(lexer->queue);
		op = match_op(current_token);
	}
	return left;
}
A_Exp parse_factor(Lexer lexer, Parser parser) {
	A_Exp left = parse_bitwise(lexer, parser);
	if (left == NULL)
		return NULL;
	Token current_token = peek(lexer->queue);
	A_Op op = match_op(current_token);
	while (op != OP_INVALID && 
		(op == OP_MUL || op == OP_DIV || op == OP_MOD || op == OP_OR || op == OP_COMPAR_OR)) {
		eat_token(lexer->queue);
		A_Exp right = parse_bitwise(lexer, parser);
		if (right == NULL) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Invalid Expression"
			);
		}
		left = make_op_exp(op, left, right);
		current_token = peek(lexer->queue);
		op = match_op(current_token);
	}
	return left;
}

A_Exp parse_term(Lexer lexer, Parser parser) {
	A_Exp left = parse_factor(lexer, parser);
	if (left == NULL)
		return NULL;
	Token current_token = peek(lexer->queue);
	A_Op op = match_op(current_token);
	printf("\n  Input of term %s \n", peek(lexer->queue)->input);
	while (op != OP_INVALID && (op == OP_ADD || op == OP_SUB || op == OP_AND || op == OP_COMPAR_AND))	{
	     eat_token(lexer->queue);
	     
	     A_Exp right = parse_factor(lexer, parser);
	     if (right == NULL) {
	     	report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Invalid Expression"
		);
	     }

	     left = make_op_exp(op, left, right);
	     current_token = peek(lexer->queue);
	     op = match_op(current_token);
		
	}
	printf("\n parsed term\n");
	return left;
}
A_Exp parse_control_exp(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	A_Pos position = make_pos(current_token->line_pos, current_token->char_pos);
	

	if (match(current_token, WHILE) == TRUE) {
		eat_token(lexer->queue);
		parser->current_stm = While_Exp;

		A_Exp condition = parse_expression(lexer, parser);
		eat_lines(lexer, parser);
		current_token = peek(lexer->queue);
		printf("\n Input %s \n", current_token->input);
		if (match(current_token, DO) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must specify DO with while expressions"
			);
		}
		eat_token(lexer->queue);
		eat_lines(lexer, parser);
		parser->current_stm = While_Exp;

		A_Exp block = parse_expression(lexer, parser);
		return make_while_exp(condition, block, position);		
	}
	else if (match(current_token, ID) == TRUE) {
		A_Exp id_exp = parse_term(lexer, parser);
		if (id_exp->kind != ID_Exp && 
		(id_exp->kind == Field_Exp && id_exp->u.field_exp.field->kind != Subscript_Field)) {
			return id_exp;	
		}

		current_token = peek(lexer->queue);

		if (match(current_token, ASSIGN) == FALSE) {
			return id_exp;
		}

		eat_token(lexer->queue);
		current_token = peek(lexer->queue);

		A_Exp val = parse_term(lexer, parser);

		return make_assign_exp(id_exp, val);

	}
	else if (match(current_token, FOR) == TRUE) {
		eat_token(lexer->queue);
		
		A_Exp low_id_exp = parse_term(lexer, parser);
		if (low_id_exp->kind != ID_Exp) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"For expression must start with ID expression"
			);
		}
		current_token = peek(lexer->queue);
		if (match(current_token, ASSIGN) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must specify := after ID expression in for expression"
			);
		}

		eat_token(lexer->queue);

		A_Exp low = parse_term(lexer, parser);

		eat_lines(lexer, parser);
		
		
		current_token = peek(lexer->queue);

		if (match(current_token, TO) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must specify TO after assign exp"
			);
		}
		eat_token(lexer->queue);
		eat_lines(lexer, parser);

		A_Exp high = parse_term(lexer, parser);
		eat_lines(lexer, parser);
		current_token = peek(lexer->queue);

		if (match(current_token, DO) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must specify DO in for exp"
			);
		}
		eat_token(lexer->queue);
		eat_lines(lexer, parser);
		parser->current_stm = For_Exp;
		A_Exp block = parse_expression(lexer, parser);
		return make_for_exp(low_id_exp, low, high, block, position);
	}
	else if (match(current_token, IF) == TRUE) {
		eat_token(lexer->queue);

		A_Exp conditional = parse_expression(lexer, parser);
		eat_lines(lexer, parser);
		current_token = peek(lexer->queue);
		if (match(current_token, THEN) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must specify THEN for if expressions"
			);
		}
		eat_token(lexer->queue);
		eat_lines(lexer, parser);

		A_Exp block = parse_expression(lexer, parser);
		eat_lines(lexer, parser);

		A_Exp else_branch = NULL;
		current_token = peek(lexer->queue);

		if (match(current_token, ELSE) == TRUE) {
			eat_token(lexer->queue);
			else_branch = parse_expression(lexer, parser);
		}
		return make_if_exp(conditional, block, else_branch, position);
	}
        else if (match(current_token, LET) == TRUE) {
		eat_token(lexer->queue);
		eat_lines(lexer, parser);

	
		A_DecList declaration_list_head = parse_declarations(lexer, parser);
		
		current_token = peek(lexer->queue);
		printf("\n Input before IN %s \n", current_token->input);
		if (match(current_token, IN) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must include IN within LET expression after declarations"
			);
		}

		eat_token(lexer->queue);
		eat_lines(lexer, parser);

		A_ExpList block_list = NULL;
		A_ExpList tail = block_list;
		eat_lines(lexer, parser);
		current_token = peek(lexer->queue);
		while (match(current_token, END) == FALSE) {
			
			A_Exp current_exp = parse_expression(lexer, parser); 

			if (block_list == NULL) {
				block_list = make_exp_list(current_exp);
				tail = block_list;
			}
			else {
				tail->next = make_exp_list(current_exp);
				tail = tail->next;
			}
			
			if(match(peek(lexer->queue), SEMI_COLON) == TRUE)
				eat_token(lexer->queue);
			eat_lines(lexer, parser);
			current_token = peek(lexer->queue);
		}
		A_Exp block = make_seq_exp(block_list);

		current_token = peek(lexer->queue);
		if (match(current_token, END) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Must end LET statement with END"
			);
		} 
		eat_token(lexer->queue);

		return make_let_exp(declaration_list_head, block, position);

	}
	return parse_term(lexer, parser);
}
A_Exp parse_expression(Lexer lexer, Parser parser) {
	
	return parse_control_exp(lexer, parser);
}
A_DecList parse_declarations(Lexer lexer, Parser parser) {
	A_DecList head = NULL;
	A_DecList tail = head;

	
	while (TRUE) {
	
		Token current_token = peek(lexer->queue);
		
		if (match(current_token, IN) == TRUE)
			break;
		if (current_token == NULL)
			break;
		if (match(current_token, VAR_DEC) == FALSE && match(current_token, TYPE_DEC) == FALSE && match(current_token, FUNCTION_DEF) == FALSE) {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Expected declaration"
			);
		}
		printf("\n Input: %s \n", current_token->input);

		A_Dec declaration = parse_declaration(lexer, parser);
		if (declaration == NULL)
			break;
		if (head == NULL) {
			head = make_dec_list(declaration);
			tail = head;
		} else {
			tail->next = make_dec_list(declaration);
			tail = tail->next;
		}
		eat_lines(lexer, parser);
		
	}
	return head;

}
A_Dec parse_declaration(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	A_Dec current_declaration = NULL;
	A_Pos position = make_pos(current_token->char_pos, current_token->line_pos);

	if (match(current_token, VAR_DEC) == TRUE) {
		eat_token(lexer->queue);
		current_declaration = parse_variable(lexer, parser, position);
	}
	else if (match(current_token, TYPE_DEC) == TRUE) {
		eat_token(lexer->queue);
		current_declaration= parse_type(lexer, parser, position);
	}
	else if (match(current_token, FUNCTION_DEF) == TRUE) {
		eat_token(lexer->queue);
		current_declaration = parse_function_dec(lexer, parser, position);
	}
	else {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Expected declaration"
		);
	}
	if (match(peek(lexer->queue), SEMI_COLON) == TRUE)
		eat_token(lexer->queue);
	return current_declaration;
}
A_Dec parse_variable(Lexer lexer, Parser parser, A_Pos position) {
	A_Dec current_declaration = NULL;
	A_Exp id = parse_primary(lexer, parser);
        Token current_token = peek(lexer->queue);
	if (match(current_token, ASSIGN) == TRUE) {
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);
		
		A_Exp val = parse_expression(lexer, parser);
		if (id->kind == ID_Exp) {
			current_declaration = make_simple_var_dec(
				id->u.id_exp.identifier,
				val,
				position
			);
		}
		else if (id->kind == Field_Exp && id->u.field_exp.field->kind == Ty_Field) {
			current_declaration = make_field_var_dec(
				id->u.field_exp.field,
				val,
				position
			);
		}
		else {
			report_error(
				SyntaxError,
				current_token->input,
				current_token->line_pos,
				current_token->char_pos,
				"Var declarations must be specified with IDs"
			);
		}
	}
	else {
		report_error(
			SyntaxError, 
			current_token->input, 
			current_token->line_pos, 
			current_token->char_pos, 
			"Must specify declarations with :="
		);
	}

	return current_declaration;
}
A_Dec parse_type(Lexer lexer, Parser parser, A_Pos position) {
	A_Dec current_declaration = NULL;
	A_Exp id = parse_data_structure(lexer, parser);
	if (id == NULL)
		return NULL;
        Token current_token = peek(lexer->queue);
	if (id->kind != ID_Exp) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Type declarations must come with IDs"
		);
	}
	if (match(current_token, EQ) == FALSE) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Invalid type dec"
		);
	}
	eat_token(lexer->queue);

        A_Exp type_val = parse_data_structure(lexer, parser);
	current_token = peek(lexer->queue);
	
	if (type_val->kind != ID_Exp && type_val->kind != Field_Exp) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Invalid expression for type dec"
		);
	}

        if (type_val->kind == ID_Exp) {
		current_declaration = make_type_dec(
			id->u.id_exp.identifier,
                        make_type_field(
				type_val->u.id_exp.identifier,
				"",
				type_val->position
			),
			position

		);
	}
	else {
		current_declaration = make_type_dec(
			id->u.id_exp.identifier,
			type_val->u.field_exp.field,
			position
		);
	}

	return current_declaration;

	

}
A_Dec parse_function_dec(Lexer lexer, Parser parser, A_Pos position) {
	Token current_token = peek(lexer->queue);
	
	if (match(current_token, ID) != TRUE) {
		report_error(
			SyntaxError,
			"function",
			position->line_pos,
			position->col_pos,
			"Must specify function name"
		);
	}
	string id = strdup(current_token->input);

	eat_token(lexer->queue);
	current_token = peek(lexer->queue);

	if(match(current_token, L_PAREN) == FALSE) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Forgot ("
		);
	}
        eat_token(lexer->queue);
	
	A_FieldList args = parse_fieldlist(lexer, parser, COMMA);
	current_token = peek(lexer->queue);
	if (match(current_token, R_PAREN) == FALSE) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Forgot )"
		);
	}

	eat_token(lexer->queue);
	current_token = peek(lexer->queue);
	string type_id = "";
	if (match(current_token, COLON) == TRUE && match(current_token->next, ID) == TRUE) {
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);
		type_id = strdup(current_token->input);
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);		
	}

	if (match(current_token, EQ) == FALSE) {
		report_error(
			SyntaxError,
			current_token->input,
			current_token->line_pos,
			current_token->char_pos,
			"Must specify = in function declaration"
		);
	}
	eat_token(lexer->queue);
	eat_lines(lexer, parser);
	A_Exp block = parse_expression(lexer, parser);
	return make_func_dec(id, args,type_id, block, position);
}
void eat_lines(Lexer lexer, Parser parser) {
	while (peek(lexer->queue) != NULL && match(peek(lexer->queue), NEW_LINE) == TRUE)
		eat_token(lexer->queue);
}


A_Stm parse_stm(Lexer lexer, Parser parser) {
	eat_lines(lexer, parser);

 	Token current_token = peek(lexer->queue);


	if (match(current_token, VAR_DEC) == TRUE || 
		match(current_token, TYPE_DEC) == TRUE || 
		match(current_token, FUNCTION_DEF) == TRUE) {
		A_Dec declaration = parse_declaration(lexer, parser);
		return make_declaration_stm(declaration);
	}
	else {
		A_Exp exp = parse_expression(lexer, parser);
		if (exp == NULL) {
			eat_token(lexer->queue);
			return NULL;
		}
		return make_expression_stm(exp);
	 	
	}
	return NULL;
}


A_Stm parse_program(Lexer lexer, Parser parser) {
        A_Stm root = NULL;
	while (peek(lexer->queue) != NULL && match(peek(lexer->queue), END_OF_FILE) == FALSE) {
		A_Stm parsed_stm = parse_stm(lexer, parser);
		if (parsed_stm == NULL)
			continue;

		parser->current_stm = -1;

		if (match(peek(lexer->queue), SEMI_COLON) == TRUE || match(peek(lexer->queue), NEW_LINE) == TRUE)
			eat_token(lexer->queue);

		if (root == NULL)
			root = parsed_stm;
		else
			root = make_compound_stm(root, parsed_stm);
	}
	return root;
}
