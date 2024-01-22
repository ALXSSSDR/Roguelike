#ifndef Header_h
#define Header_h

// класс живых созданий ( игрока, мобов)
class creature : public coord {
    public :
    int cur_hp;   // current health
    int max_hp;   // max health
    int cur_gold; // current mana
    int dmg;      // урон
    int armor;    // броня
    bool is_alive;// живой?
    
};

#endif /* Header_h */
