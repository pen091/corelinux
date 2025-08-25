#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int main() {
    initscr();              // start ncurses
    start_color();          // enable colors
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    box(stdscr, 0, 0);

    mvprintw(2, 2, "Simple CLI Location App (Demo)");
    mvprintw(4, 4, "City: Lagos");
    mvprintw(5, 4, "Latitude: 6.5244 N");
    mvprintw(6, 4, "Longitude: 3.3792 E");
    mvprintw(8, 4, "Press any key to exit...");
    attroff(COLOR_PAIR(1));

    refresh();
    getch(); // wait for user input

    endwin(); // end ncurses
    return 0;
}
