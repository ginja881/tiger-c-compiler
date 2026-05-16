#include "semant/types.h"


Type make_int_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(struct Type_));
	new_type->kind = Int_Type;

	return new_type;
}
Type make_real_type(void) {
	Type new_type = (Type)checked_malloc(sizeof(
}
