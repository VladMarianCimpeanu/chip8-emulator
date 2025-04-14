#include <ncurses.h>

#include "NCursesDisplayDescriptor.h"

namespace display_descriptor {

    NCursesDisplayDescriptor::NCursesDisplayDescriptor() {
        this->ncurses_window = initscr();
    }

    NCursesDisplayDescriptor::~NCursesDisplayDescriptor() {
        delwin(this->ncurses_window);
        endwin();
    }

    void NCursesDisplayDescriptor::print_string(const char* string_to_print){
        printw(string_to_print);
    }

    void NCursesDisplayDescriptor::refresh() {
        wrefresh(this->ncurses_window);
    }
    void NCursesDisplayDescriptor::clear(){
        ::clear();
    }

}
