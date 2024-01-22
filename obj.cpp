#include "obj.h"
#include "room.h"

// рандоминг координаты обьекта в выбранной комнате(квесты, переход на след этаж и тп)
void obj::calc_obj_coord(room start) {
    x = (rand() % start.size_x) + start.x - 1;
    y = (rand() % start.size_y) + start.y;
};
