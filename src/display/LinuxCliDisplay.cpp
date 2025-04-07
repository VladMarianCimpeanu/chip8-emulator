#include "display.h"
#include <ncurses.h>


namespace chip8::display{
    
    LinuxCliDisplay::LinuxCliDisplay() {
        this->window = initscr();
    }

    LinuxCliDisplay::~LinuxCliDisplay() {
        endwin();
    }

    void LinuxCliDisplay::reset() {

    }

    void LinuxCliDisplay::draw_rectangle(int  x, int y, int h, int w, Color color) {
        printw("Mock draw rectangle");
        refresh();
    }



}