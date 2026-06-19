#include "util.h"
#include "semant/temp.h"

typedef struct Frame_* Frame;
typedef struct Access_* Access;
typedef struct AccessList_* AccessList;


struct AccessList_ {
	Access access;
	AccessList next;
};

Frame new_frame(TempLabel frame_name, AccessList params, AccessList locals);
Frame frame_static_link(Frame frame);
TempLabel frame_name(Frame frame);
AccessList frame_parameters(Frame frame);
Access local_alloc(Frame frame, bool escape);



