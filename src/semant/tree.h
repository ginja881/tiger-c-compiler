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
typedef struct Tr_Stm_* Tr_Stm;
typedef struct Tr_StmList_* Tr_StmList;

typedef struct Tr_Exp_* Tr_Exp;
typedef struct Tr_ExpList_* Tr_ExpList;


// IR expressions
struct Tr_Exp_ {
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
		struct {Tr_Exp op1; Tr_Exp op2; Tr_Binop op;} binop;
		Temp temp;
		TempLabel name;
		struct {Tr_Exp function_name; Tr_ExpList arguments;} call_exp;
		Tr_Exp mem;
		struct {Tr_Stm statement; Tr_Exp next;} seq_exp;
	} u;
};

struct Tr_ExpList_ {
	Tr_Exp tr_exp;
	Tr_ExpList next;
};

// Constructors
Tr_Exp Tr_make_constant(int constant);
Tr_Exp Tr_make_binop(Tr_Exp op1, Tr_Binop op, Tr_Exp op2);
Tr_Exp Tr_make_name(TempLabel label);
Tr_Exp Tr_make_call(Tr_Exp function_name, Tr_ExpList arguments);
Tr_Exp Tr_make_mem(Tr_Exp mem);
Tr_Exp Tr_make_seq_exp(Tr_Stm statement, Tr_Exp next);

Tr_ExpList Tr_make_explist(Tr_Exp tr_exp, Tr_ExpList next);


// IR statements
struct Tr_Stm_ {
	enum {
	    TR_MOVE,
	    TR_EXP,
	    TR_JUMP,
	    TR_CJUMP,
	    TR_LABEL,
	    TR_SEQ
	} kind;
	union {
		struct {Tr_Exp source; Tr_Exp result;} move;
		Tr_Exp exp;
		struct {Tr_Exp destination, TempLabelList labels;} jump;
		struct {Tr_Relop op; Tr_Exp left; Tr_Exp right; TempLabel true_dest; TempLabel false_dest;} cjump;
		TempLabel label;
		struct {Tr_Stm stm; Tr_Stm next;} seq;
	} u;
};

struct Tr_StmList_ {
	Tr_Stm stm;
	Tr_StmList next;
};

// Constructors
Tr_Stm Tr_make_move_stm(Tr_Exp source, Tr_Exp result);
Tr_Stm Tr_make_exp_stm(Tr_Exp exp);
Tr_Stm Tr_make_jump(Tr_Exp destination, TempLabelList label);
Tr_Stm Tr_make_cjump(Tr_Relop op, Tr_Exp left, Tr_Exp right, TempLabel true_dest, TempLabel false_dest);
Tr_Stm Tr_make_label(TempLabel label);
Tr_Stm Tr_make_stm_seq(Tr_Stm stm, Tr_Stm next);

Tr_StmList Tr_make_stmlist(Tr_Stm stm, Tr_StmList next);

#endif
