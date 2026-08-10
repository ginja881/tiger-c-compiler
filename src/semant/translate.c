#include "semant/translate.h"


struct Tr_Level_ {
	Tr_Level parent;
	Frame frame;
};

static Tr_Level outermost_level = NULL;

Tr_AccessList Tr_make_access_list(Tr_Access tree_access, Tr_AccessList next) {
	Tr_AccessList access_list = (Tr_AccessList)checked_malloc(sizeof(struct Tr_AccessList_));
	access_list->tree_access = tree_access;
	access_list->next = next;

	return access_list;
}

Tr_Level Tr_outermost(void) {
	if (outermost_level == NULL) {
		outermost_level = (Tr_Level)checked_malloc(sizeof(struct  Tr_Level_));
		
		outermost_level->frame = new_frame(make_new_temporary_label(), NULL);
		outermost_level->parent = NULL;
	}
	

	return outermost_level;
}

Tr_Level Tr_new_level(Tr_Level parent, TempLabel label, BoolList parameters) {
	Tr_Level new_level = (Tr_Level)checked_malloc(sizeof(struct Tr_Level_));
	new_level->parent = parent;
	new_level->frame = new_frame(label, parameters);

	return new_level;
}
Tr_AccessList Tr_params(Tr_Level level) {
	if (level == NULL)
		return NULL;
	Tr_AccessList head = NULL;
	Tr_AccessList current = NULL;
	F_AccessList frame_access_list = frame_parameters(level->frame);

	while (frame_access_list != NULL) {
		F_Access frame_access = frame_access_list->access;
		Tr_Access tree_access = (Tr_Access)checked_malloc(sizeof(struct Tr_Access_));
		tree_access->level = level;
		tree_access->frame_access = frame_access;

		Tr_AccessList new_node = Tr_make_access_list(tree_access, NULL);

		if (head == NULL) {
			head = new_node;
			current = head;
		} else {
			current->next = new_node;
			current = current->next;
		}

		frame_access_list = frame_access_list->next;
	}
	return head;
}

Tr_Access Tr_allocLocal(Tr_Level level, bool escape) {
	F_Access access = frame_local_alloc(level->frame, escape);
	Tr_Access IR_access = (Tr_Access)checked_malloc(sizeof(struct Tr_Access_));
	IR_access->level = level;
	IR_access->frame_access = access;

	return IR_access;
}


static T_Cx Tr_CX(Tr_PatchList trues, Tr_PatchList falses, T_Stm stm) {
	T_Cx new_cx = (T_Cx)checked_malloc(sizeof(struct T_Cx_));
	new_cx->trues = trues;
	new_cx->falses = falses;
	new_cx->stm = stm;

	return new_cx;
};

static Tr_Exp Tr_CxExp(T_Cx conditional) {
	Tr_Exp new_cx_exp = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_cx_exp->kind = CX;
	new_cx_exp->u.cx = conditional;

	return new_exp;
};

static Tr_Exp Tr_NxExp(T_Stm statement) {
	Tr_Exp new_nx_exp = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_nx_exp->kind = NX;
	new_nx_exp->u.nx = statement;

	return new_nx_exp;
};

static Tr_Exp Tr_ExExp(T_Exp expression) {
	Tr_Exp new_ex_exp = (Tr_Exp)checked_malloc(sizeof(struct Tr_Exp_));
	new_ex_exp->kind = EX;
	new_ex_exp->u.ex = expression;

	return new_ex_exp;
};

static T_Cx unCx(Tr_Exp tr_exp) {
    switch (tr_exp->kind) {
    	case CX:
		return tr_exp->u.cx;
	case EX:
		return  
	default:
		return NULL;
		
    };

    return NULL;
};

static Tr_PatchList Tr_newPatchList(TempLabel label, Tr_PatchList next) {
	Tr_PatchList new_patchlist_node = (Tr_PatchList)checked_malloc(sizeof(struct Tr_PatchList_));
	new_patchlist_node->label = label;
	new_patchlist_node->next = next;

	return new_patchlist;
}



