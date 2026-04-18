#include "parser/ast.h"

// Constructors
Parser make_parser(void) {
      Parser new_parser = (Parser)checked_malloc(sizeof(struct Parser_));
      new_parser->root = NULL;
      new_parser->current_indentation_level = 0;
      new_parser->is_in_block = 0;
      new_parser->current_stm = -1;
      return new_parser;
}

A_Pos make_pos(size_t col_pos, size_t line_pos, size_t indentation_level) {
	A_Pos position = (A_Pos)checked_malloc(sizeof(struct A_Pos_));
	position->col_pos = col_pos;
	position->line_pos = line_pos;
	position->indentation_level = indentation_level;
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

	new_string_exp->u.string_exp.text = text;

	return new_string_exp;
}


A_ExpList make_exp_list(A_Exp exp) {
	A_ExpList new_exp_list = (A_ExpList)checked_malloc(sizeof(struct A_ExpList_));
	new_exp_list->exp = exp;
	new_exp_list->next = NULL;
	return new_exp_list;
}

A_Exp make_unary_exp(A_Op op, A_Exp exp, A_Pos position) {
	A_Exp new_unary_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_unary_exp->kind = Unary_Exp;
	new_unary_exp->position = position;

	new_unary_exp->u.unary_exp.op = op;
	new_unary_exp->u.unary_exp.exp = exp;

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

A_Exp make_callee_exp(string function_name, A_ExpList args, A_Pos position) {
	A_Exp callee_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	callee_exp->kind = Callee_Exp;
	callee_exp->position = position;

	callee_exp->u.callee_exp.function_name = strdup(function_name);
	callee_exp->u.callee_exp.args = args;

	return callee_exp;
}

A_Exp make_array_exp(string type, A_Exp size, A_Exp init, A_Pos position) {
	A_Exp new_array_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_array_exp->kind = Array_Exp;
	new_array_exp->position  = position;

	new_array_exp->u.array_exp.type = strdup(type);
        new_array_exp->u.array_exp.size = size;
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

A_Field make_type_field(string id, string type, A_Pos position) {
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

A_Field make_record(A_FieldList field_list, A_Pos position) {
	A_Field new_record = (A_Field)checked_malloc(sizeof(struct A_Field_));
	new_record->kind = Record; 
	new_record->position = position;

	new_record->u.record_field.record_def_fields = field_list;
	return new_record;
}
A_Exp make_field_exp(A_Field field) {
	A_Exp new_field_expression = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_field_expression->kind = Field_Exp;
	new_field_expression->position = field->position;

	new_field_expression->u.field_exp.field = field;
	return new_field_expression;
}


A_Dec make_var_dec(A_Field type_field, A_Exp var_val, A_Pos position) {
	A_Dec new_var_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_var_dec->kind = Var_Dec;
	new_var_dec->position = position;

	new_var_dec->u.var_dec.type_field = type_field;
	new_var_dec->u.var_dec.val = var_val;
	
	return new_var_dec;
}

A_Dec make_type_dec(A_Field type_field, A_Field def_type_field, A_Pos position) {
	A_Dec new_type_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_type_dec->kind = Type_Dec;
	new_type_dec->position = position;

	new_type_dec->u.type_dec.type_field = type_field;
	new_type_dec->u.type_dec.def_type_field = def_type_field;
        
	return new_type_dec;

}

A_Dec make_func_dec(string name, A_FieldList args, string type, A_Stm block, A_Pos position) {
	A_Dec new_func_dec = (A_Dec)checked_malloc(sizeof(struct A_Dec_));
	new_func_dec->kind = Func_Dec;
	new_func_dec->position = position;

	new_func_dec->u.func_dec.name = strdup(name);
	new_func_dec->u.func_dec.args = args;
	new_func_dec->u.func_dec.type = strdup(type);
	new_func_dec->u.func_dec.block = block;

	return new_func_dec;
}

A_DecList make_dec_List(A_Dec declaration) {
	A_DecList new_dec_list = (A_DecList)checked_malloc(sizeof(struct A_DecList_));
	new_dec_list->dec = declaration;
	new_dec_list->next = NULL;
	return new_dec_list;
}

A_Stm make_while_stm(A_Exp while_cond, A_Stm block, A_Pos position) {
	A_Stm new_while_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_while_stm->kind = While_Stm;
	new_while_stm->position = position;

	new_while_stm->u.while_stm.while_cond = while_cond;
	new_while_stm->u.while_stm.block = block;
	
	return new_while_stm;
}

A_Stm make_if_chain(A_Exp cond, A_Stm then_block, A_Pos position) {
	A_Stm new_if_chain = (A_Stm)checked_malloc(sizeof(struct A_Stm_));

	new_if_chain->kind = If_Stm;
	new_if_chain->position = position;

	new_if_chain->u.if_chain.condition = cond;
	new_if_chain->u.if_chain.then_block = then_block;
	new_if_chain->u.if_chain.else_branch = NULL;

	return new_if_chain;
}

A_Stm make_compound_stm(A_Stm stm1, A_Stm stm2) {
	A_Stm new_compound_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	
	new_compound_stm->kind = Compound_Stm;
	new_compound_stm->position = stm1->position;

	new_compound_stm->u.compound_stm.stm1 = stm1;
	new_compound_stm->u.compound_stm.stm2 = stm2;

	return new_compound_stm;
}

A_Stm make_assign_stm(string id, A_Exp exp, A_Pos pos) {
	A_Stm new_assign_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_assign_stm->kind = Assign_Stm;
	new_assign_stm->position = pos;

	new_assign_stm->u.assign_stm.id = strdup(id);
	new_assign_stm->u.assign_stm.exp = exp;

        return new_assign_stm;
}

A_Stm make_expression_stm(A_Exp exp) {
	A_Stm new_expression_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_expression_stm->kind = Exp_Stm;
	new_expression_stm->position = exp->position;

	new_expression_stm->u.expression_stm.exp = exp;

	return new_expression_stm;
}

A_Stm make_for_stm(string id, A_Exp low, A_Exp high, A_Stm block, A_Pos position) {
	A_Stm new_for_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_for_stm->kind = For_Stm;
	new_for_stm->position = position;

	new_for_stm->u.for_stm.id = strdup(id);
	new_for_stm->u.for_stm.low = low;
	new_for_stm->u.for_stm.high = high;
	new_for_stm->u.for_stm.block = block;
	
	return new_for_stm;
}


A_StmList make_stm_list(A_Stm stm) {
	A_StmList new_stm_list = (A_StmList)checked_malloc(sizeof(struct A_StmList_));
	new_stm_list->stm = stm;
	new_stm_list->next = NULL;
	return new_stm_list;
}

A_Stm make_let_stm(A_DecList dec_stms, A_Stm block, A_Pos position) {
	A_Stm new_let_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_let_stm->kind = Let_Stm;
	new_let_stm->position = position;

	new_let_stm->u.let_stm.dec_stms = dec_stms;
	new_let_stm->u.let_stm.block = block;

	return new_let_stm;
}
A_Stm make_break_stm(A_Pos position) {
	A_Stm new_break_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_break_stm->kind = Break_Stm;
	new_break_stm->position = position;

	return new_break_stm;
}
A_Stm make_return_stm(A_Exp exit_status, A_Pos position) {
	A_Stm new_return_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_return_stm->kind = Return_Stm;
	new_return_stm->position = position;

	new_return_stm->u.return_stm.exit_status = exit_status;
	return new_return_stm;
}
A_Stm make_declaration_stm(A_Dec dec) {
	A_Stm new_declaration_stm = (A_Stm)checked_malloc(sizeof(struct A_Dec_));
	new_declaration_stm->kind = Decl_Stm;
	new_declaration_stm->position = new_declaration_stm->position;

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
void handle_indentation(Lexer lexer, Parser parser) {
        size_t indentation = 0;
	Token first = peek(lexer->queue);
	Token current_token = first;
	while (match(current_token, TAB) == TRUE) {
		indentation++;
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);
	}
	if (indentation < parser->current_indentation_level)
		report_error(IndentError, "fuck", first->line_pos, first->char_pos);

	
}
A_ExpList parse_explist(Lexer lexer, Parser parser, token delimiter) {
	A_Exp current_exp = parse_expression(lexer, parser);
	
	A_ExpList head = make_exp_list(current_exp);
	A_ExpList exp_list = head;

	while (match(peek(lexer->queue), delimiter) == TRUE) {
		eat_token(lexer->queue);
		exp_list->next = make_exp_list(parse_expression(lexer, parser));
		exp_list = exp_list->next;
	}
	return head;
}
A_Exp parse_unary(Lexer lexer, Parser parser) {
	Token current_token = peek(lexer->queue);
	A_Exp current_exp = NULL;
	A_Pos position = make_pos(current_token->line_pos, current_token->char_pos, parser->current_indentation_level);
	if (match(current_token, NUM) == TRUE) {
		current_exp = make_num_exp(atoi(current_token->input), position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, REAL) == TRUE) {
		current_exp = make_real_exp(atof(current_token->input), position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, STRING) == TRUE) {
		current_exp = make_string_exp(current_token->input, position);
		eat_token(lexer->queue);
	}
	else if (match(current_token, CHAR) == TRUE) {
		current_exp = make_char_exp(current_token->input[0], position);
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
		
		if (match(current_token->next, L_PAREN) == TRUE) {
		        string function_name = current_token->input;
			eat_token(lexer->queue);
			eat_token(lexer->queue);
			A_ExpList arguments = parse_explist(lexer, parser, COMMA);
			current_token = peek(lexer->queue);
			if (match(current_token, R_PAREN) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos
				);
			}
			eat_token(lexer->queue);
			current_exp = make_callee_exp(function_name, arguments, position);
			printf("\n recognized callee\n");
		}
		else if (match(current_token->next, L_SQUARE_BRCKT) == TRUE) {
			string array_type = current_token->input;
			eat_token(lexer->queue);
			eat_token(lexer->queue);

			A_Exp size = parse_expression(lexer, parser);
			current_token = peek(lexer->queue);
			if (match(current_token, R_SQUARE_BRCKT) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
					current_token->line_pos,
					current_token->char_pos
				);
			}
                        eat_token(lexer->queue);
			current_token = peek(lexer->queue);
			if (match(current_token, OF) == FALSE) {
				report_error(
					SyntaxError,
					current_token->input,
                                        current_token->line_pos,
					current_token->char_pos
				);
			}
			eat_token(lexer->queue);
                        current_token = peek(lexer->queue);
			A_Exp init = parse_expression(lexer, parser);
                        
			current_exp = make_array_exp(array_type, size, init, position);

		}
		else {
			current_exp = make_id_exp(current_token->input, position);
			eat_token(lexer->queue);
		}
	}
	return current_exp;
}

A_Exp parse_factor(Lexer lexer, Parser parser) {
	A_Exp left = parse_unary(lexer, parser);

	A_Op op = match_op(peek(lexer->queue));
	while (op != OP_INVALID && 
		(op == OP_ADD || op == OP_SUB || op == OP_OR || op == OP_LSHIFT || 
		op == OP_RSHIFT || op == OP_OR || op == OP_COMPAR_OR || op == OP_COMPAR_EQ)) {
		eat_token(lexer->queue);
		A_Exp right = parse_unary(lexer, parser);
		left = make_op_exp(op, left, right);
		op = match_op(peek(lexer->queue));
	}
	return left;
}

A_Exp parse_term(Lexer lexer, Parser parser) {
	A_Exp left = parse_factor(lexer, parser);

	A_Op op = match_op(peek(lexer->queue));
	printf("\n Found Op\n");
	while (op != OP_INVALID && (op == OP_MUL || op == OP_MOD ||  
	op == OP_DIV || op == OP_AND || op == OP_COMPAR_AND))	{
	     eat_token(lexer->queue);
	     printf("\n Parsing term\n");
	     A_Exp right = parse_factor(lexer, parser);
	     left = make_op_exp(op, left, right);
	     op = match_op(peek(lexer->queue));
		
	}
	printf("\n parsed term\n");
	return left;
}
A_Exp parse_expression(Lexer lexer, Parser parser) {
        Token current_token = peek(lexer->queue);
	if (match(current_token, L_PAREN) == TRUE) {
	    eat_token(lexer->queue);

	    A_Exp expression = parse_expression(lexer, parser);
            current_token = peek(lexer->queue);

	    if (match(current_token, R_PAREN) == FALSE)
		report_error(SyntaxError, current_token->input, current_token->line_pos, current_token->char_pos);
	    return expression;
	}
	return parse_term(lexer, parser);
}

A_Stm parse_stm(Lexer lexer, Parser parser) {
 	Token current_token = peek(lexer->queue);
	A_Pos position = make_pos(current_token->char_pos, current_token->line_pos, parser->current_indentation_level);

	if (match(current_token, BREAK) == TRUE) {
		eat_token(lexer->queue);
		current_token = peek(lexer->queue);
		if (
		match(current_token, SEMI_COLON) == FALSE && 
		match(current_token, NEW_LINE) == FALSE && 
		(parser->current_stm != For_Stm  && parser->current_stm != While_Stm)) {
			report_error(
				SyntaxError,
				current_token->input, 
				current_token->line_pos,
				current_token->char_pos
			);
		}
	
		eat_token(lexer->queue);
                return make_break_stm(position);
	}
	else if (match(current_token, ID) == TRUE && match(current_token->next, ASSIGN) == TRUE) {
		string id = current_token->input;
		eat_token(lexer->queue);
		eat_token(lexer->queue);
		A_Exp val_exp = parse_expression(lexer, parser);
                return make_assign_stm(id, val_exp, position);
	}
	return make_expression_stm(parse_expression(lexer, parser));
}

A_Stm parse_program(Lexer lexer, Parser parser) {
        A_Stm root = NULL;
	while (match(peek(lexer->queue), END_OF_FILE) == FALSE) {
	   
	   if (match(peek(lexer->queue), NEW_LINE) == TRUE || 
	   match(peek(lexer->queue), TAB) == TRUE) {
	   	eat_token(lexer->queue);
	   	continue;
	   }

	   handle_indentation(lexer, parser);

	   A_Stm parsed_stm = parse_stm(lexer, parser);
	   parser->current_indentation_level = 0;
	   parser->current_stm = -1;

	   printf("\n Parsed Statement\n");
	   if (match(peek(lexer->queue), NEW_LINE) == TRUE || match(peek(lexer->queue), SEMI_COLON) == TRUE)
	   	eat_token(lexer->queue);

	   if (root == NULL)
	   	root = parsed_stm;
	   else
           	root = make_compound_stm(root, parsed_stm);

	}
	return root;

}
