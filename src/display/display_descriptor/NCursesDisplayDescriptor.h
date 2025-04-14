#ifndef NCURSES_DESCRIPTOR
#define NCURSES_DESCRIPTOR

#include "IDisplayCliDescriptor.h"
#include <ncurses.h>

namespace display_descriptor {

    class NCursesDisplayDescriptor: public IDisplayCliDescriptor {

        private:
            WINDOW* ncurses_window;

        public:
            NCursesDisplayDescriptor();
            ~NCursesDisplayDescriptor();

            void print_string(const char* string_to_print) override;
            void refresh() override;
            void clear() override;


    };

}


#endif