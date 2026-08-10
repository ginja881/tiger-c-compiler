#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_

#include "util.h"
#include "semant/tree.h"

// Accesses and Static Links

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


typedef  struct Tr_PatchList_* Tr_PatchList; 
typedef struct T_Cx_* T_Cx;
typedef struct Tr_Exp_* Tr_Exp;

struct  T_Cx_ {
	Tr_PatchList trues;
	Tr_Patchlust False;
	T_Stm stm;
};

struct Tr_Exp_ {
	enum {CX, NX, EX};
	union {
		T_Cx cx;
		T_Exp ex;
		T_Stm nx;
	} u;
};


// Don't make consturctors for this type encapsulated?


#endif
