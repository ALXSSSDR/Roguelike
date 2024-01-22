#include <iostream>
#include "room.h"



// создание комнаты в массиве
void room::create_room(int rows, int cols) {
    for (int i = x; i <= x + size_x; i++) {
        for (int j = y; j <= y + size_y; j++) {
            map[i][j]  = ' ';
        }
    }
};

// рандоминг расположения комнаты на карте  + добавление ее в массив карту обьектов
void room::calc_room_coord(int rows, int cols) {
    int collision = 1;
    
    while (collision == 1) {
        
        x = (rand() % (rows - 30)) + 6; // положение комнаты
        y = (rand() % (cols - 30)) + 6;
        size_x = (rand() % 15) + 8; // размер комнаты
        size_y = (rand() % 10) + 5;
        collision = 0;
        
        // антиналожение друг на друга комнат
        for (int i = x; i < x + size_x; i++) {
            for (int j = y; j < y + size_y; j++) {
                
                if (map[i][j] == ' ' || map[i - 2][j] == ' '
                    || map[i][j - 2] == ' ' || map[i + 2][j] == ' '
                    || map[i][j + 2] == ' ') {
                    collision = 1;
                    i = x + size_x;
                    j = y + size_y;
                }
            }
        }
    }
    create_room(rows, cols);
};


// вывод комнат в консоль
void room::draw_room(int rows, int cols) {
    for (int i = x; i <= x + size_x; i++) {
        for (int j = y; j <= y + size_y; j++) {
            if (map_vision[i][j] == 1) {
                mvaddch(j, i, ' ');
            } else {
                mvaddch(j, i, '.');
            }
        }
    }
};
