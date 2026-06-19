#include "semant/frame.h"

struct Access_ {
	bool escape;
	enum {InRegister, InFrame} kind;
	union {
		int offset;
		int register_num;
	} u;
};

struct Frame_ {
	AccessList params;
	AccessList locals;
	TempLabel frame_name;
	int used_space;
	int used_registers;
};

static Access make_register_access(int register_num) {
	(Access) new_access = (Access)checked_malloc(sizeof(struct Access_));
	new_access->u.register_num = register_num;
	new_access->kind = InRegister;
	new_access->escape = escape;

	return new_access;
}

static Access make_local_access(int offset, bool escape) {
	(Access) new_access = (Access)checked_malloc(sizeof(struct Access_));
	new_access->u.offset = offset;
	new_access->escape = escape;
	new_access->kind = InFrame;

	return new_access;
}


Frame new_frame(TempLabel frame_name, AcessList params, AccessList locals) {
	Frame frame = (Frame)checked_malloc(sizeof(struct Frame_));
	frame->params = params;
	frame->locals = locals;
	frame->frame_name = frame_name;
	frame->used_space = 0;
	frame->used_registers = 0;
	return frame;
}

Frame frame_static_link(Frame frame) {
	if (frame == NULL)
		return NULL;
	return frame->static_link;
}

TempLabel frame_name(Frame frame) {
	if (frame == NULL)
		return NULL;
	return frame->frame_name;
}
AccessList frame_parameters(Frame frame) {
	if (frame == NULL)
		return NULL;
	return frame->params;
}
Access local_alloc(Frame frame, bool escape) {
	if (escape) {
		frame->offset -= WORD_SIZE;
		return make_local_access(frame->offset, escape);
	}
	return make_register_access(frame->used_registers++, escape);
};
