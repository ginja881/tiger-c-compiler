#ifndef _FRAME_H_
#define _FRAME_H_

#include "util.h"
#include "semant/temp.h"

typedef struct Frame_* Frame;
typedef struct F_Access_* F_Access;
typedef struct F_AccessList_* F_AccessList;


struct F_AccessList_ {
	F_Access access;
	F_AccessList next;
};


Frame new_frame(TempLabel frame_name, BoolList params);
TempLabel frame_name(Frame frame);
F_AccessList frame_parameters(Frame frame);
F_Access frame_local_alloc(Frame frame, bool escape);


#endif
