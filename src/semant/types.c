#include "semant/types.h"


Type make_int_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Int_Type;

	return new_type;
}
Type make_real_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Real_Type;

	return new_type;
}

Type make_string_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = String_Type;

	return new_type;
}

Type make_char_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Char_Type;

	return new_type;
}

Type make_boolean_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Boolean_Type;

	return new_type;
}

Type make_void_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Void_Type;

	return new_type;
}

Type make_nil_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = NIL_Type;

	return new_type;
}

Type make_array_type(Type element_type) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Array_Type;

	new_type->u.array_type.element_type = element_type;

	return new_type;
}

Type make_field_type(Symobl name, Type type) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Field_Type;

	new_type->u.field_type.name = name;
	new_type->u.field_type.type = type;

	return new_type;
}

Type make_record_type(TypeList fields) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));

	new_type->kind = Record_Type;
	new_type->u.record_type.types = fields;

	return new_type;
}

TypeList make_type_list(Type type, TypeList next) {
	TypeList new_type_list = (TypeList)checked_malloc(sizeof(struct TypeList_));

	new_type_list->type = type;
	new_type_list->next = next;

	return new_type_list;
}


Type actual_type(Type type) {
	if (type->kind == Field_Type)
		return type->u.field_type.type;
	return type;
}

int match_types(Type type1, Type type2) {
	if (type1 == NULL || type2 == NULL)
		return FALSE;
	return (type1->kind == type2->kind ? TRUE : FALSE);
}

Op_Class  op_class(A_Op operation) {
	#define X(ast_op, class_op) \
		if (operation == ast_op) return class_op;
		OP_CLASS_MATCH
	#undef X

	return INVALID_OP_CLASS;
}
