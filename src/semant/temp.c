#include "semant/temp.h"
static temp_count = 0;

Temp make_new_temp(void) {
	Temp new_temp = (Temp) checked_malloc(sizeof(struct Temp_));
	new_temp->num = temp_count++;
	return new_temp;
}

Temp make_new_temporary_label(void) {
	TempLabel new_templabel = (TempLabel) checked_malloc(sizeof(struct Symbol_));
	return new_templabel;
}
Temp make_new_temporary_namedlabel(string name) {
	TempLabel new_templabel = (TempLabel)checked_malloc(sizeof(struct Symbol_));
	new_templabel->name = strdup(name);

	return new_templabel;
}

string templabel_name(TempLabel label) {
	if (label == NULL)
		return NULL;
	return label->name;
}

TempList make_templist(Temp temporary, TempList next) {
	TempList new_templist = (TempList)checked_malloc(sizeof(struct TempList_));
	new_templist->temporary = temporary;
	new_templist->next = next;

	return new_templist;
}

TempLabelList make_templabel_list(TempLabel label, TempLabelList next) {
	TempLabelList new_templabellist = (TempLabelList)checked_malloc(sizeof(struct TempLabelList_));
	new_templabellist->label = label;
	new_templabellist->next = next;

	return new_templabellist;
}


