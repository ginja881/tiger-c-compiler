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
     OP_COMPAR_NOT_EQ,
     OP_COMPAR_AND,
     OP_COMPAR_OR,
     OP_COMPAR_EQ,
     OP_GT, 
     OP_GT_EQ,
     OP_LT,
     OP_LT_EQ,
     OP_NOT,
     OP_AND,
     OP_OR,
     OP_MEMBER_REF,
     OP_INVALID
} A_Op;
#define OP_MATCH \
	X(ADD, OP_ADD) \
	X(SUB, OP_SUB) \
	X(MUL, OP_MUL) \
	X(DIV, OP_DIV) \
	X(MOD, OP_MOD) \
	X(INCREMENT, OP_INCREMENT) \
	X(DECREMENT, OP_DECREMENT) \
	X(NOT, OP_NOT) \
	X(BIT_AND, OP_AND) \
	X(BIT_OR, OP_OR) \
	X(BIT_LSHIFT, OP_LSHIFT) \
	X(BIT_RSHIFT, OP_RSHIFT) \
	X(GT, OP_GT) \
	X(LT, OP_LT) \
	X(GT_EQ, OP_GT_EQ) \
	X(LT_EQ, OP_LT_EQ) \
	X(COMPAR_AND, OP_COMPAR_AND) \
	X(COMPAR_OR, OP_COMPAR_OR) \
	X(COMPAR_EQ, OP_COMPAR_EQ) \
	X(COMPAR_NOT_EQ, OP_COMPAR_NOT_EQ) \
	X(MEMBER_REF, OP_MEMBER_REF)

// Lexer data
struct A_Pos_ {
     size_t col_pos;
     size_t line_pos;
     size_t indentation_level;
};
typedef struct A_Pos_* A_Pos;

// Expressions
struct A_Exp_ {
     enum {
     	ID_Exp, 
	Num_Exp, 
	NIL_Exp, 
	Real_Exp, 
	Op_Exp, 
	Callee_Exp,
	Bool_Exp,
	Char_Exp,
	Array_Exp,
	String_Exp,
	Field_Exp,
	Seq_Exp,
	For_Exp,
	If_Exp,
	Let_Exp,
	Assign_Exp,
	While_Exp,
	Unary_Exp
     } kind;
     A_Pos position;
     union {
     	struct {string identifier;} id_exp;
	struct {int value;} num_exp;
	struct {double value;} real_exp;
	struct {int boolean;} bool_exp;
	struct {A_Op op; struct A_Exp_* exp; int postfix;} unary_exp;
	struct {A_Op op; struct A_Exp_* exp1; struct A_Exp_* exp2;} op_exp;
	struct {string id; struct A_ExpList_* args;} callee_exp;	
	struct {string text;} string_exp;
	struct {char character;} char_exp;
	struct {string type; struct A_Exp_* init; struct A_Exp_* size;} array_exp;
        struct {struct A_Field_* field;} field_exp;
	struct {struct A_ExpList_* exp_list;} seq_exp;
	struct {struct A_Exp_* cond; struct A_Exp_* then; struct A_Exp_* else_block;} if_exp;
	struct {struct A_Exp_* cond; struct A_Exp_* block;} while_exp;
	struct {struct A_Exp_* identifier; struct A_Exp_* val;} assign_exp;
	struct {struct A_Exp_* id; struct A_Exp_* low; struct A_Exp_* high; struct A_Exp_* block;} for_exp;
	struct {struct A_DecList_* declarations; struct A_Exp_* block;} let_exp;

     } u;
};

// Expression Lists
struct A_ExpList_ {
       struct A_Exp_* exp;
       struct A_ExpList_* next;
};

// Fields
struct A_Field_ {
     enum {
         Subscript_Field,
	 Ty_Field,
	 Array_Field,
	 Record
     } kind;
     A_Pos position;
     union {
          struct {string id; struct A_Exp_* loc;} subscript_field;
	  struct {string id; string type;} ty_field;
	  struct {string type_id;} array_field;
	  struct {struct A_FieldList_* record_def_fields;} record_field;
     } u;
};

struct A_FieldList_ {
    struct A_Field_* field;
    struct A_FieldList_* next;
};


// Decs

struct A_Dec_ {
	enum {
	    Type_Dec,
	    Simple_Var_Dec,
	    Field_Var_Dec,
	    Func_Dec
	} kind;
	A_Pos position;
	union {
		struct {string name; struct A_Field_* def_type_field;} type_dec;
		struct {struct A_Field_* field; struct A_Exp_* val;} field_var_dec;
		struct {string id; struct A_Exp_* val;} simple_var_dec;
		struct {string name; struct A_FieldList_* args; string type; struct A_Exp_* block;} func_dec;
	} u;
};

struct A_DecList_ {
	struct A_Dec_* dec;
	struct A_DecList_* next;
};

struct A_Stm_ {
	enum {
		Exp_Stm,
		Continue_Stm,
		Break_Stm,
		Decl_Stm,
		Compound_Stm
	} kind;
	A_Pos position;
	union {
		struct{ struct A_Exp_* expression;} exp_stm;
		struct{struct A_Stm_* stm1; struct A_Stm_* stm2;} compound_stm;
		struct{struct A_Dec_* dec;} decl_stm;
	} u;
};

// Parser object for the overall phase & semantic analysis

struct Parser_ {
     struct A_Stm_* root;
     int is_in_block;
     int current_stm;
};

// Types
typedef struct Parser_* Parser;
typedef struct A_Exp_* A_Exp;
typedef struct A_Stm_* A_Stm;
typedef struct A_ExpList_* A_ExpList;
typedef struct A_Field_* A_Field;
typedef struct A_FieldList_* A_FieldList;
typedef struct A_Dec_* A_Dec;
typedef struct A_DecList_* A_DecList;


// Constructors
Parser make_parser(void);

A_Pos make_pos(size_t col_pos, size_t line_pos);

A_Exp make_id_exp(string id, A_Pos position);
A_Exp make_num_exp(int num, A_Pos position);
A_Exp make_real_exp(double value, A_Pos position);
A_Exp make_nil_exp(A_Pos position);
A_Exp make_bool_exp(int boolean, A_Pos position);
A_Exp make_char_exp(char character, A_Pos position);
A_Exp make_string_exp(string text, A_Pos position);


A_ExpList make_exp_list(A_Exp exp);
A_Exp make_op_exp(A_Op op, A_Exp exp1, A_Exp exp2);
A_Exp make_callee_exp(string id, A_ExpList args, A_Pos position);
A_Exp make_array_exp(string type, A_Exp size, A_Exp init, A_Pos position);
A_Exp make_unary_exp(A_Op op, A_Exp exp, int post_fix, A_Pos position);
A_Exp make_dec_exp(A_Dec exp);

A_Exp make_assign_exp(A_Exp identifier, A_Exp val);
A_Exp make_for_exp(A_Exp id, A_Exp high, A_Exp low, A_Exp block, A_Pos position);
A_Exp make_while_exp(A_Exp cond, A_Exp block, A_Pos position);
A_Exp make_if_exp(A_Exp cond, A_Exp then, A_Exp else_branch, A_Pos position);
A_Exp make_let_exp(A_DecList declarations, A_Exp block, A_Pos position);
A_Exp make_seq_exp(A_ExpList exp_list);

A_Field make_subscript_field(string id, A_Exp loc, A_Pos position);
A_Field make_type_field(string id, string type, A_Pos position);
A_FieldList make_field_list(A_Field field);
A_Field make_array_field(string type_id, A_Pos position);
A_Field make_record(A_FieldList field_list, A_Pos position);
A_Exp make_field_exp(A_Field field);

A_Dec make_simple_var_dec(string id, A_Exp var_val, A_Pos position);
A_Dec make_field_var_dec(A_Field type_field, A_Exp var_val, A_Pos Position);
A_Dec make_type_dec(string name, A_Field def_field, A_Pos position);
A_Dec make_func_dec(string name, A_FieldList args, string type, A_Exp block, A_Pos position);
A_DecList make_dec_list(A_Dec declaration);

A_Stm make_declaration_stm(A_Dec declaration);
A_Stm make_expression_stm(A_Exp expression);
A_Stm make_compound_stm(A_Stm stm1, A_Stm stm2);
A_Stm make_break_stm(A_Pos position);
A_Stm make_continue_stm(A_Pos position);


void eat_lines(Lexer lexer, Parser parser); 


// Parsing functions that implement recursive descent clauses

A_Op match_op(Token operation); 



A_FieldList parse_field_list(Lexer lexer, Parser parser, token delimiter);
A_DecList parse_dec_list(Lexer lexer, Parser parser, token delimiter);
A_ExpList parse_exp_list(Lexer lexer, Parser parser, token delimiter);




A_Exp parse_primary(Lexer lexer, Parser parser);
A_Exp parse_unary(Lexer lexer, Parser parser);
A_Exp parse_postfix(Lexer lexer, Parser parser);
A_Exp parse_factor(Lexer lexer, Parser parser);
A_Exp parse_term(Lexer lexer, Parser parser);

A_Exp parse_assign_exp(Lexer lexer, Parser parser);
A_Exp parse_for_exp(Lexer lexer, Parser parser);
A_Exp parse_if_exp(Lexer lexer, Parser parser);
A_Exp parse_while_exp(Lexer lexer, Parser parser);
A_Exp parse_let_exp(Lexer lexer, Parser parser);
A_Exp parse_control_exp(Lexer lexer, Parser parser);

A_Exp parse_expression(Lexer lexer, Parser parser);

A_Dec parse_variable(Lexer lexer, Parser parser, A_Pos position);
A_Dec parse_type(Lexer lexer, Parser parser, A_Pos position);
A_Dec parse_function_dec(Lexer lexer, Parser parser, A_Pos position);



A_Stm parse_statement(Lexer lexer, Parser parser);

A_Stm parse_program(Lexer lexer, Parser parser);
#endif
