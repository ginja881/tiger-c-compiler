#ifndef _TEMP_H_
#define _TEMP_H_

#include "util.h"
#include "symbol.h"



typedef struct Temp_* Temp;
struct Temp_ {
	int num;
};
Temp make_new_temporary(void);


typedef Symbol TempLabel;
TempLabel make_new_temporary_label(void);
TempLabel make_new_temporary_namedlabel(string name);
string templabel_name(TempLabel label);

typedef struct TempList_* TempList;
struct TempList_ {
	Temp temporary;
	TempList next;
};


TempList make_templist(Temp temporary,  TempList next);


typedef struct TempLabelList_* TempLabelList;

struct TempLabelList_ {
	TempLabel temp_label;
	TempLabelList next;
};
TempLabelList make_templabel_list(TempLabel label, TempLabelList next);

#endif
