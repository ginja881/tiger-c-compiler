#include "tree.h"

Tr_Exp Tr_make_constant(int constant) {
	Tr_Exp new_constant = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_constant->u.constant = constant;

	return new_constant;
}

Tr_Exp Tr_make_binop(Tr_Exp op1, Tr_Binop op, Tr_Exp op2) {
	Tr_Exp new_binop = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_binop->u.binop.op1 = op1;
	new_binop->u.binop.op2 = op2;
	new_binop->u.binop.op = op;

	return new_binop;
}

Tr_Exp Tr_make_name(TempLabel name) {
	Tr_Exp new_name = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_name->u.name = name;

	return new_name;
}

Tr_Exp Tr_make_call(Tr_Exp function_name, Tr_ExpList arguments) {
	Tr_Exp new_call = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_call->u.call_exp.function_name = function_name;
	new_call->u.call_exp.arguments = arguments;

	return new_call;
}

Tr_Exp Tr_make_mem(Tr_Exp mem) {
	Tr_Exp new_mem = (Tr_Exp) checked_malloc(sizeof(struct Tr_Exp_));
	new_mem->u.mem = mem;
	
	return new_mem;
}

Tr_Exp Tr_make_seq_exp(Tr_Stm statement, Tr_Exp next) {
	Tr_Exp new_seq_exp = (Tr_Exp) checked_malloc(sizeof(struct Tr_Exp_));
	new_seq_exp->u.seq_exp.statement = statement;
	new_seq_exp->u.seq_exp.next = next;

	return new_seq_exp;
}

Tr_ExpList Tr_make_explist(Tr_Exp tr_exp, Tr_Exp next) {
	Tr_ExpList new_explist = (Tr_ExpList) checked_malloc(sizeof(struct Tr_ExpList_));

	new_explist->tr_exp = tr_exp;
	new_explist->next = next;

	return new_explist;
}

Tr_Stm Tr_make_move_stm(Tr_Exp source, Tr_Exp result) {
	Tr_Stm new_move_stm = (Tr_Stm)checked_malloc(sizeof(struct Tr_Stm_));
	new_move_stm->u.move.source = source;
	new_move_stm->u.move.result = result;

	return new_move_stm;
}

Tr_Stm Tr_make_exp_stm(Tr_Exp exp) {
	Tr_Stm new_exp_stm = (Tr_Stm)checked_malloc(sizeof(struct Tr_Stm_));
	new_exp_stm->u.exp = exp;

	return new_exp_stm;
}

Tr_Stm Tr_make_jump(Tr_Exp destination, TempLabelList labels) {
	Tr_Stm new_jump_stm = (Tr_Stm)checked_malloc(sizeof(struct Tr_Stm_));

	new_jump_stm->u.jump.destination = destination;
	new_jump_stm->u.jump.labels = labels;
	
	return new_jump_stm;
	
}

Tr_Stm Tr_make_cjump(Tr_Relop op, Tr_Exp left, Tr_Exp right, TempLabel true_dest, TempLabel false_dest) {
	Tr_Stm new_cjump_stm = (Tr_Stm)checked_malloc(sizeof(struct Tr_Stm_));

	new_cjump_stm->u.cjump.op = op;
	new_cjump_stm->u.cjump.left = left;
	new_cjump_stm->u.cjump.right = right;
	new_cjump_stm->u.cjump.true_dest = true_dest;
	new_cjump_stm->u.cjump.false_dest = false_dest;

	return new_cjump_stm;

}

Tr_Stm Tr_make_label(TempLabel label) {
	Tr_Stm new_label_stm = (Tr_Stm)checked_malloc(sizeof(struct Tr_Stm_));

	new_label_stm->u.label = label;

	return new_label_stm;
}

Tr_Stm Tr_make_stm_seq(Tr_Stm stm, Tr_Stm next) {
	Tr_Stm new_stm_seq = (Tr_Stm)checked_malloc(sizeof(struct Tr_Stm_));

	new_stm_seq->u.stm_seq.stm = stm;
	new_stm_seq->u.stm_seq.next = next;

	return new_stm_seq;
}

Tr_StmList Tr_make_stmlist(Tr_Stm stm, Tr_StmList next) {
	Tr_StmList new_stmlist = (Tr_StmList)checked_malloc(sizeof(struct Tr_StmList_));
	new_stmlist->stm = stm;
	new_stmlist->next = next;

	return new_stmlist;
	
}
