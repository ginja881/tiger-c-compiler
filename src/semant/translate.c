#include "semant/translate.h"


struct Tr_Level_ {
	Tr_Level parent;

	Frame frame;
};

static Tr_Level outermost_level = NULL;
static TempLabel continue_label = NULL;
static TempLabel break_label = NULL;

void Tr_init(void) {
	continue_label = make_new_temporary_label();
	break_label = make_new_temporary_label();
}

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

IRList Tr_makeIRList(Tr_Exp tree_ir, IRList next) {
	IRList new_ir_node = (IRList)checked_malloc(sizeof(struct IRList_));
	new_ir_node->tree_ir = tree_ir;
	new_ir_node->next = next;

	return new_ir_node;
}

static Tr_PatchList Tr_makePatchList(TempLabel label, Tr_PatchList next) {
	Tr_PatchList new_patchlist = (Tr_PatchList)checked_malloc(sizeof(struct Tr_PatchList_));
	new_patchlist->label = label;
	new_patchlist->next = next;

	return new_patchlist;
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

	return new_cx_exp;
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

static T_Exp unEx(Tr_Exp tr_exp) {
	if (tr_exp == NULL)
		return Tr_make_constant(0);

	switch (tr_exp->kind) {	
		case EX:
			return tr_exp->u.ex;
		case NX: {
			T_Stm statement = tr_exp->u.nx;
			T_Exp new_sequence_head = NULL;
			T_Exp new_sequence_tail = NULL;
			if (statement->kind == TR_SEQ) {
				T_Stm current_sequence = statement;
				while (current_sequence->u.seq.next != NULL) {
					if (new_sequence_head == NULL) {
						new_sequence_head = Tr_make_seq_exp(
							current_sequence,
							NULL
						);
						new_sequence_tail = new_sequence_head;
					}
					else {
						new_sequence_tail->u.seq_exp.next = Tr_make_seq_exp(current_sequence, NULL);
						new_sequence_tail = new_sequence_tail->u.seq_exp.next;
					}

					current_sequence = current_sequence->u.seq.next;
				}
				if (new_sequence_tail == NULL)
					return unEx(Tr_NxExp(current_sequence->u.seq.stm));

				new_sequence_tail->u.seq_exp.next = unEx(Tr_NxExp(current_sequence->u.seq.stm));
				return new_sequence_head;
			}
			

			if (statement->kind == TR_EXP) {
				return statement->u.exp;
			}
			return Tr_make_seq_exp(statement, Tr_make_constant(0));
		}
		case CX: {
			Temp reg  = make_new_temporary();
			TempLabel false_lbl = make_new_temporary_label();
			TempLabel true_lbl = make_new_temporary_label();
			Tr_doPatch(tr_exp->u.cx->trues, true_lbl);
			Tr_doPatch(tr_exp->u.cx->falses, false_lbl);
			return Tr_make_seq_exp(Tr_make_move_stm(Tr_make_temp(reg), Tr_make_constant(1)),
				Tr_make_seq_exp(tr_exp->u.cx->stm,
					Tr_make_seq_exp(Tr_make_label(false_lbl),
						Tr_make_seq_exp(Tr_make_move_stm(Tr_make_temp(reg), Tr_make_constant(0)),
							Tr_make_seq_exp(Tr_make_label(true_lbl), Tr_make_temp(reg))
						)
					)
				)
			);
		}

	}
	return NULL;
}

static T_Stm unNx(Tr_Exp tr_exp) {
	if (tr_exp == NULL)
		return Tr_make_exp_stm(Tr_make_constant(0));

	switch (tr_exp->kind) {
		case EX: { 
			T_Exp expression = tr_exp->u.ex;
			T_Stm new_sequence_head = NULL;
			T_Stm new_sequence_tail = NULL;

			if (expression->kind == TR_ESEQ) {
				T_Exp current_sequence = expression;

				while (current_sequence != NULL) {
					T_Stm current_stm = current_sequence->u.seq_exp.statement;
					

					if (new_sequence_head == NULL) {
						new_sequence_head = Tr_make_stm_seq(current_stm, NULL);
						new_sequence_tail = new_sequence_head;
					}
					else {
						new_sequence_tail->u.seq.next = Tr_make_stm_seq(current_stm, NULL);
						new_sequence_tail = new_sequence_tail->u.seq.next;
					}

					current_sequence = current_sequence->u.seq_exp.next;

				}

				return new_sequence_head;
				
			}
				
			return Tr_make_exp_stm(expression);
		};
		case NX: return tr_exp->u.nx;
		case CX: return tr_exp->u.cx->stm;
	}

	return NULL;
}

static T_Cx unCx(Tr_Exp tr_exp) {
	if (tr_exp == NULL)
		return Tr_CX(NULL, NULL, Tr_make_exp_stm(Tr_make_constant(0)));

	switch (tr_exp->kind) {
		case NX: return NULL;
		case CX: return tr_exp->u.cx;
		case EX: {
			T_Exp expression = tr_exp->u.ex;
			
			if (expression->kind == TR_CONST) {
				int val = expression->u.constant;
				
				if (val != 0)
					return Tr_CX(Tr_makePatchList(make_new_temporary_label(), NULL), NULL, NULL);
				else
					return Tr_CX(NULL, Tr_makePatchList(make_new_temporary_label(), NULL), NULL); 
			}

			return Tr_CX(Tr_makePatchList(make_new_temporary_label(), NULL), 
			Tr_makePatchList(make_new_temporary_label(), NULL), Tr_make_exp_stm(expression));
		} 
	}
	return NULL;
}



// Patch API
void Tr_doPatch(Tr_PatchList patch_list, TempLabel label) {
	if (patch_list == NULL)
		return;
	
	for (; patch_list; patch_list = patch_list->next)
		patch_list->label = label;
}

Tr_PatchList Tr_joinPatch(Tr_PatchList first, Tr_PatchList second) {
	if (first == NULL)
		return second;
	
	Tr_PatchList tail = first;

	for (; tail->next; tail = tail->next);
	tail->next = second;
	return first;
}

// IR List API


void append_node(IRList node, IRList* head, IRList* tail) {
	if (*(head) == NULL) {
		(*head) = node;
		(*head)->next = NULL;
		(*tail) = (*head);
	} else {
		(*tail)->next = node;
		(*tail) = (*tail)->next;
	}

}

// Literals
Tr_Exp Tr_translateInteger(int integer) {
	return Tr_ExExp(Tr_make_constant(integer));
}

Tr_Exp Tr_translateCharacter(char character) {
	return Tr_ExExp(Tr_make_char(character));
}

Tr_Exp Tr_lowerSequence(IRList sequence) {
	T_Stm new_seq_head = NULL;
	T_Stm new_seq_tail = NULL;
	IRList current_node = sequence;
	while (current_node != NULL) {
		if (new_seq_head == NULL) {
			new_seq_head = Tr_make_stm_seq(unNx(current_node->tree_ir), NULL);
			new_seq_tail = new_seq_head;
		}
		else {
			new_seq_tail->u.seq.next = Tr_make_stm_seq(unNx(current_node->tree_ir), NULL);
			new_seq_tail = new_seq_tail->u.seq.next;
		}
		current_node = current_node->next;
	}
	return Tr_NxExp(new_seq_head);
}

// Referencing Simple and Field Variables
Tr_Exp Tr_translateVariableAccess(Tr_Access variable_access, Tr_Level level, int size) {
	if (!does_escape(variable_access->frame_access))	
		return Tr_ExExp(Tr_make_temp(make_new_temporary()));
	

	// If variable is the outermost level, then just grab from the stack pointer
	if (variable_access->level == Tr_outermost()) {
		return Tr_ExExp(
			Tr_make_mem(
				Tr_make_binop(
					Tr_make_temp(SP()),
					TR_PLUS,
					Tr_make_constant(get_frame_offset(variable_access->frame_access))
				),
				size
			)
		);
	}
	
	Tr_Level current_level = level;
	

	// If variable exists in some function, then just grab  from the associated frame pointer via computing the stack link offset shown below
	T_Exp static_chain_head = Tr_make_mem(Tr_make_binop(
		Tr_make_temp(FP()),
		TR_PLUS,
		0
	), F_WordSize);

	T_Exp static_chain = static_chain_head;
	Tr_Level target_level = variable_access->level;
	while (current_level != target_level) {
			
		static_chain->u.binop.op2 = Tr_make_constant(get_frame_offset(frame_static_link(current_level->frame)));
		static_chain = Tr_make_mem(Tr_make_binop(
			static_chain,
			TR_PLUS,
			0
		), F_WordSize);

		current_level = current_level->parent;
	}
	static_chain->u.mem.memory->u.binop.op2 = Tr_make_constant(get_frame_offset(variable_access->frame_access));
	static_chain->u.mem.size = size;

	return Tr_ExExp(static_chain_head);


}
 
Tr_Exp Tr_translateAssignment(Tr_Exp lvalue, Tr_Exp rvalue) {
	return Tr_NxExp(Tr_make_move_stm(unEx(lvalue), unEx(rvalue)));
}
// Lowering Basic Binary Operations to IR
Tr_Exp Tr_translateBasicOperation(Tr_Exp operand1, Tr_Exp operand2, A_Op operation) {
	if (operand1 == NULL || operand2 == NULL)
		return NULL;

	Tr_Binop binary_operation = match_tree_binop(operation);

	return Tr_ExExp(Tr_make_binop(unEx(operand1), binary_operation, unEx(operand2))); 
}

// Lowering Unary Operations to IR
Tr_Exp Tr_translateUnaryOperation(Tr_Exp operand, A_Op unary_op) {
	if (operand == NULL)
		return NULL;
	
	switch	(unary_op) {
		case OP_SUB : return Tr_ExExp(Tr_make_binop(Tr_make_constant(0), TR_SUB, unEx(operand)));
		case OP_NOT: return Tr_ExExp(Tr_make_binop(unEx(operand), TR_XOR, Tr_make_constant(1)));
		case OP_INCREMENT: return Tr_ExExp(Tr_make_binop(unEx(operand), TR_PLUS, Tr_make_constant(1)));
		case OP_DECREMENT: return Tr_ExExp(Tr_make_binop(unEx(operand), TR_SUB, Tr_make_constant(1)));
		default: return NULL;
 	}
	return NULL;
}

Tr_Exp Tr_translateComparison(Tr_Exp operand1, A_Op comparison, Tr_Exp operand2) {
	TempLabel true_label = make_new_temporary_label();
	TempLabel false_label = make_new_temporary_label();

	T_Stm conditional_jump = Tr_make_cjump(match_tree_relop(comparison), unEx(operand1), unEx(operand2), true_label, false_label);

	return Tr_CxExp(
		Tr_CX(
			Tr_makePatchList(true_label, NULL),
			Tr_makePatchList(false_label, NULL),
			conditional_jump
		)
	);
}

// Translating Arrays & Records
Tr_Exp Tr_translateArray(Tr_Exp default_value, int element_size, Tr_Exp array_size) {
	return Tr_ExExp(AddExternalCall("initArray", Tr_make_explist(unEx(default_value), 
	Tr_make_explist(Tr_make_constant(element_size), Tr_make_explist(unEx(array_size), NULL)))));
}


Tr_Exp Tr_translateArrayAccess(Tr_Exp array_id, Tr_Exp index_ir, int element_type_size) {
	T_Exp array_id_exp = unEx(array_id);
	T_Exp index = unEx(index_ir);

	T_Exp length_temporary = Tr_make_temp(make_new_temporary());
	T_Stm move_length_into_temporary = Tr_make_move_stm(
	length_temporary,
	Tr_make_mem(Tr_make_binop(
		unEx(array_id),
		TR_PLUS,
		Tr_make_constant(4)
	), 4));

	TempLabel bound_error_label = make_new_temporary_label();

	// If bounds have been succeeded, report runtime error with out of bounds error (hard coded to 0 for now until runtime dev)
	T_Stm bound_error = Tr_make_stm_seq(Tr_make_label(bound_error_label), Tr_make_stm_seq(
		Tr_make_exp_stm(AddExternalCall(
			"runtime_error",
			Tr_make_explist(Tr_make_constant(0), NULL)
		)), 
		NULL
	));
	TempLabel success_label = make_new_temporary_label();
	T_Exp result_temporary= Tr_make_temp(make_new_temporary());

	T_Exp address_computation = Tr_make_binop(
		Tr_make_binop(
			index,	
			TR_MUL,
			Tr_make_constant(element_type_size)
		),
		TR_PLUS,
		Tr_make_binop(
			array_id_exp,
			TR_PLUS,
			Tr_make_constant(4)
		)
	);
	T_Stm no_bound_error = Tr_make_stm_seq(Tr_make_label(success_label), Tr_make_stm_seq(
		Tr_make_move_stm(
			Tr_make_mem(
				address_computation,
				element_type_size
			),
			result_temporary
		),
		NULL
	));

	T_Stm bound_condition = Tr_make_cjump(TR_ULT, index, length_temporary, success_label, bound_error_label);

	return Tr_ExExp(Tr_make_seq_exp(move_length_into_temporary,
		Tr_make_seq_exp(bound_condition, 
			Tr_make_seq_exp(Tr_make_label(success_label), Tr_make_seq_exp(
				no_bound_error, Tr_make_seq_exp(
					Tr_make_label(bound_error_label), Tr_make_seq_exp(
						bound_error, NULL
					))
				)
			)))); 
}

Tr_Exp Tr_translateRecord(IRList field_assignments, int length) {
	T_Exp call = AddExternalCall("malloc", Tr_make_explist(Tr_make_constant(length * F_WordSize), NULL));

	T_Exp record_temporary = Tr_make_temp(make_new_temporary());
	T_Stm move_address_into_temporary =  Tr_make_move_stm(
		record_temporary,
		call
	);

	int field_pos = 0;

	IRList current_assignment = field_assignments;
	T_Stm field_sequence_head = NULL;
	T_Stm field_sequence_tail = NULL;
	while (current_assignment != NULL) {
		T_Exp current_assignment_value = unEx(current_assignment->tree_ir);
		T_Exp compute_field_address = Tr_make_mem(
			Tr_make_binop(
				record_temporary,
				TR_PLUS,
				Tr_make_constant(field_pos * F_WordSize)
			),
			F_WordSize
		);
		T_Stm move_value_to_address = Tr_make_move_stm(
			compute_field_address,
			current_assignment_value
		);
		
		if (field_sequence_head == NULL) {
			field_sequence_head = Tr_make_stm_seq(move_value_to_address, NULL);
			field_sequence_tail = field_sequence_head;
		}
		else {
			field_sequence_tail->u.seq.next = Tr_make_stm_seq(move_value_to_address, NULL);
			field_sequence_tail = field_sequence_tail->u.seq.next;
		}

		current_assignment = current_assignment->next;
	}

	return Tr_ExExp(
		Tr_make_seq_exp(
			Tr_make_stm_seq(move_address_into_temporary, field_sequence_head),
			record_temporary
		)
	);


}

Tr_Exp Tr_translateFieldReference(Tr_Exp record_id, int field_pos) {
	return Tr_ExExp(
		Tr_make_mem(Tr_make_binop(
			unEx(record_id),
			TR_PLUS,
			Tr_make_constant(field_pos * F_WordSize)
		),
		F_WordSize
	));
}

// Control Flow

Tr_Exp Tr_translateBreak(void) {
	return Tr_NxExp(Tr_make_jump(
		Tr_make_name(break_label),
		make_templabel_list(break_label, NULL)
	));
}

Tr_Exp Tr_translateContinue(void) {
	return Tr_NxExp(Tr_make_jump(
		Tr_make_name(continue_label), 
		make_templabel_list(continue_label ,NULL)
	));
}

Tr_Exp Tr_translateWhileLoop(Tr_Exp condition, Tr_Exp block) {
	
	TempLabel iterating = make_new_temporary_label();
	T_Exp iterating_name = Tr_make_name(iterating);
	TempLabel done = make_new_temporary_label();
	T_Exp done_name = Tr_make_name(done);
	
	T_Cx cx_condition = unCx(condition);

	Tr_doPatch(cx_condition->trues, iterating);
	Tr_doPatch(cx_condition->falses, done);
	
	T_Stm while_condition = unNx(condition);
	

	T_Stm while_loop = Tr_make_stm_seq(
		Tr_make_label(iterating), 
		while_condition
	);
	
	

	T_Stm current_sequence_node = while_loop->u.seq.next;
	T_Stm actual_block = unNx(block);
	T_Stm block_statement = actual_block;

	while (block_statement != NULL && block_statement->kind == TR_SEQ) {
		block_statement = block_statement->u.seq.stm;

		if (block_statement->kind == TR_JUMP) {
			TempLabel destination = block_statement->u.jump.destination->u.name;
			TempLabel chosen_label = destination;

			if (destination == break_label) {
				chosen_label = break_label;
				block_statement->u.jump.labels->temp_label = chosen_label;
			}
			else if (destination == continue_label) {
				chosen_label = continue_label;
				block_statement->u.jump.labels->temp_label = chosen_label;
			}

			block_statement->u.jump.destination->u.name = chosen_label;
			
		}
		block_statement = block_statement->u.seq.next;
	}

	while_loop = Tr_make_stm_seq(while_loop, actual_block);

	while_loop = Tr_make_stm_seq(while_loop, Tr_make_label(done));

	return Tr_NxExp(while_loop);
}



Tr_Exp Tr_translateIfStatement(Tr_Exp condition, Tr_Exp then_block, Tr_Exp else_block) {
	TempLabel true_branch = make_new_temporary_label();
	TempLabel join_branch = make_new_temporary_label();
	TempLabel false_branch = else_block != NULL ? make_new_temporary_label() : join_branch; 
	Temp result_temporary = NULL;
	
	T_Cx cx_condition = unCx(condition);
	Tr_doPatch(cx_condition->trues, true_branch);
	Tr_doPatch(cx_condition->falses, false_branch);

	T_Stm main_condition = unNx(condition);

	T_Stm then_seq = unNx(then_block);
	T_Stm seq_head = then_seq;
	// Obtain expression
	
	while (then_seq != NULL) {
		if (then_seq->kind != TR_SEQ)
			break;

		if (then_seq->u.seq.next == NULL && then_seq->u.seq.stm->kind == TR_EXP) {
			T_Exp statement_exp = then_seq->u.seq.stm->u.exp;
			result_temporary = make_new_temporary();

			then_seq->u.seq.stm = Tr_make_move_stm(Tr_make_temp(result_temporary), statement_exp);
		}
		
		then_seq = then_seq->u.seq.next;

	} 	
	then_seq = seq_head;
	T_Stm else_seq = NULL;
	if (else_block != NULL) {
		else_seq = unNx(else_block);
		seq_head = else_seq;
		T_Stm prev_node = seq_head;
		while (else_seq != NULL) {
			if (else_seq->kind != TR_SEQ)	
				break;

			if ((else_seq->u.seq.next == NULL && else_seq->u.seq.stm->kind == TR_JUMP) && 
			(prev_node->u.seq.stm->kind == TR_EXP)) {
				T_Exp statement_exp = prev_node->u.seq.stm->u.exp;
				prev_node->u.seq.stm = Tr_make_move_stm(Tr_make_temp(result_temporary), statement_exp);
				
				else_seq->u.seq.stm->u.jump.destination->u.name = join_branch;
				else_seq->u.seq.stm->u.jump.labels->temp_label = join_branch;

			}
			prev_node = else_seq;
			else_seq = else_seq->u.seq.next;
		}
		else_seq = seq_head;
	}

	
	T_Stm main_if_statement = main_condition;
	
	if (else_block == NULL) {
		main_if_statement = Tr_make_stm_seq(
			main_if_statement,
			Tr_make_stm_seq(
				Tr_make_label(true_branch),
				Tr_make_stm_seq(
					then_seq,
					Tr_make_stm_seq(
						Tr_make_label(false_branch),
						NULL
					)
				)
			)
		);
	}
	else {
		main_if_statement = Tr_make_stm_seq(
			main_if_statement,
			Tr_make_stm_seq(
				Tr_make_label(true_branch),
				Tr_make_stm_seq(
					then_seq,
					Tr_make_stm_seq(
						Tr_make_label(false_branch),
						Tr_make_stm_seq(
							else_seq,
							Tr_make_stm_seq(
								Tr_make_label(join_branch),
								NULL
							)
						)
					)
				)
			)
		);
	}

	// Still in progress
	
	if (result_temporary != NULL) {
		return Tr_ExExp(
			Tr_make_seq_exp(
				main_if_statement,
				Tr_make_temp(result_temporary)
			)
		);
	}

	return Tr_NxExp(main_if_statement);
	
}

Tr_Exp Tr_translateForLoop(Tr_Exp low_access, Tr_Exp low_value, Tr_Exp limit, Tr_Exp block) {
	T_Stm low_move = Tr_make_move_stm(
		unEx(low_access),
		unEx(low_value)
	);

	TempLabel iterating_label = make_new_temporary_label();
	TempLabel done_label = make_new_temporary_label();
	T_Exp iterating_name = Tr_make_name(iterating_label);
	T_Exp done_name = Tr_make_name(done_label);
	
	T_Exp low_access_exp = unEx(low_access);
	T_Exp limit_exp = unEx(limit);

	T_Stm conditional_jump = Tr_make_cjump(TR_ULE, low_access_exp, limit_exp, iterating_label, done_label);
	
	T_Stm for_loop = Tr_make_stm_seq(Tr_make_label(iterating_label), conditional_jump);

	T_Stm actual_block = unNx(block);
	T_Stm block_statement = actual_block;

	while (block_statement != NULL) {
		block_statement = block_statement->u.seq.stm;

		if (block_statement->kind == TR_JUMP) {
			TempLabel destination = block_statement->u.jump.destination->u.name;
			TempLabel chosen_label = destination;

			if (destination == break_label) {
				chosen_label = break_label;
				block_statement->u.jump.labels->temp_label = chosen_label;
			}
			else if (destination == continue_label) {
				chosen_label = continue_label;
				block_statement->u.jump.labels->temp_label = chosen_label;
			}
			block_statement->u.jump.destination->u.name = chosen_label;
		}
		block_statement = block_statement->u.seq.next;
	}

	actual_block = Tr_make_stm_seq(actual_block, Tr_make_move_stm(
		low_access_exp, 
		Tr_make_binop(
			low_access_exp,
			TR_PLUS,
			Tr_make_constant(1)
	)));

	actual_block = Tr_make_stm_seq(actual_block, Tr_make_jump(
		iterating_name,
		make_templabel_list(iterating_label, NULL)
	));

	actual_block = Tr_make_stm_seq(actual_block, Tr_make_label(done_label));
	
	for_loop = Tr_make_stm_seq(for_loop, actual_block);

	return Tr_NxExp(for_loop);
}

Tr_Exp Tr_translateCallee(TempLabel flbl, Tr_Level current_level, Tr_Level function_level, IRList arguments) {
	T_ExpList arguments_head = NULL;
	T_ExpList arguments_tail = NULL;
	T_Exp label = Tr_make_name(flbl);
	

	IRList current_argument = arguments;
	
	while (current_argument != NULL) {
		T_Exp current_argument_exp = unEx(current_argument->tree_ir);

		if (arguments_head == NULL) {
			arguments_head = Tr_make_explist(current_argument_exp, NULL);
			arguments_tail = arguments_head; 
		}
		else {
			arguments_tail->next = Tr_make_explist(current_argument_exp, NULL);
			arguments_tail = arguments_tail->next;
		}

		current_argument = current_argument->next;
	}
	
	T_Exp static_access = Tr_make_mem(
		Tr_make_binop(
			Tr_make_temp(FP()),
			TR_PLUS,
			0
		),
		F_WordSize
	);
	T_Exp current_access = static_access;
	Tr_Level level = current_level;

	while (level != NULL && level != function_level) {
		current_access->u.binop.op2 = Tr_make_constant(get_frame_offset(frame_static_link(level->frame)));
		current_access = Tr_make_mem(
			Tr_make_binop(
				current_access,
				TR_PLUS, 0
			),
			F_WordSize
		);

		level = level->parent;
	}

	
	arguments_head = Tr_make_explist(static_access, arguments_head);

	return Tr_ExExp(Tr_make_call(Tr_make_name(flbl), arguments_head));
}


Tr_Exp Tr_translateLetExp(IRList variable_assignments, Tr_Exp let_block) {
	IRList current_assignment = variable_assignments;
	
	T_Stm moves_head = NULL;
	T_Stm moves_tail = NULL;

	T_Exp block = unEx(let_block);

	while (current_assignment != NULL) {
		if (moves_head == NULL) {
			moves_head = Tr_make_stm_seq(unNx(current_assignment->tree_ir), NULL);
			moves_tail = moves_head;
		}
		else {
			moves_tail->u.seq.next = Tr_make_stm_seq(unNx(current_assignment->tree_ir), NULL);
			moves_tail = moves_tail->u.seq.next;
		}

		current_assignment = current_assignment->next;
	}

	return Tr_ExExp(Tr_make_seq_exp(moves_head, block));
}

void Tr_makeFunction(Tr_Level current_level, Tr_Exp body, Tr_AccessList parameters) {
	T_Stm setup_formals_head = NULL;
	T_Stm setup_formals_tail = NULL;
	
	Tr_AccessList current_parameter = parameters;
	while (current_parameter != NULL) {
		Tr_Access parameter = current_parameter->tree_access;
		T_Stm variable_access = unNx(Tr_translateVariableAccess(parameter, current_level, F_WordSize));
		
		if (setup_formals_head == NULL) {
			setup_formals_head = Tr_make_stm_seq(variable_access, NULL);
			setup_formals_tail = setup_formals_head;
		}
		else {
			setup_formals_tail->u.seq.next = Tr_make_stm_seq(variable_access, NULL);
			setup_formals_tail = setup_formals_tail->u.seq.next;
		}

		current_parameter = current_parameter->next;
	}

	T_Exp function = unEx(body);
	function = Tr_make_seq_exp(setup_formals_head, function);

	T_Stm main_function = Tr_make_move_stm(Tr_make_temp(RV()), function);
	main_function = Tr_make_stm_seq(
		main_function, 
		Tr_make_jump(
			Tr_make_name(RL()),
			make_templabel_list(RL(), NULL)
		)
	);

	
	frame_ProcFragExit(current_level->frame, main_function);
}


Tr_Exp Tr_translateString(string text) {
	TempLabel string_label = make_new_temporary_label();
	frame_makeFragmentList(frame_makeStringFragment(text, string_label));

	return Tr_ExExp(Tr_make_name(string_label));
}
