/*
    Overcooked OS
    Giovanni Shibaki Camargo        11796444
    Lucas Keiti Anbo Mihara         11796472
    Pedro Kenzo Muramatsu Carmo     11796451
*/

#ifndef COLOURS_H
#define COLOURS_H

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

int is_bold(int fg);
void init_colorpairs(void);
short curs_color(int fg);
int colornum(int fg, int bg);
void setcolor(int fg, int bg);
void unsetcolor(int fg, int bg);

void init_colorpairs(void)
{
    int fg, bg;
    int colorpair;

    for (bg = 0; bg <= 7; bg++)
    {
        for (fg = 0; fg <= 7; fg++)
        {
            colorpair = colornum(fg, bg);
            init_pair(colorpair, curs_color(fg), curs_color(bg));
        }
    }
}

short curs_color(int fg)
{
    switch (7 & fg)
    {       /* RGB */
    case 0: /* 000 */
        return (COLOR_BLACK);
    case 1: /* 001 */
        return (COLOR_BLUE);
    case 2: /* 010 */
        return (COLOR_GREEN);
    case 3: /* 011 */
        return (COLOR_CYAN);
    case 4: /* 100 */
        return (COLOR_RED);
    case 5: /* 101 */
        return (COLOR_MAGENTA);
    case 6: /* 110 */
        return (COLOR_YELLOW);
    case 7: /* 111 */
        return (COLOR_WHITE);
    }

    return COLOR_WHITE;
}

int colornum(int fg, int bg)
{
    int B, bbb, ffff;

    B = 1 << 7;
    bbb = (7 & bg) << 4;
    ffff = 7 & fg;

    return (B | bbb | ffff);
}

/*
                    To create a predictable color pair number for each foreground and background color, 
                    I also need a function colornum() to set an integer bit pattern based on the classic color byte:
                */
void setcolor(int fg, int bg)
{
    /* set the color pair (colornum) and bold/bright (A_BOLD) */

    attron(COLOR_PAIR(colornum(fg, bg)));
    if (is_bold(fg))
    {
        attron(A_BOLD);
    }
}

void unsetcolor(int fg, int bg)
{
    /* unset the color pair (colornum) and
                bold/bright (A_BOLD) */

    attroff(COLOR_PAIR(colornum(fg, bg)));
    if (is_bold(fg))
    {
        attroff(A_BOLD);
    }
}

/*
                simply tests if the "intensifier" bit on the iRGB value (foreground colors 8 to 15) is set,
                using a simple bit mask
                */
int is_bold(int fg)
{
    /* return the intensity bit */

    int i;

    i = 1 << 3;
    return (i & fg);
}
#endif