#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_

#include "frame.h"
#include "temp.h"

typedef struct Tr_Access_* Tr_Access;
typedef  struct Tr_AccessList_* Tr_AccessList;
typedef struct Tr_Level_* Tr_Level;

struct Tr_Access_ {
	Tr_Level level;
	F_Access frame_access;
};

struct Tr_AccessList_ {
	Tr_Access tree_access;
	Tr_AccessList next;
};

Tr_AccessList Tr_make_access_list(Tr_Access tree_access, Tr_AccessList next);
Tr_Level Tr_outermost(void);
Tr_Level Tr_new_level(Tr_Level parent, TempLabel label, BoolList params);

Tr_AccessList Tr_params(Tr_Level level); 
Tr_Access Tr_allocLocal(Tr_Level level, bool escape);

#endif
