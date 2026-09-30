#pragma once

#ifndef _FRAME_H_
#define _FRAME_H_

#include "util.h"
#include "semant/temp.h"
#include "semant/tree.h"

typedef struct Frame_* Frame;
typedef struct F_Access_* F_Access;
typedef struct F_AccessList_* F_AccessList;
typedef struct F_CodeFragment_* F_CodeFragment;
typedef struct F_CodeFragmentList_* F_CodeFragmentList;

struct F_AccessList_ {
	F_Access access;
	F_AccessList next;
};


struct F_CodeFragment_ {
	enum {StringFrag, ProcFrag} kind;
	union {
		struct {string txt; TempLabel label;} string_fragment;
		struct {Frame frame; T_Stm block;} procedure_fragment;
	} u;
};

struct F_CodeFragmentList_ {
	F_CodeFragment fragment;
	F_CodeFragmentList next;
};

Frame new_frame(TempLabel frame_name, BoolList params);
TempLabel frame_name(Frame frame);
F_AccessList frame_parameters(Frame frame);
F_Access frame_local_alloc(Frame frame, bool escape);
F_Access frame_static_link(Frame frame);

bool does_escape(F_Access access);
int get_register_num(F_Access access);
int get_frame_offset(F_Access access);

Temp FP(void);
bool frame_isFP(Temp temp);

Temp SP(void);
bool frame_isSP(Temp temp);

Temp RV(void);
bool frame_isRV(Temp temp);

TempLabel RL(void);
bool frame_isRL(TempLabel label);

T_Exp AddExternalCall(string named_label, T_ExpList args);

F_CodeFragment frame_makeStringFragment(string text, TempLabel label);
F_CodeFragment frame_makeCodeFragment(Frame frame, T_Stm block);
void frame_makeFragmentList(F_CodeFragment frag);
void frame_ProcFragExit(Frame frame, T_Stm body);
F_CodeFragmentList frame_ReturnFragmentList(void);

#endif
