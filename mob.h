#ifndef mob_h
#define mob_h

#include "creature.h"

//класс для мобов
class mob : public creature {
    public :
    int attack_radius; // Радиус атаки бота
    int evil_rate; // В скольких случаях из ста бот будет агриться на человека
    int damage_rate; // В скольких случаях из ста бот будет наносить урон игроку
    int spawn_rate;// В последующем будет влиять на шанс выпадения
    bool are_you_evil_now;
    
    void spawn_mob (room start);
    
    void draw_mob(player pl);
    
    void behavior_bot(player* pl, int action);
    
    void taking_damage_and_death(player* pl, int action);
    
    mob(int u_cur_hp, int u_dmg, int u_attack_radius, int u_evil_rate, int u_damage_rate, int u_spawn_rate){
        cur_hp = u_cur_hp;
        dmg = u_dmg;
        
        attack_radius = u_attack_radius;
        evil_rate = u_evil_rate;
        damage_rate = u_damage_rate;
        spawn_rate = u_spawn_rate;
        are_you_evil_now = false;
        is_alive = true;
    }
};



#endif /* mob_h */
