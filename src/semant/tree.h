#ifndef _TRE_H_
#define _TREE_H_

#include "util.h"
#include "semant/frame.h"
#include "semant/temp.h"

// Binary Operations
enum Tr_Binop {
	TR_PLUS,
	TR_SUB,
	TR_DIV,
	TR_MUL,
	TR_MOD,
	TR_AND,
	TR_OR,
	TR_XOR,
	TR_LSHIFT,
	TR_RSHIFT,
	TR_ARISHIFT
};

// Relational operators
enum Tr_Relop {
	TR_EQ,
	TR_NE,
	TR_LT,
	TR_GT,
	TR_LE,
	TR_GE,
	TR_ULT,
	TR_ULE,
	TR_UGT,
	TR_UGE
};

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
		TR_ESEQ
	} kind;

	union {
		int constant;
		struct {T_Exp op1; T_Exp op2; T_Binop op;} binop;
		Temp temp;
		TempLabel name;
		struct {T_Exp function_name; T_ExpList arguments;} call_exp;
		T_Exp mem;
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
T_Exp Tr_make_mem(T_Exp mem);
T_Exp Tr_make_seq_exp(T_Stm statement, T_Exp next);

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
		struct {T_Exp destination, TempLabelList labels;} jump;
		struct {T_Relop op; T_Exp left; T_Exp right; TempLabel true_dest; TempLabel false_dest;} cjump;
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

#endif
