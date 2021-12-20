/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/
// changing Library according to OS
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#define sleep usleep
#endif

#include "../lib/grill.hpp"
#include "../lib/blender.hpp"
#include "../lib/iceCream.hpp"
#include "../lib/order.hpp"
#include "../lib/orderManager.hpp"
#include "../lib/printManager.hpp"
#include "../lib/colours.hpp"

#include <bits/stdc++.h>
#include <ncurses.h>
#include <cstring>

typedef int (*NCURSES_WINDOW_CB)(WINDOW *, void *);
typedef int (*NCURSES_SCREEN_CB)(SCREEN *, void *);

int get_escdelay(void);
int set_escdelay(int ms);
int set_tabsize(int cols);

int use_screen(SCREEN *scr, NCURSES_SCREEN_CB func, void *data);
int use_window(WINDOW *win, NCURSES_WINDOW_CB func, void *data);

#define ESC 27

#define GREEN_BLACK 1
#define RED_BLACK 2
#define YELLOW_BLACK 3

using namespace std;

Electricity elect;
OrderManager ordMan;
PrintManager printMan;
vector<Gadget*> gadgetsVec;
int score = 0;
bool hasTime = true;

// Threads
pthread_t energyWindow;
pthread_t timerWindow;
pthread_t orderWindow;
pthread_t gadgetWindow;

static void *updateEnergy(void *arg)
{
    WINDOW *screen = (WINDOW *)arg;
    while (hasTime)
    {
        sleep(500000);
        printMan.printEnergy(elect.getCurEnergy(), screen);
    }
}

static void *updateTimer(void *arg)
{
    WINDOW *screen = (WINDOW *)arg;
    int time = 300;

    while (time >= 0)
    {
        sleep(1000000);
        printMan.printTimer(time, screen);
        time--;
    }
    
    elect.stop();
    ordMan.stop();
    hasTime = false;
    sleep(400000);

    // Print the end screen
    printMan.printEndScreen(score);
    
    return NULL;
}

// static void *updateScore(void *arg)
// {
//     WINDOW *screen = (WINDOW *)arg;

//     while (1)
//     {
//         sleep(1000000);
//         printMan.printScore(score, screen);
//     }
//     return NULL;
// }

static void *updateOrder(void *arg)
{
    WINDOW *screen = (WINDOW *)arg;

    while (hasTime)
    {
        vector<Order> ords = ordMan.getOrders();
        printMan.printOrders(ords, screen);
        sleep(400000);
    }

    return NULL;
}

static void *updateGadget(void *arg)
{
    sleep(100000);
    vector<WINDOW *> *screen = (vector<WINDOW*> *)arg;

    while (hasTime)
    {
        for(int i = 0; i < gadgetsVec.size(); i++)
        {
            printMan.printGadget(gadgetsVec.at(i), screen->at(i));
        }
        sleep(1000000);
    }
    
    return NULL;
}

void activateGadget(Gadget *gad, WINDOW *scoreWin)
{
    bool state = gad->activate();
    if(state == true)
    {
        if(gad->getIdGadget() == 0)
        {
            // Decrease one hamburguer from the order list
            if(ordMan.decreaseGrill())
            {
                score += 10;
                printMan.printScore(score, scoreWin);
            }
        } else if(gad->getIdGadget() == 1)
        {
            // Decrease one juice from the order list
            if(ordMan.decreaseBlender())
            {
                score += 10;
                printMan.printScore(score, scoreWin);
            }
        }
        else
        {
            // Decrease one ice cream from the order list
            if(ordMan.decreaseIceCream())
            {
                score += 10;
                printMan.printScore(score, scoreWin);
            }
        }
    }
}

int main()
{
    //----------------------INITIALIZING RESOURCES ------------------------------
    score = 0;

    elect.start();
    ordMan.start();

    // Initializa ncurses window
    WINDOW *wnd = initscr(); // Init the screen

    // ---------------------------------- INITIALIZING COLOURS -------------------------
    int fgColour, bgColour;

    if (has_colors() == FALSE)
    {
        endwin();
        puts("Your terminal does not support color");
        exit(1);
    }

    start_color();
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_colorpairs();


    // -----------------------DRAWING UI ELEMENTS INTO SCREEN------------------------

    raw();                // Disable the need of /n in the end of input
    noecho();             // Do not show characters wtritten by the userm in the terminal
    keypad(stdscr, TRUE); // Enable the input of arrows and F1, F2 ...
    refresh();

    // Create all windows for the game
    mvprintw(0, 1, "Time");
    WINDOW *winTime = printMan.create_newwin(3, 15, 1, 0);

    printMan.draw_borders(winTime);

    mvprintw(0, 106, "Score");
    WINDOW *winScore = printMan.create_newwin(3, 15, 1, 105);
    printMan.draw_borders(winScore);
    printMan.printScore(score, winScore);

    mvprintw(0, 34, "Energy");
    WINDOW *winEnergy = printMan.create_newwin(3, 52, 1, 33);
    printMan.draw_borders(winEnergy);

    mvprintw(4, 1, "Orders");
    WINDOW *winOrders = printMan.create_newwin(6, 120, 5, 0);
    printMan.draw_borders(winOrders);

    vector<WINDOW *> winGadgets;

    int j = 1;
    for (int i = 0; i <= 90; i += 30)
    {
        winGadgets.emplace_back(printMan.create_newwin(9, 29, 11, i));
        mvwprintw(winGadgets.back(), 1, 1, ("[" + to_string(j++) + "]").c_str());
        printMan.draw_borders(winGadgets.back());
    }
    for (int i = 0; i <= 90; i += 30)
    {
        winGadgets.emplace_back(printMan.create_newwin(9, 29, 20, i));
        mvwprintw(winGadgets.back(), 1, 1, ("[" + to_string(j++) + "]").c_str());
        printMan.draw_borders(winGadgets.back());
    }

    // ---------------------- MANAGING THREADS ------------------
    Grill grill1 = Grill(elect);
    gadgetsVec.emplace_back(&grill1);
    Grill grill2 = Grill(elect);
    gadgetsVec.emplace_back(&grill2);
    Blender blender1 = Blender(elect);
    gadgetsVec.emplace_back(&blender1);
    IceCream iceCream1 = IceCream(elect);
    gadgetsVec.emplace_back(&iceCream1);

    Grill grill3 = Grill(elect);
    gadgetsVec.emplace_back(&grill3);
    Grill grill4 = Grill(elect);
    gadgetsVec.emplace_back(&grill4);
    Blender blender2 = Blender(elect);
    gadgetsVec.emplace_back(&blender2);
    IceCream iceCream2 = IceCream(elect);
    gadgetsVec.emplace_back(&iceCream2);

    // Detach Atribute for all threads
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Energy window
    pthread_create(&energyWindow, &attr, updateEnergy, winEnergy);

    // Timer window
    pthread_create(&timerWindow, &attr, updateTimer, winTime);

    // Score window
    /*pthread_t scoreWindow;
    pthread_create(&scoreWindow, &attr, updateScore, winScore);*/

    // Orders window
    pthread_create(&orderWindow, &attr, updateOrder, winOrders);

    // Gadgets windows
    pthread_create(&gadgetWindow, &attr, updateGadget, &winGadgets);
    
    pthread_attr_destroy(&attr);

    // -------------- USER INTERACTION  -------------------
    while (hasTime)
    {
        char key = getch();
        if(key >= '1' && key <='8' && hasTime)
        {
            activateGadget(gadgetsVec.at(key - '1'), winScore);
        }
        else if((key == 'q' || key == 'Q'))
        {
            endwin();
            exit(0);
        }
    }

    endwin();
    return 0;
}
