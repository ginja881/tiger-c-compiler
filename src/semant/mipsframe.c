#include "semant/frame.h"

struct F_Access_ {
	bool escape;
	enum {InRegister, InFrame} kind;
	union {
		int offset;
		int register_num;
	} u;
};

struct Frame_ {
	F_AccessList params;
	F_AccessList locals;
	TempLabel frame_name;
	int used_space;
	int used_registers;
};

static F_Access make_register_access(int register_num, bool escape) {
	F_Access new_access = (F_Access)checked_malloc(sizeof(struct F_Access_));
	new_access->u.register_num = register_num;
	new_access->kind = InRegister;
	new_access->escape = escape;

	return new_access;
}

static F_Access make_local_access(int offset, bool escape) {
	F_Access new_access = (F_Access)checked_malloc(sizeof(struct F_Access_));
	new_access->u.offset = offset;
	new_access->escape = escape;
	new_access->kind = InFrame;

	return new_access;
}


Frame new_frame(TempLabel frame_name, BoolList params) {
	

	F_AccessList frame_parameters = NULL;
	F_AccessList current_frame_param = NULL;	

	Frame frame = (Frame)checked_malloc(sizeof(struct Frame_));
	
	frame->frame_name = frame_name;
	frame->used_space = 0;
	frame->used_registers = 0;


	BoolList current_param = params;

	while (current_param != NULL) {

		F_Access f_access = frame_local_alloc(frame, current_param->BOOL);
		F_AccessList new_access = checked_malloc(sizeof(struct F_AccessList_));
		new_access->access = f_access;
		if (frame_parameters == NULL) {
			frame_parameters = new_access;
			current_frame_param = frame_parameters;
		}
		else {
			current_frame_param->next = new_access;
			current_frame_param = current_frame_param->next;
		}

		current_param = current_param->next;
	}

	frame->params = frame_parameters;
	

	return frame;
}


TempLabel frame_name(Frame frame) {
	if (frame == NULL)
		return NULL;
	return frame->frame_name;
}
F_AccessList frame_parameters(Frame frame) {
	if (frame == NULL)
		return NULL;
	return frame->params;
}
F_Access frame_local_alloc(Frame frame, bool escape) {
	if (escape) {
		printf("\n ESCAPE FOUND\n");
		frame->used_space -= WORD_SIZE;
		return make_local_access(frame->used_space, escape);
	}
	printf("\n VAR THAT DOES NOT ESCAPE\n");
	return make_register_access(frame->used_registers++, escape);
};
