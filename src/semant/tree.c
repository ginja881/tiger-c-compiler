#include "tree.h"

T_Exp Tr_make_constant(int constant) {
	T_Exp new_constant = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_constant->u.constant = constant;
	new_constant->kind = TR_CONST;
	return new_constant;
}

T_Exp Tr_make_binop(T_Exp op1, Tr_Binop op, T_Exp op2) {
	T_Exp new_binop = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_binop->kind = TR_BINOP;

	new_binop->u.binop.op1 = op1;
	new_binop->u.binop.op2 = op2;
	new_binop->u.binop.op = op;

	return new_binop;
}

T_Exp Tr_make_name(TempLabel name) {
	T_Exp new_name = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_name->u.name = name;
	new_name->kind = TR_NAME;

	return new_name;
}

T_Exp Tr_make_temp(Temp temp) {
	T_Exp new_temp = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_temp->u.temp = temp;
	new_temp->kind = TR_TEMP;

	return new_temp;
}

T_Exp Tr_make_call(T_Exp function_name, T_ExpList arguments) {
	T_Exp new_call = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_call->kind = TR_CALL;

	new_call->u.call_exp.function_name = function_name;
	new_call->u.call_exp.arguments = arguments;

	return new_call;
}

T_Exp Tr_make_mem(T_Exp memory, int size) {
	T_Exp new_mem = (T_Exp) checked_malloc(sizeof(struct T_Exp_));
	new_mem->kind = TR_MEM;
	new_mem->u.mem.memory = memory;
	new_mem->u.mem.size = size;
	
	return new_mem;
}

T_Exp Tr_make_seq_exp(T_Stm statement, T_Exp next) {
	T_Exp new_seq_exp = (T_Exp) checked_malloc(sizeof(struct T_Exp_));
	new_seq_exp->kind = TR_ESEQ;
	new_seq_exp->u.seq_exp.statement = statement;
	new_seq_exp->u.seq_exp.next = next;

	return new_seq_exp;
}


T_Exp Tr_make_char(char character) {
	T_Exp new_character_exp = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_character_exp->kind = TR_CHAR;
	new_character_exp->u.character = (int)character;

	return new_character_exp;
}

T_ExpList Tr_make_explist(T_Exp tr_exp, T_ExpList next) {
	T_ExpList new_explist = (T_ExpList) checked_malloc(sizeof(struct T_ExpList_));
	
	new_explist->tr_exp = tr_exp;
	new_explist->next = next;

	return new_explist;
}

T_Stm Tr_make_move_stm(T_Exp source, T_Exp result) {
	T_Stm new_move_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));
	new_move_stm->u.move.source = source;
	new_move_stm->u.move.result = result;
	new_move_stm->kind = TR_MOVE;

	return new_move_stm;
}

T_Stm Tr_make_exp_stm(T_Exp exp) {
	T_Stm new_exp_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));
	new_exp_stm->u.exp = exp;
	new_exp_stm->kind = TR_EXP;

	return new_exp_stm;
}

T_Stm Tr_make_jump(T_Exp destination, TempLabelList labels) {
	T_Stm new_jump_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_jump_stm->u.jump.destination = destination;
	new_jump_stm->u.jump.labels = labels;
	new_jump_stm->kind = TR_JUMP;

	return new_jump_stm;
	
}

T_Stm Tr_make_cjump(Tr_Relop op, T_Exp left, T_Exp right, TempLabel true_dest, TempLabel false_dest) {
	T_Stm new_cjump_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_cjump_stm->u.cjump.op = op;
	new_cjump_stm->u.cjump.left = left;
	new_cjump_stm->u.cjump.right = right;
	new_cjump_stm->u.cjump.true_dest = true_dest;
	new_cjump_stm->u.cjump.false_dest = false_dest;
	
	new_cjump_stm->kind = TR_CJUMP;

	return new_cjump_stm;

}

T_Stm Tr_make_label(TempLabel label) {
	T_Stm new_label_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_label_stm->u.label = label;
	new_label_stm->kind = TR_LABEL;

	return new_label_stm;
}

T_Stm Tr_make_stm_seq(T_Stm stm, T_Stm next) {
	T_Stm new_stm_seq = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_stm_seq->u.seq.stm = stm;
	new_stm_seq->u.seq.next = next;
	new_stm_seq->kind = TR_SEQ;

	return new_stm_seq;
}

T_StmList Tr_make_stmlist(T_Stm stm, T_StmList next) {
	T_StmList new_stmlist = (T_StmList)checked_malloc(sizeof(struct T_StmList_));
	new_stmlist->stm = stm;
	new_stmlist->next = next;

	return new_stmlist;
	
}


Tr_Binop match_tree_binop(A_Op binary_op) {
	#define X(ast_op, tree_op) \
		if (binary_op == ast_op) return tree_op;
		TREE_BASIC_OP_MATCH
	#undef X

	return INVALID_TREE_BINOP;
}

Tr_Relop match_tree_relop(A_Op relation_op) {
	#define X(ast_op, tree_op) \
		if (relation_op == ast_op) return tree_op;
		TREE_REL_OP_MATCH
	#undef X
	return INVALID_TREE_RELOP;
}
