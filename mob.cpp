#include "mob.h"
#include "player.h"
#include "room.h"


// Получение урона от игрока ботом
void mob::taking_damage_and_death(player* pl, int action) {
    if((abs(pl->x - x) <= 1) && (abs(pl->y - y) <= 1) && action == 32) {
        cur_hp = cur_hp - pl->dmg;
    }
    
    if(cur_hp <= 0 && is_alive) {
        is_alive = false;
        draw_mob(*pl);
        
        pl->cur_gold += 10; // Даём игроку 10 монет за убийство бота
        
        mvwprintw(stdscr, 0, 1, "               Монстр убит, из него выполо 10 монет!               ");
    }
}

// Поведение бота (передвижение бота по карте, нанесение урона игроку и т.д.)
void mob::behavior_bot(player* pl, int action)  {
    if(is_alive){ // Функция активна только при условии того, что бот живой
        if((abs(pl->x - x) <= 1) && (abs(pl->y - y) <= 1)) { // Поведение бота, если рядом игрок
            mvwprintw(stdscr, 0, 1, "               Монстр заметил вас!                      "); // Сообщение о том, что бот занет про игрока
            if(!are_you_evil_now) { // Если сейчас не злой, то с определённым шансом будет злой
                int rand_evil_number = (rand() % 101);
                
                if(evil_rate >= rand_evil_number) {
                    are_you_evil_now = true;
                }
            }
            if(are_you_evil_now) { // Если бот злой, то он догоняет игрока и наносит ему урон с определённым шансом
                int rand_damage_number = (rand() % 100);
                
                if(damage_rate >= rand_damage_number) { // Если бот решил атаковать
                    mvwprintw(stdscr, 0, 1, "               Монстр атакует!                   ");
                    pl->cur_hp = pl->cur_hp - dmg;
                    if(pl->cur_hp <= 0){
                        pl->is_alive = false;
                    }
                }
                // Алгоритм преследования ботом игрока
                int rand_catch_number = (rand() % 4);
                
                switch(rand_catch_number) {
                    case 0 :
                        x = pl -> x + 1;
                        y = pl -> y;
                        break;
                        
                    case 1:
                        x = pl -> x - 1;
                        y = pl -> y;
                        break;
                        
                    case 2:
                        x = pl -> x;
                        y = pl -> y + 1;
                        break;
                        
                    case 3:
                        x = pl -> x;
                        y = pl -> y - 1;
                        break;
                }
            }
            
        } else { // Поведение бота, если игрока рядом нету
            int rand_move_number = (rand() % 4);
            
            switch(rand_move_number) {
                case 0 :
                    if (map[x][y - 1] == ' ') { // проверка чтобы не выйти за стену
                        y--;
                        
                        map[x][y] = 'a';
                        map[x][y + 1] = ' ';
                    }
                    break;
                    
                case 1 :
                    if (map[x][y + 1] == ' ') {
                        y++;
                        
                        map[x][y] = 'a';
                        map[x][y - 1] = ' ';
                    }
                    break;
                    
                case 2 :
                    if (map[x + 1][y] == ' ') {
                        x++;
                        
                        map[x][y] = 'a';
                        map[x - 1][y] = ' ';
                    }
                    break;
                    
                case 3 :
                    if (map[x - 1][y] == ' ') {
                        x--;
                        
                        map[x][y] = 'a';
                        map[x + 1][y] = ' ';
                    }
                    break;
            }
        }
    }
};

// Просчёт координат бота
void mob::spawn_mob(room start) {
    x = (rand() % start.size_x) + start.x;
    y = (rand() % start.size_y) + start.y;
    this->is_alive = true; //////
    map[x][y] = 'a';
};

// Нанесение бота на экран консоли
void mob::draw_mob(player pl) {
    if((abs(pl.x - x) <= 5) && (abs(pl.y - y) <= 5)) { // Отрисовка только в поле зрения игрока, равному 5 клеткам
        if (is_alive && map_vision[x][y] == 1) {
            if((abs(pl.x - x) <= 1) && (abs(pl.y - y) <= 1)) {
                init_pair(1, COLOR_RED, COLOR_BLACK);
                attron(COLOR_PAIR(1));
                mvaddch(y, x, 'a');
                attroff(COLOR_PAIR(1));
                attrset(0);
            }
            else {
                mvaddch(y, x, 'a');
            }
        }
        else if (map_vision[x][y] == 1) {
            init_pair(1, COLOR_YELLOW, COLOR_BLACK);
            attron(COLOR_PAIR(1));
            mvaddch(y, x, '%');
            attroff(COLOR_PAIR(1));
            attrset(0);
        }
    }
};
