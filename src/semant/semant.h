#include "util.h"
#include "semant/symbol.h"
#include "parser/ast.h"

struct Scope_ {
	Environment var_environment;
	Environment type_environment;
	struct Scope_* parent;
};

struct SemanticAnalyzer_ {
	struct Scope_* scope_head;
	int checking_for_value;
	Parser parser;
	Type builtin_string_type;
	Type builtin_int_type;
	Type builtin_boolean_type;
	Type builtin_char_type;
	Type builtin_real_type;
	Type builtin_nil_type;
	Type builtin_void_type;
};

typedef struct Scope_* Scope;
typedef struct ScopeStack_* ScopeStack;
typedef struct SemanticAnalyzer_* SemanticAnalyzer;
typedef void* Tr_Exp;

struct Exp_Ty_ {
	Tr_Exp main_exp;
	Type exp_type;
};

typedef struct Exp_Ty_* Exp_Ty;
extern int panic_mode;

Exp_Ty make_exp_ty(Tr_Exp main_exp, Type exp_type);

// Standard Environments
Environment make_standard_var_env(SemanticAnalyzer sem);
Environment make_standard_type_env(SemanticAnalyzer sem);

// Scope Stack
SemanticAnalyzer begin_scope(SemanticAnalyzer sem, Environment var_env, Environment type_env);
SemanticAnalyzer end_scope(SemanticAnalyzer sem);
Scope peek_scope(SemanticAnalyzer sem);

SemanticAnalyzer make_semantic_analyzer(Parser parser);

Exp_Ty check_literal(A_Exp expression, SemanticAnalyzer sem);
Exp_Ty check_strict_op(A_Exp strict_op_exp, SemanticAnalyzer sem);
Exp_Ty check_overload_op(A_Exp overload_op_exp, SemanticAnalyzer sem);

Exp_Ty check_exp(A_Exp expression, SemanticAnalyzer sem);

void semantic_main(SemanticAnalyzer sem);

