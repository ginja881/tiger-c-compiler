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


