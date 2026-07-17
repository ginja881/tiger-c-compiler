#ifndef _ESCAPE_H_
#define _ESCAPE_H_

#include "parser/ast.h"
#include "semant/symbol.h"


Environment analyze_escapes(A_Exp root, Environment escape_environment, int starting_depth);
#endif
