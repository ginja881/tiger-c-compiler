#include "tree.h"

T_Exp Tr_make_constant(int constant) {
	T_Exp new_constant = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_constant->u.constant = constant;

	return new_constant;
}

T_Exp Tr_make_binop(T_Exp op1, Tr_Binop op, T_Exp op2) {
	T_Exp new_binop = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_binop->u.binop.op1 = op1;
	new_binop->u.binop.op2 = op2;
	new_binop->u.binop.op = op;

	return new_binop;
}

T_Exp Tr_make_name(TempLabel name) {
	T_Exp new_name = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_name->u.name = name;

	return new_name;
}

T_Exp Tr_make_call(T_Exp function_name, T_ExpList arguments) {
	T_Exp new_call = (T_Exp)checked_malloc(sizeof(struct T_Exp_));
	new_call->u.call_exp.function_name = function_name;
	new_call->u.call_exp.arguments = arguments;

	return new_call;
}

T_Exp Tr_make_mem(T_Exp mem) {
	T_Exp new_mem = (T_Exp) checked_malloc(sizeof(struct T_Exp_));
	new_mem->u.mem = mem;
	
	return new_mem;
}

T_Exp Tr_make_seq_exp(T_Stm statement, T_Exp next) {
	T_Exp new_seq_exp = (T_Exp) checked_malloc(sizeof(struct T_Exp_));
	new_seq_exp->u.seq_exp.statement = statement;
	new_seq_exp->u.seq_exp.next = next;

	return new_seq_exp;
}

T_ExpList Tr_make_explist(T_Exp tr_exp, T_Exp next) {
	T_ExpList new_explist = (T_ExpList) checked_malloc(sizeof(struct T_ExpList_));

	new_explist->tr_exp = tr_exp;
	new_explist->next = next;

	return new_explist;
}

T_Stm Tr_make_move_stm(T_Exp source, T_Exp result) {
	T_Stm new_move_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));
	new_move_stm->u.move.source = source;
	new_move_stm->u.move.result = result;

	return new_move_stm;
}

T_Stm Tr_make_exp_stm(T_Exp exp) {
	Tr_Stm new_exp_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));
	new_exp_stm->u.exp = exp;

	return new_exp_stm;
}

T_Stm Tr_make_jump(T_Exp destination, TempLabelList labels) {
	T_Stm new_jump_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_jump_stm->u.jump.destination = destination;
	new_jump_stm->u.jump.labels = labels;
	
	return new_jump_stm;
	
}

T_Stm Tr_make_cjump(Tr_Relop op, T_Exp left, T_Exp right, TempLabel true_dest, TempLabel false_dest) {
	T_Stm new_cjump_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_cjump_stm->u.cjump.op = op;
	new_cjump_stm->u.cjump.left = left;
	new_cjump_stm->u.cjump.right = right;
	new_cjump_stm->u.cjump.true_dest = true_dest;
	new_cjump_stm->u.cjump.false_dest = false_dest;

	return new_cjump_stm;

}

T_Stm Tr_make_label(TempLabel label) {
	T_Stm new_label_stm = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_label_stm->u.label = label;

	return new_label_stm;
}

T_Stm Tr_make_stm_seq(T_Stm stm, T_Stm next) {
	T_Stm new_stm_seq = (T_Stm)checked_malloc(sizeof(struct T_Stm_));

	new_stm_seq->u.stm_seq.stm = stm;
	new_stm_seq->u.stm_seq.next = next;

	return new_stm_seq;
}

T_StmList Tr_make_stmlist(T_Stm stm, T_StmList next) {
	T_StmList new_stmlist = (T_StmList)checked_malloc(sizeof(struct T_StmList_));
	new_stmlist->stm = stm;
	new_stmlist->next = next;

	return new_stmlist;
	
}
