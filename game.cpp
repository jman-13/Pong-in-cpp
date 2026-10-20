#include <pdcurses.h>
#include <windows.h>
#include <iostream>
#include <random>
#include <chrono>


std::random_device semilla;
std::mt19937 generador(semilla());
std::uniform_int_distribution<int> elegir(0, 1);

class ball {
    public:
        int x;
        int y;
        int v;
        int d[2] = {elegir(generador), elegir(generador)};
        // d[0] = izquierda 0, derecha 1
        // d[1] = arriba 0, abajo 1
    ball(const int _y,const int _x) {
        x = _x;
        y = _y;
    }
};

int main(void) {
    float s = 0;
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
    int center_x = width/2;

    int player1 = center_y - 2;
    int player2 = center_y - 2;
    ball boll(center_y, center_x);

    nodelay(W, TRUE);
    
    auto ultimoMovimiento = std::chrono::steady_clock::now();
    while (!exit) {
        s += 0.0007;
        curs_set(0);

        wclear(W);

        box(W, '|', '-');
        mvwvline(W, player1, 1, '0', 4);
        mvwvline(W, player2, width - 2, '0', 4);
        mvwprintw(W, boll.y, boll.x, "o");

        wrefresh(W);
        

        if (s > 1) {
            if (boll.d[0] == 0) {
                boll.x--;
            } else {
                boll.x++;
            }
            if (boll.d[1] == 0) {
                boll.y--;
            } else {
                boll.y++;
            }
            s--;
        }
        auto ahora = std::chrono::steady_clock::now();
        if (ahora - ultimoMovimiento >= std::chrono::milliseconds(100)) {
            // Mover la pelota según su dirección
            boll.x += boll.d[0] == 0 ? -1 : 1;
            boll.y += boll.d[1] == 0 ? -1 : 1;

            ultimoMovimiento = ahora;
            
            if (boll.y == 1 || boll.y == height - 2) {
                boll.d[1] = boll.d[1] == 0 ? 1 : 0;
                std::cout << boll.d[1];
            }
        }

    napms(16); // pausa breve; evita que el bucle corra sin límite

        int enter = wgetch(W);

        switch (enter) {
            case 27:
                exit = true;
                break;
            case 's':
                if(height - 2 != player1 + 3) {
                    player1++;
                }
                break;
            case 'w':
                if(1 != player1) {
                    player1--;
                }
                break;
            case KEY_DOWN:
                if(height - 2 != player2 + 3) {
                    player2++;
                }
                break;
            case KEY_UP:
                if(1 != player2) {
                    player2--;
                }
                break;
        }
    }

    endwin();

    return 0;
}