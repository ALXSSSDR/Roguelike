#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdlib>

#include "obj.h"
#include "mob.h"
#include "player.h"
#include "room.h"


using namespace std;

// карты
char map[210][52]; // карта обьектов
int map_vision[210][52]; // карта вижена



// обнуляем вижн
void init_map_vision(int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            map_vision[i][j] = 0;
        }
    }
};


// заполнение  массива стенами
void fill_map(int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            map[i][j] = '#'; // # - стена
        }
    }
};



// вывод стен в консоль
void draw_walls(int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (map_vision[i][j] == 1) {
                mvaddch(j, i, '#');
            } else {
                mvaddch(j, i, '.');
            }
        }
    }
};





// вывод значка квеста в консоль
void draw_quest(coord quest) {
    if (map_vision[quest.x][quest.y] == 1) {
        mvaddch(quest.y, quest.x, '!');
    }
};

void draw_magazine(coord magazine) {
    if (map_vision[magazine.x][magazine.y] == 1) {
        mvaddch(magazine.y, magazine.x, '$');
    }
};

// вывод значка перехода на след этаж в консоль
void draw_restart(coord restart) {
    if (map_vision[restart.x][restart.y] == 1) {
        mvaddch(restart.y, restart.x, '0');
    }
};

// старт текстового квеста
obj start_quest(int rows, int cols, player *pl) {
    obj quest;
    coord accept;
    coord cancel;
    int action;
    bool is_complete = false;
    const char *mesg = "Можно зачет по ТРПО?";
    coord checkpoint_pl;
    checkpoint_pl.x = pl->x;
    checkpoint_pl.y = pl->y;
    pl->x = rows / 2;
    pl->y = cols / 2;
    accept.x = (rows / 2) - 20;
    accept.y = cols / 2 + 2;
    cancel.x = (rows / 2) + 20;
    cancel.y = cols / 2 + 2;
    
    while (!is_complete && (action = getch()) != 27) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                mvaddch(j, i, ' ');
            }
        }
        mvwprintw(stdscr, cols / 4, (rows - strlen(mesg)) / 2, "%s", mesg);
        mvwprintw(stdscr, accept.y - 1, accept.x - 1, "YES");
        mvwprintw(stdscr, accept.y, accept.x, "x");
        mvwprintw(stdscr, cancel.y - 1, cancel.x - 1, "NO");
        mvwprintw(stdscr, cancel.y, cancel.x, "x");
        pl->game_movement(action);
        
        if (pl->x == accept.x && pl->y == accept.y ) {
            pl->dmg = pl->dmg + 10;
            is_complete = true;
        }
        if (pl->x == cancel.x && pl->y == cancel.y ) {
            pl->cur_hp = pl->cur_hp - 10;
            is_complete = true;
        }
    }
    
    pl->x = checkpoint_pl.x;
    pl->y = checkpoint_pl.y;
    quest.x = 0; // убирать квест с карты
    quest.y = 0;
    return quest;
};

void enter_magazine(int rows, int cols, player *pl) {
    coord dmg_up;
    coord restore_hp;
    coord exit_magazine;
    int action;
    bool is_complete = false;
    const char *mesg = "Магазин";
    coord checkpoint_pl;
    checkpoint_pl.x = pl->x;
    checkpoint_pl.y = pl->y;
    pl->x = rows / 2;
    pl->y = cols / 2;
    dmg_up.x = (rows / 2) - 20;
    dmg_up.y = cols / 2 + 2;
    exit_magazine.x = (rows / 2);
    exit_magazine.y = cols / 2 + 18;
    restore_hp.x = (rows / 2) + 20;
    restore_hp.y = cols / 2 + 2;
    
    while (!is_complete && (action = getch()) != 27) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                mvaddch(j, i, ' ');
            }
        }
        pl->draw_stats(rows, cols);
        mvwprintw(stdscr, cols / 4, (rows - strlen(mesg)) / 2, "%s", mesg);
        
        mvwprintw(stdscr, dmg_up.y - 2, dmg_up.x - 2, "10 gold");
        mvwprintw(stdscr, dmg_up.y - 1, dmg_up.x - 3, "ПОВЫШЕНИЕ УРОНА");
        mvwprintw(stdscr, dmg_up.y, dmg_up.x, "x");
        
        mvwprintw(stdscr, restore_hp.y - 2, restore_hp.x - 2, "10 gold");
        mvwprintw(stdscr, restore_hp.y - 1, restore_hp.x - 3, "ПОДЛЕЧИТЬСЯ");
        mvwprintw(stdscr, restore_hp.y, restore_hp.x, "x");
        
        mvwprintw(stdscr, exit_magazine.y - 1, exit_magazine.x - 2, "ВЫХОД");
        mvwprintw(stdscr, exit_magazine.y, exit_magazine.x, "x");
        
        pl->game_movement(action);
        
        if (pl->x == dmg_up.x && pl->y == dmg_up.y && pl->cur_gold >= 10) {
            pl->dmg = pl->dmg + 5;
            pl->cur_gold = pl->cur_gold - 10;
        }
        if (pl->x == restore_hp.x && pl->y == restore_hp.y && pl->cur_gold >= 10) {
            pl->cur_hp = pl->max_hp;
            pl->cur_gold = pl->cur_gold - 10;
        }
        if(pl->x == exit_magazine.x && pl->y == exit_magazine.y) {
            is_complete = true;
        }
    }
    
    
    pl->x = checkpoint_pl.x + 1;
    pl->y = checkpoint_pl.y;
}

// рассчитывание коридоров между комнатами
void calc_coridors(room old, room neww) {
    int neww_center_x, neww_center_y, old_center_x, old_center_y;
    //  центры комнат по х и у
    neww_center_x = neww.x + (neww.size_x / 2);
    neww_center_y = neww.y + (neww.size_y / 2);
    old_center_x = old.x + (old.size_x / 2);
    old_center_y = old.y + (old.size_y / 2);
    
    int y;
    for (y = old_center_y; y != neww_center_y; ) {
        
        map[old_center_x][y] = ' ';
        
        if (map_vision[old_center_x][y] == 1) {
            mvaddch(y, old_center_x, ' ');
        } else {
            mvaddch(y, old_center_x, '.');
        }
        
        if (old_center_y < neww_center_y) {
            y++;
        } else {
            y--;
        }
    }
    
    for (int x = old_center_x; x != neww_center_x; ) {
        map[x][y] = ' ';
        
        if (map_vision[x][y] == 1) {
            mvaddch(y, x, ' ');
        } else {
            mvaddch(y, x, '.');
        }
        
        
        if (old_center_x < neww_center_x) {
            x++;
        } else {
            x--;
        }
    }
    
    
}

void draw_all(int rows, int cols, room start, room lvl1, room lvl2, room lvl3, room lvl4, coord quest, coord restart, coord magazine, player pl, mob mob_1, mob mob_2, mob mob_3, mob mob_4, mob mob_5) {
    //отрисовываем карту
    draw_walls(rows, cols); // стены
    start.draw_room(rows, cols); // комната
    calc_coridors(start, lvl1); // коридор
    lvl1.draw_room(rows, cols);
    calc_coridors(lvl1, lvl2);
    lvl2.draw_room(rows, cols);
    calc_coridors(lvl2, lvl3);
    lvl3.draw_room(rows, cols);
    calc_coridors(lvl3, lvl4);
    lvl4.draw_room(rows, cols);
    // calc_coridors(lvl4, start);
    draw_quest(quest); // quest
    draw_magazine(magazine);
    draw_restart(restart); // переход на след этаж
    pl.draw_stats(rows, cols); // состояние игрока и мира
    mob_1.draw_mob(pl); // Рисуем моба
    mob_2.draw_mob(pl);
    mob_3.draw_mob(pl);
    mob_4.draw_mob(pl);
    mob_5.draw_mob(pl);
    mvaddch(pl.y, pl.x, '@'); // Необходимо для корректной покраски мобов
};


void init_floor(int rows, int cols, room *start, room *lvl1, room *lvl2, room *lvl3, room *lvl4, obj *quest, obj *restart, obj *magazine, mob *mob_1, mob *mob_2, mob *mob_3, mob *mob_4, mob *mob_5, player *pl) {
    // добавляем стены в массив
    fill_map(rows, cols);
    init_map_vision(rows, cols);
    
    // // random spawn + добавляем комнаты в массив
    start->calc_room_coord(rows, cols);
    lvl1->calc_room_coord(rows, cols);
    lvl2->calc_room_coord(rows, cols);
    lvl3->calc_room_coord(rows, cols);
    lvl4->calc_room_coord(rows, cols);
    
    magazine->calc_obj_coord(*lvl2);
    quest->calc_obj_coord(*lvl1); // рандомим координаты квеста
    restart->calc_obj_coord(*lvl4); // рандомим координаты перехода на след этаж
    mob_1->spawn_mob(*start);  // рандомим координаты моба
    mob_2->spawn_mob(*lvl1);
    mob_3->spawn_mob(*lvl2);
    mob_4->spawn_mob(*lvl3);
    mob_5->spawn_mob(*lvl4);
    pl->spawn_player(*start);
    
    
};


void menu_start(int rows, int cols, player *pl, bool *is_playing_game) {
    coord start_game , exit_game;
    bool is_complete = false;
    int action;
    
    pl->x = rows / 2;
    pl->y = cols / 2;
    
    start_game.x = (rows / 2) - 20;
    start_game.y = (cols / 2) + 2;
    
    exit_game.x = (rows / 2) + 20;
    exit_game.y = (cols / 2) + 2;
    
    while (!is_complete && (action = getch()) != 27) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                mvaddch(j, i, ' ');
            }
        }
        wattron(stdscr, A_BOLD);
        mvwprintw(stdscr, 22, 95, "БЕГУУЩИЙ К ЗАЧЕТУ");
        wattroff(stdscr, A_BOLD);
        
        mvwprintw(stdscr, start_game.y, start_game.x, "x");
        mvwprintw(stdscr, start_game.y - 1, start_game.x - 5, "START GAME");
        mvwprintw(stdscr, start_game.y - 2, start_game.x - 5, "GIGACHAD :");
        
        mvwprintw(stdscr, exit_game.y, exit_game.x, "x");
        mvwprintw(stdscr, exit_game.y - 1, exit_game.x - 5, "EXIT GAME");
        mvwprintw(stdscr, exit_game.y - 2, exit_game.x - 5, "VIRGIN :");
        
        mvprintw(15, 75, "______ _____ _____ _   _ _____ _     _____ _   __ _____ \n");
        mvprintw(16, 75, "| ___ \\  _  |  __ \\ | | |  ___| |   |_   _| | / /|  ___|\n");
        mvprintw(17, 75, "| |_/ / | | | |  \\/ | | | |__ | |     | | | |/ / | |__  \n");
        mvprintw(18, 75, "|    /| | | | | __| | | |  __|| |     | | |    \\ |  __| \n");
        mvprintw(19, 75, "| |\\ \\\\ \\_/ / |_\\ \\ |_| | |___| |_____| |_| |\\  \\| |___ \n");
        mvprintw(20, 75, "\\_| \\_|\\___/ \\____/\\___/\\____/\\_____/\\___/\\_| \\_/\\____/ \n");
        
        pl->game_movement(action);
        
        if(pl->x == start_game.x && pl->y == start_game.y) {
            *is_playing_game = true;
            is_complete = true;
        }
        
        if(pl->x == exit_game.x && pl->y == exit_game.y) {
            *is_playing_game = false;
            is_complete = true;
        }
    }
    
    
    
}

void end_game(int rows, int cols, player *pl) {
    coord restart_game;
    std::string mesg1 = "__   _______ _   _  ______ _____ ___________ \n";
    std::string mesg2 = "\\ \\ / /  _  | | | | |  _  \\_   _|  ___|  _  \\\n";
    std::string mesg3 = " \\ V /| | | | | | | | | | | | | | |__ | | | |\n";
    std::string mesg4 = "  \\ / | | | | | | | | | | | | | |  __|| | | |\n";
    std::string mesg5 = "  | | \\ \\_/ / |_| | | |/ / _| |_| |___| |/ / \n";
    std::string mesg6 = "  \\_/  \\___/ \\___/  |___/  \\___/\\____/|___/  \n";
    std::string mesg7 = "        Поставьте пожалуйста 6 балов";
    
    bool is_complete = false;
    int action;
    pl->x = rows / 2;
    pl->y = cols / 2;
    
    pl->is_alive = true;
    pl->cur_hp = 100;
    
    pl->floor_counter = 1;
    restart_game.x = (rows / 2);
    restart_game.y = (cols / 2) + 7;
    
    while (!is_complete && (action = getch()) != 27) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols + 20; j++) {
                mvaddch(j, i, ' ');
            }
        }
        mvwprintw(stdscr, restart_game.y, restart_game.x, "x");
        mvwprintw(stdscr, restart_game.y - 1, restart_game.x - 1, "OK");
        mvwprintw(stdscr, (cols - 31) / 2, (rows - 40) / 2, "%s", mesg1.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 1, (rows - 40) / 2, "%s", mesg2.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 2, (rows - 40) / 2, "%s", mesg3.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 3, (rows - 40) / 2, "%s", mesg4.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 4, (rows - 40) / 2, "%s", mesg5.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 5, (rows - 40) / 2, "%s", mesg6.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 6, (rows - 40) / 2, "%s", mesg7.c_str());
        
        pl->game_movement(action);
        
        if(pl->x == restart_game.x && pl->y == restart_game.y) {
            is_complete = true;
        }
    }
}

void end_win(int rows, int cols, player *pl) {
    coord restart_game;
    std::string mesg1 = "__   _______ _   _   _    _ _____ _   _ _\n";
    std::string mesg2 = "\\ \\ / /  _  | | | | | |  | |_   _| \\ | | |\n";
    std::string mesg3 = " \\ V /| | | | | | | | |  | | | | |  \\| | |\n";
    std::string mesg4 = "  \\ / | | | | | | | | |/\\| | | | | . ` | |\n";
    std::string mesg5 = "  | | \\ \\_/ / |_| | \\  /\\  /_| |_| |\\  |_|\n";
    std::string mesg6 = "  \\_/  \\___/ \\___/   \\/  \\/ \\___/\\_| \\_(_)\n\n";
    std::string mesg7 = "          Made by Zhukov & Sivokhin\n";

    bool is_complete = false;
    int action;
    pl->x = rows / 2;
    pl->y = cols / 2;

    pl->is_alive = true;
    pl->cur_hp = 100;

    pl->floor_counter = 1;
    restart_game.x = (rows / 2);
    restart_game.y = (cols / 2) + 7;

    while (!is_complete && (action = getch()) != 27) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols + 20; j++) {
                mvaddch(j, i, ' ');
            }
        }
        mvwprintw(stdscr, restart_game.y, restart_game.x, "x");
        mvwprintw(stdscr, restart_game.y - 1, restart_game.x - 1, "OK");
        mvwprintw(stdscr, (cols - 31) / 2, (rows - 40) / 2, "%s", mesg1.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 1, (rows - 40) / 2, "%s", mesg2.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 2, (rows - 40) / 2, "%s", mesg3.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 3, (rows - 40) / 2, "%s", mesg4.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 4, (rows - 40) / 2, "%s", mesg5.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 5, (rows - 40) / 2, "%s", mesg6.c_str());
        mvwprintw(stdscr, ((cols - 31) / 2) + 6, (rows - 40) / 2, "%s", mesg7.c_str());

        pl->game_movement(action);

        if (pl->x == restart_game.x && pl->y == restart_game.y) {
            is_complete = true;
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    int action; // переменная для хранения нажатой клавиши
    int rows = 210, cols = 52; //  границы экрана
    // int rows, cols;
    bool is_playing_game = true;
    player pl(100, 5, 40); // игрок in dung
    room start, lvl1, lvl2, lvl3, lvl4; // комнаты
    obj quest; // quest
    obj restart; // переход нна след этаж
    obj magazine; // magazine
    mob mob_1(10, 1, 1, 99, 99, 99); //тестовый моб
    mob mob_2(10, 5, 1, 99, 99, 99); //тестовый моб
    mob mob_3(10, 1, 1, 99, 99, 99); //тестовый моб
    mob mob_4(10, 5, 1, 99, 99, 99); //тестовый моб
    mob mob_5(10, 2, 1, 99, 99, 99); //тестовый моб
    
    
    
    
    // mob_1.spawn_mob(start);
    
    
    // curses settings
    initscr();                    // start curses
    start_color();                    // colors
    keypad(stdscr, 1);            // allow arrows
    noecho();                     // dont dispay input
    curs_set(0);                  // hide cursor
    
    // getmaxyx(stdscr, cols, rows); // границы экрана(консоли)
    // getmaxyx(stdscr, rows, cols); // границы экрана(консоли)
    
    
    
    system("clear");
    
    menu_start(rows, cols, &pl, &is_playing_game);
    init_floor(rows, cols, &start, &lvl1, &lvl2, &lvl3, &lvl4, &quest, &restart, &magazine, &mob_1, &mob_2, &mob_3, &mob_4, &mob_5, &pl); // рандомим этаж
    
    
    while(is_playing_game) {
        
        //передвижение по карте
        while((action = getch()) != 27 && pl.is_alive && pl.floor_counter != 5) {
            
            if (pl.floor_counter == 2) {
                    end_win(rows, cols, &pl);
                    menu_start(rows, cols, &pl, &is_playing_game);
                    init_floor(rows, cols, &start, &lvl1, &lvl2, &lvl3, &lvl4, &quest, &restart, &magazine, &mob_1, &mob_2, &mob_3, &mob_4, &mob_5, &pl);
                  }
            // отрисовываем все обьекты
            draw_all(rows, cols, start, lvl1, lvl2, lvl3, lvl4, quest, restart, magazine, pl, mob_1, mob_2, mob_3, mob_4, mob_5);
            
            pl.map_movement(action); // поведение игрока
            
            mob_1.behavior_bot(&pl, action); // поведение бота
            mob_1.taking_damage_and_death(&pl, action); // Получение урона ботом от игрока
            
            mob_2.behavior_bot(&pl, action); // поведение бота
            mob_2.taking_damage_and_death(&pl, action); // Получение урона ботом от игрока
            
            mob_3.behavior_bot(&pl, action); // поведение бота
            mob_3.taking_damage_and_death(&pl, action); // Получение урона ботом от игрока
            
            mob_4.behavior_bot(&pl, action); // поведение бота
            mob_4.taking_damage_and_death(&pl, action); // Получение урона ботом от игрока
            
            mob_5.behavior_bot(&pl, action); // поведение бота
            mob_5.taking_damage_and_death(&pl, action); // Получение урона ботом от игрока
            
            draw_all(rows, cols, start, lvl1, lvl2, lvl3, lvl4, quest, restart, magazine, pl, mob_1, mob_2, mob_3, mob_4, mob_5);
            
            
            if(pl.x == quest.x && pl.y == quest.y) {
                quest = start_quest(rows, cols, &pl);
            }
            if(pl.x == magazine.x && pl.y == magazine.y) {
                enter_magazine(rows, cols, &pl);
            }
            
            if(pl.x == restart.x && pl.y == restart.y) {
                init_floor(rows, cols, &start, &lvl1, &lvl2, &lvl3, &lvl4, &quest, &restart, &magazine, &mob_1, &mob_2, &mob_3, &mob_4, &mob_5, &pl); // рандомим этаж
                pl.floor_counter++; // рандомим этаж и увеличениее счетчика этажа
            }
            
        } ; // 27 - escape - leave from cycle and while player is alive
        
        end_game(rows, cols, &pl);
        menu_start(rows, cols, &pl, &is_playing_game);
        init_floor(rows, cols, &start, &lvl1, &lvl2, &lvl3, &lvl4, &quest, &restart, &magazine, &mob_1, &mob_2, &mob_3, &mob_4, &mob_5, &pl); // рандомим этаж
        
    }
    
    
    
    
    endwin(); // end curses
    return 0;
}
