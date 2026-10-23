#include <pdcurses.h>
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <chrono>
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

bool enablecolorsANSI(DWORD exit) {
        HANDLE console = GetStdHandle(exit);
        DWORD mode;

        return GetConsoleMode(console, &mode) &&
            SetConsoleMode(
                console, 
                mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
            );
}

int main(void) {
    int win = 0;

    enablecolorsANSI(STD_OUTPUT_HANDLE);
    enablecolorsANSI(STD_ERROR_HANDLE);

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
        curs_set(0);

        wclear(W);

        box(W, '|', '-');
        mvwvline(W, player1, 1, '0', 4);
        mvwvline(W, player2, width - 2, '0', 4);
        mvwprintw(W, boll.y, boll.x, "o");

        if (win != 0) {
            if (win == 2) {
                mvwprintw(W, center_y, center_x - 10, "haz ganado jugador dos");
                mvwprintw(W, center_y + 1, center_x - 10, "Pulsa 'r' para volver a jugar");
            } else if (win == 1) {
                mvwprintw(W, center_y, center_x - 10, "haz ganado jugador uno");
                mvwprintw(W, center_y + 1, center_x - 10, "Pulsa 'r' para volver a jugar");
            }
        }

        wrefresh(W);

        if (boll.x == 2 || boll.x == 1) {
                for (int i = 0; i < 4; i++) {
                    if (boll.y == i + player1) {
                        boll.d[0] = 1;
                        std::cout << "toco\n";
                    }
                }
            } else if (boll.x == width - 3 || boll.x == width - 2) {
                for (int i = 0; i < 4; i++) {
                    if (boll.y == i + player2) {
                        boll.d[0] = 0;
                        std::cout << "toco\n";
                    }
                }
            }

        auto ahora = std::chrono::steady_clock::now();
        if (ahora - ultimoMovimiento >= std::chrono::milliseconds(65)) {
            // Mover la pelota según su dirección
            boll.x += boll.d[0] == 0 ? -1 : 1;
            boll.y += boll.d[1] == 0 ? -1 : 1;

            ultimoMovimiento = ahora;
            
            if (boll.y == 1 || boll.y == height - 2) {
                boll.d[1] = boll.d[1] == 0 ? 1 : 0;
            }
            
        }

    napms(16); // pausa breve; evita que el bucle corra sin límite

        int enter = wgetch(W);
        if (boll.x == 0) {
            win = 2;
        } else if (boll.x == width - 1) {
            win = 1;
        }

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
            case 'r':
                win = 3;
                exit = true;
                break;
        }
    }
    if (win == 3) {
        std::filesystem::path path_a = LR"(C:\Users\jmanm\OneDrive\Pong\a.exe)";
        std::filesystem::path path_pong = LR"(C:\Users\jmanm\OneDrive\Pong\Pong.exe)";
        if (std::filesystem::exists(path_pong)) {
            ShellExecuteW(
                nullptr,
                L"open",
                path_pong.c_str(),
                nullptr,
                path_pong.parent_path().c_str(),
                SW_SHOWNORMAL
            );
        } else if (std::filesystem::exists(path_a)) {
            ShellExecuteW(
                nullptr,
                L"open",
                path_a.c_str(),
                nullptr,
                path_a.parent_path().c_str(),
                SW_SHOW
            );
        }
        else {
            std::cerr << "ERROR: Al intentar executar el Pong";
        }
    }

    endwin();

    return 0;
}