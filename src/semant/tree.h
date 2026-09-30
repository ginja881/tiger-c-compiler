#pragma once

#ifndef _TREE_H_
#define _TREE_H_

#include "util.h"
#include "semant/temp.h"
#include "parser/ast.h"

extern const int F_WordSize; 

// Binary Operations
typedef enum {
	TR_PLUS,
	TR_SUB,
	TR_DIV,
	TR_MUL,
	TR_MOD,
	TR_AND,
	TR_OR,
	TR_XOR,
	TR_LSHIFT,
	TR_RSHIFT, // Logical R SHIFT
	TR_ARISHIFT,
	INVALID_TREE_BINOP
} Tr_Binop;



#define TREE_BASIC_OP_MATCH \
	X(OP_ADD, TR_PLUS) \
	X(OP_SUB, TR_SUB) \
	X(OP_DIV, TR_DIV) \
	X(OP_MUL, TR_MUL) \
	X(OP_AND, TR_AND) \
	X(OP_OR, TR_AND) \
	X(OP_LSHIFT, TR_LSHIFT) \
	X(OP_RSHIFT, TR_RSHIFT)


// Relational operators
typedef enum {
	TR_EQ,
	TR_NE,
	TR_LT,
	TR_GT,
	TR_LE,
	TR_GE,
	TR_ULT,
	TR_ULE,
	TR_UGT,
	TR_UGE,
	INVALID_TREE_RELOP
} Tr_Relop;



#define TREE_REL_OP_MATCH \
	X(OP_GT, TR_GT) \
	X(OP_GT_EQ, TR_GE) \
	X(OP_LT, TR_LT) \
	X(OP_LT_EQ, TR_LE) \
	X(OP_EQ, TR_EQ) \
	X(OP_COMPAR_NOT_EQ, TR_NE)



// Type aliases  
typedef struct T_Stm_* T_Stm;
typedef struct T_StmList_* T_StmList;

typedef struct T_Exp_* T_Exp;
typedef struct T_ExpList_* T_ExpList;


// IR expressions
struct T_Exp_ {
	enum {
		TR_BINOP,
		TR_TEMP,
		TR_CONST,
		TR_MEM,
		TR_CALL,
		TR_NAME,
		TR_CHAR,
		TR_ESEQ
	} kind;

	union {
		int constant;
		int character;
		struct {T_Exp op1; T_Exp op2; Tr_Binop op;} binop;
		Temp temp;
		TempLabel name;
		struct {T_Exp function_name; T_ExpList arguments;} call_exp;
		struct {T_Exp memory; int size;} mem;
		struct {T_Stm statement; T_Exp next;} seq_exp;
	} u;
};

struct T_ExpList_ {
	T_Exp tr_exp;
	T_ExpList next;
};

// Constructors
T_Exp Tr_make_constant(int constant);
T_Exp Tr_make_binop(T_Exp op1, Tr_Binop op, T_Exp op2);
T_Exp Tr_make_name(TempLabel label);
T_Exp Tr_make_call(T_Exp function_name, T_ExpList arguments);
T_Exp Tr_make_mem(T_Exp memory, int size);
T_Exp Tr_make_seq_exp(T_Stm statement, T_Exp next);
T_Exp Tr_make_temp(Temp temp);
T_Exp Tr_make_char(char character);
T_ExpList Tr_make_explist(T_Exp tr_exp, T_ExpList next);


// IR statements
struct T_Stm_ {
	enum {
	    TR_MOVE,
	    TR_EXP,
	    TR_JUMP,
	    TR_CJUMP,
	    TR_LABEL,
	    TR_SEQ
	} kind;
	union {
		struct {T_Exp source; T_Exp result;} move;
		T_Exp exp;
		struct {T_Exp destination; TempLabelList labels;} jump;
		struct {Tr_Relop op; T_Exp left; T_Exp right; TempLabel true_dest; TempLabel false_dest;} cjump;
		TempLabel label;
		struct {T_Stm stm; T_Stm next;} seq;
	} u;
};

struct T_StmList_ {
	T_Stm stm;
	T_StmList next;
};

// Constructors
T_Stm Tr_make_move_stm(T_Exp source, T_Exp result);
T_Stm Tr_make_exp_stm(T_Exp exp);
T_Stm Tr_make_jump(T_Exp destination, TempLabelList label);
T_Stm Tr_make_cjump(Tr_Relop op, T_Exp left, T_Exp right, TempLabel true_dest, TempLabel false_dest);
T_Stm Tr_make_label(TempLabel label);
T_Stm Tr_make_stm_seq(T_Stm stm, T_Stm next);

T_StmList Tr_make_stmlist(T_Stm stm, T_StmList next);

Tr_Binop match_tree_binop(A_Op binary_op);
Tr_Relop match_tree_relop(A_Op relation_op);
#endif
