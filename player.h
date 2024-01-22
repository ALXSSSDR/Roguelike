#ifndef player_h
#define player_h

#include "creature.h"

// класс игрока
class player : public creature {
    public :
    int floor_counter; // счетчик этажа
    
    // default stats
    player(int health, int armour, int damage) {
        cur_hp = health;
        max_hp = health;
        armor = armour;
        dmg = damage;
        cur_gold = 0;
        is_alive = true;
        floor_counter = 1;
    };
    
    // функция передвежния по карте игрока
    void map_movement (int action);
    void game_movement (int action);
    
    //функция вывода статов
    void draw_stats (int rows, int cols);
    
    // спавн игрока
    void spawn_player (room start);
};

#endif /* player_h */
