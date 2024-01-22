#include "player.h"
#include "room.h"


// передвежение игрока
void player::game_movement(int action) {
    switch (action) {
        case KEY_UP :
            y--;
            break;
            
        case KEY_DOWN :
            y++;
            break;
            
        case KEY_RIGHT :
            x++;
            break;
            
        case KEY_LEFT :
            x--;
            break;
            
        default:
            break;
    }
    mvaddch(y, x, '@'); // печать игрока по определенным координатам
};


void player::map_movement(int action) {
    switch (action) {
        case KEY_UP :
            // проверка чтобы не выйти на стену
            if (map[x][y - 1] == ' ') {
                y--;
                // вижн
                for (int i = x - 2; i < x + 3; i++) {
                    for (int j = y - 3; j < y + 2; j++) {
                        map_vision[i][j] = 1;
                    }
                }
            }
            mvwprintw(stdscr, 0, 1, "               Ничего интересного...                  ");
            break;
            
        case KEY_DOWN :
            if (map[x][y + 1] == ' ') {
                y++;
                for (int i = x - 2; i < x + 3; i++) {
                    for (int j = y - 2; j < y + 4; j++) {
                        map_vision[i][j] = 1;
                    }
                }
            }
            mvwprintw(stdscr, 0, 1, "               Ничего интересного...                  ");
            break;
            
        case KEY_RIGHT :
            if (map[x + 1][y] == ' ') {
                x++;
                for (int i = x; i < x + 4; i++) {
                    for (int j = y - 2; j < y + 3; j++) {
                        map_vision[i][j] = 1;
                    }
                }
            }
            mvwprintw(stdscr, 0, 1, "               Ничего интересного...                  ");
            break;
            
        case KEY_LEFT :
            if (map[x - 1][y] == ' ') {
                for (int i = x - 4; i < x + 1; i++) {
                    for (int j = y - 2; j < y + 3; j++) {
                        map_vision[i][j] = 1;
                    }
                }
                x--;
            }
            mvwprintw(stdscr, 0, 1, "               Ничего интересного...                  ");
            break;
            
        default:
            break;
    }
    mvaddch(y, x, '@'); // печать игрока по определенным координатам
};


// состояние игрока и мира
void player::draw_stats(int rows, int cols) {
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvwprintw(stdscr, cols - 1, 1, "Здоровье : %d(%d)    Монеты : %d   Броня : %d   Урон : %d", cur_hp, max_hp, cur_gold, armor, dmg);
    attroff(COLOR_PAIR(1));

    mvwprintw(stdscr, 0, 1, "Floor : %d", floor_counter);
    mvwprintw(stdscr, 53, cols - 50, "Легенда:");
    mvwprintw(stdscr, 54, cols - 50, "@ - Игрок");
    mvwprintw(stdscr, 55, cols - 50, "a - Монстр");
    mvwprintw(stdscr, 56, cols - 50, "0 - Лифт");
    mvwprintw(stdscr, 57, cols - 50, "! - Квест");
    mvwprintw(stdscr, 58, cols - 50, "# - Стена");
    mvwprintw(stdscr, 59, cols - 50, ". - Дым");
    mvwprintw(stdscr, 60, cols - 50, "$ - Магазин");
    
}

// спавн игрока
void player::spawn_player(room start) {
    x = start.x + 3;
    y = start.y + 3;
    
    // spawn vision
    for (int i = x - 3; i < x + 5; i++) {
        for (int j = y - 3; j < y + 5; j++) {
            map_vision[i][j] = 1;
        }
    }
    
};
