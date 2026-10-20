#include <pdcurses.h>
#include <windows.h>
#include <iostream>

int main(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < 3 || cols < 3) {
        endwin();
        std::cerr << "La consola es demasiado pequeña para mostrar la ventana.\n";
        return 1;
    }

    int height = rows < 20 ? rows : 20;
    int width = cols < 60 ? cols : 60;
    WINDOW* W = newwin(height, width, 0, 0);
    SetConsoleTitleA("ale es tonto");
    if (W == nullptr) {
        endwin();
        std::cerr << "No se pudo crear la ventana de curses.\n";
        return 1;
    }
    keypad(W, TRUE);

    bool exit = false;

    int center_y = height/2;

    int player1 = center_y - 2;

    while (!exit) {
        curs_set(0);

        wclear(W);

        box(W, '|', '-');
        mvwvline(W, player1, 1, '0', 4);

        wrefresh(W);

        int enter = wgetch(W);

        switch (enter) {
            case 27:
                exit = true;
                break;
            case KEY_DOWN:
                if(height - 2 != player1 + 3) {
                    player1++;
                }
                break;
            case KEY_UP:
                if(1 != player1) {
                    player1--;
                }
                break;
        }
    }

    endwin();

    return 0;
}