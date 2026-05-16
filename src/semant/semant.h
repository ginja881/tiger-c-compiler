#include "util.h"
#include "semant/symbol.h"
#include "parser/ast.h"

struct Scope_ {
	Environment var_environment;
	Environment type_environment;
	struct Scope_* parent;
};

struct SemanticAnalyzer_ {
	struct Scope_* scopes_head;
	Parser parser;
};

typedef struct Scope_* Scope;
typedef struct ScopeStack_* ScopeStack;
typedef struct SemanticAnalyzer_* SemanticAnalyzer;
typedef void* Tr_Exp; 


Type builtin_string_type = make_string_type();
Type builtin_int_type = make_int_type();
Type builtin_boolean_type = make_boolean_type();
Type builtin_char_type = make_char_type();
Type builtin_real_type = make_real_type();
Type builtin_nil_type = make_nil_type();
Type builtin_void_type = make_void_type();

extern int panic_mode;

// Standard Environments
Environment make_standard_var_env(void);
Environment make_standard_type_env(void);

// Scope Stack
SemanticAnalyzer begin_scope(SemanticAnalyzer sem, Environment var_env, Environment type_env);
SemanticAnalyzer end_scope(SemanticAnalyzer sem);
SemanticAnalyzer peek_scope(SemanticAnalyzer sem);


SemanticAnalyzer make_semantic_analyzer(Parser parser);

Tr_Exp make_tree_exp(A_Exp expression);

Tr_Exp check_exp(A_Exp expression, SemanticAnalyzer sem);

void semantic_main(SemanticAnalyzer sem);


