#include "semant/types.h"
Type make_nil_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = NIL_Type;
	new_type->state = Resolved;

	return new_type;
}
Type make_void_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Void_Type;
	new_type->state = Resolved;

	return new_type;
}
Type make_int_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Int_Type;
	new_type->state = Resolved;

	return new_type;
}
Type make_real_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Real_Type;
	new_type->state = Resolved;

	return new_type;
}

Type make_string_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = String_Type;
	new_type->state = Resolved;

	return new_type;
}

Type make_char_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Char_Type;
	new_type->state = Resolved;

	return new_type;
}

Type make_error_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Error_Type;
	new_type->state = Resolved;

	return new_type;
}



Type make_array_type(Type element_type) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Array_Type;

	new_type->u.array_type = element_type;
	new_type->state = Resolved;
	return new_type;
}

Type make_name_type(Symbol name, Type type) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Name_Type;

	new_type->u.name_type.name = name;
	new_type->u.name_type.type = type;
	new_type->state = Unvisited;
	return new_type;
}
Type make_field_type(string name, Type type) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Field_Type;

	new_type->u.field_type.name = strdup(name);
	new_type->u.field_type.type = type;
	new_type->state = Resolved;

	return new_type;
}

Type make_record_type(TypeList fields) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));

	new_type->kind = Record_Type;
	new_type->u.record_type.types = fields;
	new_type->state = Resolved;

	return new_type;
}

TypeList make_type_list(Type type, TypeList next) {
	TypeList new_type_list = (TypeList)checked_malloc(sizeof(struct TypeList_));

	new_type_list->type = type;
	new_type_list->next = next;

	return new_type_list;
}



Type actual_type(Type type) {
	while (type && type->kind == Name_Type && type->u.name_type.type != NULL) {
		type = type->u.name_type.type;
	}
	return type;
}

int match_types(Type type1, Type type2) {
	if (type1 == NULL || type2 == NULL)
		return FALSE;

	Type actual_type1 = actual_type(type1);
	Type actual_type2 = actual_type(type2);
	
	if  (actual_type1->kind == Field_Type && actual_type2->kind == Field_Type) {
		return match_types(actual_type(actual_type1->u.field_type.type), actual_type(actual_type2->u.field_type.type));
	}
	else if (actual_type1->kind == Field_Type && actual_type2->kind != Field_Type)
		return match_types(actual_type(actual_type1->u.field_type.type), actual_type2);
	else if (actual_type1->kind != Field_Type && actual_type2->kind == Field_Type)
		return match_types(actual_type1, actual_type(actual_type2->u.field_type.type));
	else if (actual_type1->kind == NIL_Type && actual_type2->kind == Record_Type)
		return TRUE;
	else if (actual_type1->kind == Record_Type &&  actual_type2->kind == NIL_Type)
		return TRUE;
	else if (actual_type1->kind == Array_Type && actual_type2->kind == Array_Type)
		return match_types(actual_type(actual_type1->u.array_type), actual_type(actual_type2->u.array_type));
	else if (actual_type1->kind == actual_type2->kind)
		return TRUE;

	return FALSE;
}
size_t type_cost(Type type1) {
	if (type1 == NULL)
		return 0;

	switch(type1->kind) {
		case Void_Type: return 0;
		case NIL_Type: return 0;
		case Int_Type: return sizeof(int);
		case Char_Type: return sizeof(char);
		case Real_Type: return sizeof(double);
		case Array_Type: return type_cost(type1->u.array_type) * 10;
		case Field_Type: return type_cost(type1->u.field_type.type);
		case Record_Type: {
			size_t total_cost = 0;
			TypeList current_type = type1->u.record_type.types;
			while (current_type != NULL) {
				total_cost += type_cost(current_type->type);
				current_type = current_type->next;
			}
			return total_cost;
		}
		default: break;
	}
	return -1;
}
Op_Class  op_class(A_Op operation) {
	#define X(ast_op, class_op) \
		if (operation == ast_op) return class_op;
		OP_CLASS_MATCH
	#undef X

	return INVALID_OP_CLASS;
}
