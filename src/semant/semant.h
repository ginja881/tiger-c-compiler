#ifndef _SEMANT_H_
#define _SEMANT_H_

#include "util.h"
#include "semant/symbol.h"
#include "parser/ast.h"
#include "semant/escape.h"
#include "semant/translate.h"

struct Scope_ {
	Environment var_environment;
	Environment type_environment;
	Environment escape_environment;
	Tr_Level current_level;	
	struct Scope_* parent;
};


typedef enum {Block, While, For, Let, If, Global} ExpLoc;
typedef struct TreeIR_* TreeIR;

struct SemanticAnalyzer_ {
	struct Scope_* scope_head;
	int checking_for_value;
	ExpLoc currently_checking_in;
	Parser parser;
	Type builtin_string_type;
	Type builtin_int_type;
	Type builtin_char_type;
	Type builtin_nil_type;
	Type builtin_void_type;
	Type builtin_error_type;
	Tr_Level outermost_level;
	TreeIR ir_root;
};

typedef struct Scope_* Scope;
typedef struct ScopeStack_* ScopeStack;
typedef struct SemanticAnalyzer_* SemanticAnalyzer;



struct TreeIR_ {
	enum {TreeCompound, TreeExp} kind;
	union {
		struct {TreeIR exp1; TreeIR exp2;} compound;	
		struct {Tr_Exp exp_ir; Type exp_type;} expression;
		
	} u;
};


// Panic mode for errors
extern int panic_mode;


// Tree IR API
TreeIR make_tree_ir_exp(Tr_Exp exp_ir, Type exp_type);
TreeIR make_tree_ir_compound(TreeIR exp1, TreeIR exp2);

// Standard Environments
Environment make_standard_var_env(SemanticAnalyzer sem);
Environment make_standard_type_env(SemanticAnalyzer sem);

// Scope Stack
SemanticAnalyzer begin_scope(SemanticAnalyzer sem);
SemanticAnalyzer end_scope(SemanticAnalyzer sem);
Scope peek_scope(SemanticAnalyzer sem);

// Main functions for semantic analysis pass that take AST nodes -> IR nodes
SemanticAnalyzer make_semantic_analyzer(Parser parser);
TreeIR check_field_exp(A_Exp field, SemanticAnalyzer sem);
TreeIR check_literal(A_Exp expression, SemanticAnalyzer sem);
TreeIR check_strict_op(A_Exp strict_op_exp, SemanticAnalyzer sem);
TreeIR check_overload_op(A_Exp overload_op_exp, SemanticAnalyzer sem);


TreeIR check_exp(A_Exp expression, SemanticAnalyzer sem);

// Where framing and escape analysis are located
SemanticAnalyzer process_body(A_Dec declaration, SemanticAnalyzer sem);
TreeIR handle_simple_variable(A_Dec declaration, SemanticAnalyzer sem);
TreeIR handle_field_variable(A_Dec declaration, SemanticAnalyzer sem);
void handle_function(A_Dec declaration, SemanticAnalyzer sem);


// Type handling & helpers
void resolve_type(Type result_type, SemanticAnalyzer sem, A_Pos position);
SemanticAnalyzer precheck_decs(A_Dec declaration, SemanticAnalyzer sem);
Type handle_type_def(Symbol current_symbol, A_Dec current_declaration, SemanticAnalyzer sem);

TreeIR process_statement(A_Stm stm, SemanticAnalyzer sem);
TreeIR semantic_main(SemanticAnalyzer sem);

#endif
