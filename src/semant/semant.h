#include "symbol.h"
#include "types.h"
#include "parser/ast.h"

struct Scope_ {
	SymbolTable environment;
	struct Scope_* deeper_scope;
};



struct SemanticAnalyzer_ {
	struct Scope_* scopes_head;
	Parser parser;
};

typedef struct Scope_* Scope;
typedef struct ScopeStack_* ScopeStack;
typedef struct SemanticAnalyzer_* SemanticAnalyzer;

Scope begin_scope(SemanticAnalyzer sem);
void end_scope(SemanticAnalyzer sem);


SemanticAnalyzer make_semantic_analyzer(Parser parser);


void analyze_semantic(SemanticAnalyzer sem);
v
