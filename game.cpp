#include <pdcurses.h>
#include <string.h>
#include <windows.h>
#include <iostream>

/*
class place {
    public:
        int x1;
        int x2;
        int y1;
        int y2;
        char v[2];
        char h[2];
    
    place (const int X1, const int Y1, const int X2, const int Y2, const char *V, const char *H) {
        x1 = X1;
        x2 = X2;
        y1 = Y1;
        y2 = Y2;
        strcpy(v, V);
        strcpy(h, H);
    }
};
*/
struct line {

};


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
    WINDOW* W = newwin(height, width, rows/3, cols/2);
    SetConsoleTitleA("ale es tonto");
    if (W == nullptr) {
        endwin();
        std::cerr << "No se pudo crear la ventana de curses.\n";
        return 1;
    }
    keypad(W, TRUE);

    bool exit = false;

    while (!exit) {
        curs_set(0);

        wclear(W);

        box(W, '|', '-');
        mvwvline(W, 1, 1, '0', 4);

        wrefresh(W);

        int enter = wgetch(W);

        switch (enter) {
            case 27:
                exit = true;
                break;
        }
    }

    endwin();

    return 0;
}