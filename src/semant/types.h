
#ifndef __TYPE_H__
#define __TYPE_H__

#include "util.h"
#include "parser/ast.h"

typedef struct Symbol_* Symbol;

struct Type_ {
	enum {
		Int_Type, 
		String_Type,
		Real_Type,
		Char_Type,
		Array_Type, 
		Record_Type, 
		NIL_Type, 
		Name_Type,
		Field_Type,
		Error_Type,
		Void_Type
	} kind;
	int strict;
	enum {
		Unvisited,
		Visiting,
		Resolved
	} state;
	union {
		struct {Symbol name; struct Type_* type;} name_type;
		struct {string name; struct Type_* type;} field_type;
		struct {struct TypeList_* types;} record_type;
		
		struct Type_* array_type;
		
	} u;
};

struct TypeList_ {
	struct Type_* type;
	struct TypeList_* next;
};

typedef struct Type_* Type;
typedef struct TypeList_* TypeList;

typedef enum {
	COMPAR_OP, // Comparisons
	STRICT_OP, // Strict operators with specific typing rules
	INVALID_OP_CLASS,
	OVERLOAD_OP // Operators that perform conversions
} Op_Class;

#define OP_CLASS_MATCH \
	X(OP_ADD, OVERLOAD_OP) \
	X(OP_SUB, OVERLOAD_OP) \
	X(OP_MUL, OVERLOAD_OP) \
	X(OP_DIV, OVERLOAD_OP) \
	X(OP_MOD, STRICT_OP) \
	X(OP_INCREMENT, STRICT_OP) \
	X(OP_DECREMENT, STRICT_OP) \
	X(OP_NOT, STRICT_OP) \
	X(OP_AND, STRICT_OP) \
	X(OP_OR, STRICT_OP) \
	X(OP_LSHIFT, STRICT_OP) \
	X(OP_RSHIFT, STRICT_OP) \
	X(OP_GT, COMPAR_OP) \
	X(OP_LT, COMPAR_OP) \
	X(OP_GT_EQ, COMPAR_OP) \
	X(OP_LT_EQ, COMPAR_OP) \
	X(OP_EQ, COMPAR_OP) \
	X(OP_COMPAR_AND, COMPAR_OP) \
	X(OP_COMPAR_OR, COMPAR_OP) \
	X(OP_COMPAR_NOT_EQ, COMPAR_OP) 
Type make_nil_type(void);
Type make_void_type(void);
Type make_int_type(void);
Type make_string_type(void);
Type make_char_type(void);
Type make_real_type(void);
Type make_error_type(void);
Type make_array_type(Type element_type);
Type make_field_type(string name, Type type);
Type make_name_type(Symbol name, Type type);
Type make_record_type(TypeList fields);

TypeList make_type_list(Type type, TypeList next);
Type actual_type(Type type); 

int match_types(Type type1, Type type2);
size_t type_cost(Type type1);

Op_Class op_class(A_Op operation);

#endif 
