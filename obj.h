#ifndef obj_h
#define obj_h

#include "coord.h"

class obj : public coord {
    public :
    void calc_obj_coord(room start);
};

#endif /* obj_h */
