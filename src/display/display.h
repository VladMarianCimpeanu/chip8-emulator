#ifndef DISPLAY_HEADER
#define DISPLAY_HEADER

#include <ncurses.h>

namespace chip8::display {

    class Color {
        public:
            Color(int r, int g, int b);
            int r();
            int g();
            int b();
            bool is_white();
        private:
            int r_, g_, b_;
    };
    
    class IDisplay{
        public:
            virtual ~IDisplay() = default;
            virtual void clear_screen() = 0;
            virtual void draw_rectangle(int x, int y, int w, int h, Color color) = 0;
    };

}

#endif