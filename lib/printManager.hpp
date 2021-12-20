/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef PRINT_MANAGER_H
#define PRINT_MANAGER_H

#include <bits/stdc++.h>
#include <mutex>
#include "order.hpp"
#include "gadget.hpp"

using namespace std;

// changing Library according to OS
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#define sleep usleep
#endif

#include <ncurses.h>
#include <cstring>

typedef int (*NCURSES_WINDOW_CB)(WINDOW *, void *);
typedef int (*NCURSES_SCREEN_CB)(SCREEN *, void *);

int get_escdelay(void);
int set_escdelay(int ms);
int set_tabsize(int cols);

int use_screen(SCREEN *scr, NCURSES_SCREEN_CB func, void *data);
int use_window(WINDOW *win, NCURSES_WINDOW_CB func, void *data);

#define GREEN_BLACK 1
#define RED_BLACK 2
#define YELLOW_BLACK 3

class PrintManager
{
private:
    mutex mtxRefresh;

public:
    PrintManager()
    {
        start_color();
        init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
        init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
        init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
    }
    WINDOW *create_newwin(int height, int width, int starty, int startx)
    {
        WINDOW *local_win;

        local_win = newwin(height, width, starty, startx);
        box(local_win, 0, 0); /* 0, 0 dá caracteres padrão para as linhas verticais and horizontais	*/
        wrefresh(local_win);  /* Mostra aquela caixa 	*/

        return local_win;
    }

    void destroy_win(WINDOW *local_win)
    {
        /* box (local_win, '', ''); : Isso não produzirá o resultado
         *  desejado de apagar a janela. Vai deixar seus quatro cantos,
         * e uma lembrança feia da janela.
         */
        wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        /* Os parâmetros usados são
         * 1. win: a janela na qual operar
         * 2. ls: caractere a ser usado para o lado esquerdo da janela
         * 3. rs: caractere a ser usado para o lado direito da janela
         * 4. ts: caractere a ser usado na parte superior da janela
         * 5. bs: caractere a ser usado na parte inferior da janela
         * 6. tl: caractere a ser usado para o canto superior esquerdo da janela
         * 7. tr: caractere a ser usado no canto superior direito da janela
         * 8. bl: caractere a ser usado no canto inferior esquerdo da janela
         * 9. br: caractere a ser usado no canto inferior direito da janela
         */
        mtxRefresh.lock();
        wrefresh(local_win);
        mtxRefresh.unlock();
        delwin(local_win);
    }

    void draw_borders(WINDOW *screen)
    {
        int x, y, i;
        getmaxyx(screen, y, x);

        // 4 corners
        mvwprintw(screen, 0, 0, "+");
        mvwprintw(screen, y - 1, 0, "+");
        mvwprintw(screen, 0, x - 1, "+");
        mvwprintw(screen, y - 1, x - 1, "+");

        // sides
        for (i = 1; i < (y - 1); i++)
        {
            mvwprintw(screen, i, 0, "|");
            mvwprintw(screen, i, x - 1, "|");
        }

        // top and bottom
        for (i = 1; i < (x - 1); i++)
        {
            mvwprintw(screen, 0, i, "-");
            mvwprintw(screen, y - 1, i, "-");
        }

        mtxRefresh.lock();
        wrefresh(screen);
        mtxRefresh.unlock();
    }

    void printGadget(Gadget *gad, WINDOW *screen)
    {
        for (int i = 0; i <= 7; i++)
            mvwprintw(screen, 7 - i, 27, " ");

        if (gad->getEnergyGadget() == 0)
            wattron(screen, COLOR_PAIR(GREEN_BLACK));
        else
            wattron(screen, COLOR_PAIR(RED_BLACK));

        for (int i = 0; i < (gad->getDefaultEnergy() - gad->getEnergyGadget()); i++)
        {
            mvwprintw(screen, 7 - i, 27, "#");
        }

        if (gad->getEnergyGadget() == 0)
            wattroff(screen, COLOR_PAIR(GREEN_BLACK));
        else
            wattroff(screen, COLOR_PAIR(RED_BLACK));

        vector<string> sprite = gad->print();
        int i = 1;

        for (string text : sprite)
        {
            mvwprintw(screen, i++, 4, text.c_str());
        }

        mtxRefresh.lock();
        wrefresh(screen);
        mtxRefresh.unlock();
    }

    void printTimer(int time, WINDOW *screen)
    {
        mvwprintw(screen, 1, 1, "   ");

        if (time <= 25 && time > 10)
            wattron(screen, COLOR_PAIR(YELLOW_BLACK));
        if (time <= 10)
            wattron(screen, COLOR_PAIR(RED_BLACK));

        mvwprintw(screen, 1, 1, to_string(time).c_str());

        if (time <= 25 && time > 10)
            wattroff(screen, COLOR_PAIR(YELLOW_BLACK));
        if (time <= 10)
            wattroff(screen, COLOR_PAIR(RED_BLACK));

        mtxRefresh.lock();
        wrefresh(screen);
        mtxRefresh.unlock();
    }

    void printScore(int score, WINDOW *screen)
    {
        mvwprintw(screen, 1, 1, "   ");
        mvwprintw(screen, 1, 1, to_string(score).c_str());
        mtxRefresh.lock();
        wrefresh(screen);
        mtxRefresh.unlock();
    }

    void printEnergy(int energy, WINDOW *screen)
    {
        mvwprintw(screen, 1, 1, "                                                  ");
        wattron(screen, COLOR_PAIR(YELLOW_BLACK));
        for (int i = 0; i < (energy / 2) && i < 50; i++)
        {
            mvwprintw(screen, 1, i + 1, "#");
        }
        wattroff(screen, COLOR_PAIR(YELLOW_BLACK));
        mtxRefresh.lock();
        wrefresh(screen);
        mtxRefresh.unlock();
    }

    void printOrders(vector<Order> ords, WINDOW *screen)
    {
        for (int i = 0; i < 4; i++)
            mvwprintw(screen, i + 1, 1, "                                                                                                                      ");

        int aux = 1;
        for (Order i : ords)
        {
            mvwprintw(screen, 1, aux, "New Order");
            mvwprintw(screen, 2, aux, (to_string(i.getNumGrill()) + " Hamburgers").c_str());
            mvwprintw(screen, 3, aux, (to_string(i.getNumBlender()) + " Juices").c_str());
            mvwprintw(screen, 4, aux, (to_string(i.getNumIceCream()) + " Ice Creams").c_str());
            aux += 17;
        }
        mtxRefresh.lock();
        wrefresh(screen);
        mtxRefresh.unlock();
    }

    void printEndScreen(int score)
    {
        // Clear the whole screen
        clear();
        mvprintw(1, 1, "FIM DE JOGO!");
        mvprintw(2, 1, "SEU SCORE FINAL E DE: ");
        mvprintw(3, 1, (to_string(score) + " Pontos!").c_str());

        mvprintw(10, 1, "Jogo desenvolvido por:");
        mvprintw(11, 1, "Giovanni Shibaki Camargo");
        mvprintw(12, 1, "Lucas Keiti Anbo Mihara");
        mvprintw(13, 1, "Pedro Kenzo Muramatsu Carmo");

        mvprintw(15, 1, "Para a materia de:");
        mvprintw(16, 1, "SSC0140 - Sistemas Operacionais");
        mvprintw(17, 1, "Profa. Dra. Kalinka Castelo Branco");

        mvprintw(19, 1, "ICMC - USP - 2021");

        mvprintw(21, 1, "Pressione qualquer tecla para continuar...");

        mvprintw(1, 40, "                  .88888888:.              ");
        mvprintw(2, 40, "                88888888.88888.             ");
        mvprintw(3, 40, "              .8888888888888888.           ");
        mvprintw(4, 40, "              888888888888888888           ");
        mvprintw(5, 40, "              88' _`88'_  `88888           ");
        mvprintw(6, 40, "              88 88 88 88  88888           ");
        mvprintw(7, 40, "              88_88_::_88_:88888           ");
        mvprintw(8, 40, "              88:::,::,:::::8888           ");
        mvprintw(9, 40, "              88`:::::::::'`8888           ");
        mvprintw(10, 40, "             .88  `::::'    8:88.          ");
        mvprintw(11, 40, "            8888            `8:888.        ");
        mvprintw(12, 40, "          .8888'             `888888.      ");
        mvprintw(13, 40, "         .8888:..  .::.  ...:'8888888:.    ");
        mvprintw(14, 40, "        .8888.'     :'     `'::`88:88888   ");
        mvprintw(15, 40, "       .8888        '         `.888:8888.  ");
        mvprintw(16, 40, "      888:8         .           888:88888  ");
        mvprintw(17, 40, "    .888:88        .:           888:88888: ");
        mvprintw(18, 40, "    .888:88        .:           888:88888: ");
        mvprintw(19, 40, "    8888888.       ::           88:888888  ");
        mvprintw(20, 40, "    `.::.888.      ::          .88888888   ");
        mvprintw(21, 40, "   .::::::.888.    ::         :::`8888'.:. ");
        mvprintw(22, 40, "  ::::::::::.888   '         .:::::::::::: ");
        mvprintw(23, 40, "  ::::::::::::.8    '      .:8::::::::::::.");
        mvprintw(24, 40, " .::::::::::::::.        .:888:::::::::::::");
        mvprintw(25, 40, " :::::::::::::::88:.__..:88888:::::::::::' ");
        mvprintw(26, 40, "  `'.:::::::::::88888888888.88:::::::::'   ");
        mvprintw(27, 40, "        `':::_:' -- '' -'-' `':_::::'`      ");
        
        refresh();
    }

};

#endif