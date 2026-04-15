#include "parser/ast.h"

// Constructors
Parser make_parser(void) {
      Parser new_parser = (Parser)checked_malloc(sizeof(struct Parser_));
      new_parser->AST_Root = NULL;
      new_parser->current_indentation_level = 0;
      new_parser->is_in_block = 0;
      return new_parser;
}

A_Pos make_pos(size_t col_pos, size_t line_pos, size_t indentation_level) {
	A_Pos position = (A_Pos)checked_malloc(sizeof(struct A_Pos_));
	position->col_pos = col_pos;
	position->line_pos = line_pos;
	position->indentation_level = indentation_level;
	return position;
}

A_Exp make_id_exp(string id, string type_id, A_Pos position) {
	A_Exp new_id_exp = (A_Exp)checked_malloc(sizeof(struct A_Exp_));
	new_id_exp->u.id_exp.id = strdup(id);
	new_id_exp->u.id_exp.type_id = strdup(type_id);
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
	calle_exp->position = position;

	callee_exp->u.callee_exp.function_name = strdup(function_name);
	callee_exp->u.callee_exp.args = args;

	return callee_exp;
}

A_Stm make_while_stm(A_Exp while_cond, A_Stm block, A_Pos position) {
	A_Stm new_while_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_while_stm->kind = While_Stm;
	new_while_stm->position = position;

	new_while_stm->u.while_stm.while_cond = while_cond;
	new_while_stm->u.while_stm.block = block;
	
	return new_while_stm;
}

A_Stm make_if_chain(int conditional_type, A_Exp cond, A_Stm block, A_Pos position) {
	A_Stm new_if_chain = (A_Stm)checked_malloc(sizeof(struct A_Stm_));

	new_if_chain->kind = conditional_type;
	new_if_chain->position = position;

	new_if_chain->u.if_chain.condition = cond;
	new_if_chain->u.if_chain.cond_block = block;
	new_if_chain->u.if_chain.next_cond = NULL;

	return new_if_chain;
}

A_Stm make_compound_stm(A_Stm stm1, A_Stm stm2) {
	A_Stm new_compound_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	
	new_compound_stm->kind = Compound_Stm;
	new_compound_stm->position = stm1->position;

	new_compoun_stm->u.compound_stm.stm1 = stm1;
	new_compound_stm->u.compound_stm.stm2 = stm2;

	return new_compound_stm;
}

A_Stm make_var_dec(string id, A_Exp var_val, A_Pos position) {
	A_Stm new_var_dec = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_var_dec->kind = Var_Dec;
	new_var_dec->position = position;

	new_var_dec->u.var_dec.id = strdup(id);
	new_var_dec->u.var_dec.var_val = var_val;

	return new_var_dec;
}

A_Stm make_function_dec(string id, A_Stm block, A_ExpList args, A_Pos position) {
	A_Stm function_dec = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	function_dec->kind = Func_Dec;
	function_dec->position = position;

	function_dec->u.function_dec.id = strdup(id);
	function_dec->u.function_dec.block = block;
	function_dec->u.function_dec.args = args;

	return function_dec;
}

A_Stm make_assign_stm(A_Exp id_exp, A_Exp exp) {
	A_Stm new_assign_stm = (A_Stm)checked_malloc(sizeof(struct A_Stm_));
	new_assign_stm->kind = Assign_Stm;
	new_assign_stm->position = id_exp->position;

	new_assign_stm->u.assign_stm.id_exp = id_exp;
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


A_Op match_op(token operation) {
      if (operation == ADD)
          return OP_ADD;
      else if (operation == SUB)
        return OP_SUB;
      else if (operation == MUL)
        return OP_MUL:
      else if (operation == MOD)
        return OP_MOD;
      else if (operation == DIV)
        return OP_DIV;
      else if (operation == NOT)
        return OP_NOT;
      else if (operation == MEMBER_REF)
        return OP_MEMBER_REF;
      else if (operation == BIT_AND)
        return OP_AND;
      else if (operation == BIT_OR)
        return OP_OR;
      else if (operation == GT)
        return OP_GT;
      else if (operation == GT_EQ)
        return OP_GT_EQ;
      else if (operation == LT)
        return OP_LT;
      else if (operation == LT_EQ)
        return OP_LT_EQ;
      else if (operation == COMPAR_AND)
        return OP_COMPAR_AND;
      else if (operation == COMPAR_OR)
        return OP_COMPAR_OR;
      else if (operation == COMPAR_EQ)
        return OP_COMPAR_EQ;
      else if (operation == COMPAR_NOT_EQ)
        return OP_COMPAR_NOT_EQ;
      else if (operation == INCREMENT)
      	return OP_INCREMENT;
      else if (operation == DECREMENT)
        return OP_DECREMENT;
      else if (operation == BIT_LSHIFT)
      	return  OP_LSHIFT;
      return OP_RSHIFT;
}
