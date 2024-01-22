#ifndef Header_h
#define Header_h

#include "coord.h"

// класс комнаты
class room : public coord {
    public :
    int size_x; // ширина комнаты
    int size_y; // длина комнаты
    
    room() {
        size_x = 0;
        size_y = 0;
    }
    
    void calc_room_coord(int rows, int cols); // рассчитывает координаты комнаты
    
    void create_room(int rows, int cols);// добавляет и нформацию о комнате в массив
    
    void draw_room(int rows, int cols);// рисует комнаты в консоле
};


#endif /* Header_h */
