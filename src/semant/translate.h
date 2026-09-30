#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_

#include "util.h"
#include "semant/tree.h"
#include "semant/frame.h"
#include "parser/ast.h"

// Accesses and Static Links

typedef struct Tr_Access_* Tr_Access;
typedef  struct Tr_AccessList_* Tr_AccessList;
typedef struct Tr_Level_* Tr_Level;

struct Tr_Access_ {
	Tr_Level level;
	F_Access frame_access;
};

struct Tr_AccessList_ {
	Tr_Access tree_access;
	Tr_AccessList next;
};

Tr_AccessList Tr_make_access_list(Tr_Access tree_access, Tr_AccessList next);
Tr_Level Tr_outermost(void);
Tr_Level Tr_new_level(Tr_Level parent, TempLabel label, BoolList params);

Tr_AccessList Tr_params(Tr_Level level); 
Tr_Access Tr_allocLocal(Tr_Level level, bool escape);


typedef  struct Tr_PatchList_* Tr_PatchList; 
typedef struct T_Cx_* T_Cx;
typedef struct Tr_Exp_* Tr_Exp;
typedef struct IRList_* IRList;

struct  T_Cx_ {
	Tr_PatchList trues;
	Tr_PatchList falses;
	T_Stm stm;
};

struct Tr_Exp_ {
	enum {CX, NX, EX} kind;
	union {
		T_Cx cx;
		T_Exp ex;
		T_Stm nx;
	} u;
};


struct Tr_PatchList_ {
	TempLabel label;
	Tr_PatchList next;
};

struct IRList_ {
	Tr_Exp tree_ir;
	IRList next;
};

void Tr_init(void);

// Patch API
void Tr_doPatch(Tr_PatchList patch_list, TempLabel label);
Tr_PatchList Tr_joinPatch(Tr_PatchList first, Tr_PatchList second);

// IR List API
IRList Tr_makeIRList(Tr_Exp tree_ir, IRList next);
void append_node(IRList node, IRList* head, IRList* tail);



// Variables, Records, and Arrays
Tr_Exp Tr_translateVariableAccess(Tr_Access variable_access, Tr_Level level, int size);
Tr_Exp Tr_translateAssignment(Tr_Exp lvalue, Tr_Exp rvalue);
Tr_Exp Tr_translateArray(Tr_Exp default_value, int element_size, Tr_Exp array_size);
Tr_Exp Tr_translateRecord(IRList field_assignments, int length);
Tr_Exp Tr_translateFieldReference(Tr_Exp record_id, int field_pos);


// Operations: Strict & Overloading
Tr_Exp Tr_translateBasicOperation(Tr_Exp operand1, Tr_Exp operand2, A_Op operation);
// Operations: Unary
Tr_Exp Tr_translateUnaryOperation(Tr_Exp operand, A_Op unary_op);
// Operations: Comparisons
Tr_Exp Tr_translateComparison(Tr_Exp operand1, A_Op comparison, Tr_Exp operand2);

// Literals
Tr_Exp Tr_translateInteger(int integer);
Tr_Exp Tr_translateCharacter(char character);
Tr_Exp Tr_translateArrayAccess(Tr_Exp array_id, Tr_Exp index_ir, int element_type_size);
Tr_Exp Tr_lowerSequence(IRList sequence);

// Control Flow
Tr_Exp Tr_translateBreak(void);
Tr_Exp Tr_translateContinue(void);
Tr_Exp Tr_translateWhileLoop(Tr_Exp condition, Tr_Exp block);
Tr_Exp Tr_translateIfStatement(Tr_Exp condition, Tr_Exp then_block, Tr_Exp else_block);
Tr_Exp Tr_translateForLoop(Tr_Exp low_acces, Tr_Exp low_value, Tr_Exp limit, Tr_Exp block);
Tr_Exp Tr_translateCallee(TempLabel flbl, Tr_Level current_level, Tr_Level function_level, IRList arguments);
Tr_Exp Tr_translateLetExp(IRList variable_assignments, Tr_Exp let_block);

// Functions & Fragments
void Tr_makeFunction(Tr_Level current_level, Tr_Exp body, Tr_AccessList parameters);
Tr_Exp Tr_translateString(string text);

#endif
