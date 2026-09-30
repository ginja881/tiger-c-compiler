#include "semant/frame.h"

// In bytes

const int F_WordSize = 4;
static Temp frame_pointer = NULL;
static Temp stack_pointer = NULL;
static Temp return_register = NULL;

static F_CodeFragmentList fragments = NULL;
static TempLabel return_label = NULL;

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
	F_Access static_link_access;
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
	frame->static_link_access = frame_local_alloc(frame, true);


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
		frame->used_space -= F_WordSize;
		return make_local_access(frame->used_space, escape);
	}
	printf("\n VAR THAT DOES NOT ESCAPE\n");
	return make_register_access(frame->used_registers++, escape);
};

F_Access frame_static_link(Frame frame) {
	return frame->static_link_access;
}


// For enforcing encapsulation mainly.
bool does_escape(F_Access access) {
	return (access->kind == InFrame ? true : false); 
}

int get_register_num(F_Access access) {
	if (access->kind != InRegister)
		return -1;
	return access->u.register_num;
}

int get_frame_offset(F_Access access) {
	if (access->kind != InFrame)
		return -1;
	return access->u.offset;
}

Temp FP(void) {
	if (frame_pointer == NULL)
		frame_pointer = make_new_temporary();
	return 	frame_pointer;
}

bool frame_isFP(Temp temp) {
	return FP() == temp;
}

Temp SP(void) {
	if (stack_pointer == NULL)
		stack_pointer = make_new_temporary();
	return stack_pointer;
}

bool frame_isSP(Temp temp) {
	return SP() == temp;
}


Temp RV(void) {
	if (return_register == NULL)
		return_register = make_new_temporary();
	return return_register;
}

bool frame_isRV(Temp temp) {
	return RV() == temp;
}

TempLabel RL(void) {
	if (return_label == NULL)
		return_label = make_new_temporary_label();

	return return_label;
}

bool frame_isRL(TempLabel label) {
	return RL() == label;
}

T_Exp AddExternalCall(string named_label, T_ExpList args) {
	return Tr_make_call(Tr_make_name(make_new_temporary_namedlabel(named_label)), args);
}

F_CodeFragment frame_makeStringFragment(string text, TempLabel label) {
	F_CodeFragment string_fragment = (F_CodeFragment)checked_malloc(sizeof(struct F_CodeFragment_));
	string_fragment->kind = StringFrag;
	string_fragment->u.string_fragment.txt = text;
	string_fragment->u.string_fragment.label = label;

	return string_fragment;
}

F_CodeFragment frame_makeCodeFragment(Frame frame, T_Stm block) {
	F_CodeFragment procedure_fragment = (F_CodeFragment)checked_malloc(sizeof(struct F_CodeFragment_));
	procedure_fragment->kind = ProcFrag;
	procedure_fragment->u.procedure_fragment.frame = frame;
	procedure_fragment->u.procedure_fragment.block = block;

	return procedure_fragment;
}


void frame_makeFragmentList(F_CodeFragment frag) {
	F_CodeFragmentList new_fragment_list = (F_CodeFragmentList)checked_malloc(sizeof(struct F_CodeFragmentList_));

	new_fragment_list->fragment = frag;
	new_fragment_list->next = fragments;

	fragments = new_fragment_list;

}

void frame_ProcFragExit(Frame frame, T_Stm body) {
	frame_makeFragmentList(frame_makeCodeFragment(frame, body));
}

F_CodeFragmentList frame_ReturnFragmentList(void) {
	return fragments;
}
