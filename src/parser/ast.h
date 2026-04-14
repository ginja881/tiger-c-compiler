#ifndef __AST_H__
#define __AST_H__

#include "util.h"
#include "lex/tokens.h"

typedef enum {
     OP_ADD,
     OP_LSHIFT,
     OP_RSHIFT,
     OP_INCREMENT,
     OP_MUL,
     OP_SUB,
     OP_DIV,
     OP_DECREMENT,
     OP_MOD,
     OP_COMPAR_AND,
     OP_COMPAR_OR,
     OP_COMPAR_EQ,
     OP_GT, 
     OP_GT_EQ,
     OP_LT,
     OP_LT_EQ,
     OP_AND,
     OP_OR,
     OP_REF
} A_Op;

// Lexer data
struct A_Pos_ {
     size_t start_col;
     size_t line_loc;
     size_t indentation_level;
};
typedef struct A_Pos_* A_Pos;

struct A_Exp_ {
     enum {ID_Exp, Num_Exp, NIL_Exp, Real_Exp, Op_Exp, A_Callee} kind;
     A_Pos pos_info;
     union {
     	struct {string identifier; string type_id;} id;
	struct {int value;} num;
	struct {double value;} real;
	struct {A_Op op; struct A_Exp_* exp1; struct A_Exp_* exp2;} op_exp;
	struct {string function_name; struct A_ExpList_* args;} callee_exp;
     } u;
};

struct A_ExpList_ {
       struct A_Exp_* exp;
       struct A_ExpList_* next;
};

struct A_Stm_ {
	enum {
		While_Stm, 
		For_Stm, 
		If_Stm,
		Else_Stm.
		Elif_Stm,
		Var_Dec,
		Type_Dec,
		Let_Stm,
		Compound_Stm,
		Func_Dec
	} kind;
	A_Pos pos_info;
        union {
	    struct {struct A_Exp_* while_cond; struct A_Stm_* block;} while_stm;
	    struct {struct A_Exp_* condition; struct A_Stm_* cond_block; struct A_Stm_* next_cond;} if_chain;
	    struct {struct A_Stm_* stm1; struct A_Stm_* stm2;} compound_stm;
	    struct {string id; struct A_Exp_* var_val;} variable_dec;
	    struct {string id; struct A_Stm_* block; struct A_ExpList_* args;} function_dec;
	} u;
};

struct Parser_ {
     A_Stm root;
     int is_in_block;
     size_t current_indentation_level;
};
typedef struct Parser_* Parser;
typedef struct A_Exp_* A_Exp;
typedef struct A_ExpList_* A_ExpList;
typedef struct A_Stm_* A_Stm;


// Constructors
Parser make_parser(void);

A_Pos make_pos(size_t col_pos, size_t line_pos, size_t indentation_level);

A_Exp make_id_exp(string id, string type_dec, A_Pos node_position);
A_Exp make_num_exp(int id, A_Pos node_position);
A_Exp make_real_exp(double value, A_Pos node_position);
A_Exp make_op_exp(A_Op op, A_Exp exp1, A_Exp exp2);
A_Exp make_callee_exp(string function_name, A_ExpList args);


A_Stm make_while_stm(A_Exp while_cond, A_Stm block);
A_Stm make_if_chain(A_Exp cond, A_Stm block);
A_Stm make_compound_stm(A_Stm stm1, A_Stm stm2);
A_Stm make_variable_dec(string id, A_Exp var_val);
A_Stm make_function_dec(stirng id, A_Stm block, A_ExpList args);


#endif
