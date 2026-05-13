#include "symbol.h"
#include "types.h"
#include "parser/ast.h"

struct Scope_ {
	SymbolTable environment;
	struct Scope_* deeper_scope;
};

struct ScopeStack_ {
	struct Scope_* head;
	struct Scope_* top;
};

struct SemanticAnalyzer_ {
	struct ScopeStack_* scopes;
	Parser parser;
};

typedef struct Scope_* Scope;
typedef struct ScopeStack_* ScopeStack;
typedef struct SemanticAnalyzer_* SemanticAnalyzer;



ScopeStack make_scope_stack(void);
Scope begin_scope(void);
ScopeStack end_top_scope(ScopeStack current_stack);
Scope peek(ScopeStack current_stack);


SemanticAnalyzer make_semantic_analyzer(Parser parser, ScopeStack scope_stack);


void analyze_semantic(SemanticAnalyzer sem);
v
