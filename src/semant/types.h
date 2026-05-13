
#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "semant/env.h"

struct Type_ {
	enum {
		Int_Type, 
		String_Type,
		Boolean_Type,
		Real_Type,
		Char_Type,
		Array_Type, 
		Record_Type, 
		NIL_Type, 
		Field_Type,
		Void_Type
	} kind;
	union {
		struct {Symbol name; struct Type_* type;} field_type;
		struct {struct TypeList_* types;} record_type;
		
		struct {struct Type_* element_type;} array_type;
		
	} u;
};

struct Type_List_ {
	struct Type_* type;
	struct Type_List_* type_list;
};

typedef struct Type_* type;
typedef struct Type_List_* Type_List;

Type make_int_type(void);
Type make_string_type(void);
Type make_char_type(void);
Type make_boolean_type(void);
Type make_real_type(void);
Type make_array_type(Type element_type);
Type make_field_type(Symbol name, Type type);
Type_List make_type_list(Type type);
#endif 
