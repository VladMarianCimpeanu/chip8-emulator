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
        private:
            int r_, g_, b_;
    };
    
    class IDisplay{
        public:
            virtual ~IDisplay() = default;
            virtual void reset() = 0;
            virtual void draw_rectangle(int x, int y, int w, int h, Color color) = 0;
    };

    class LinuxCliDisplay: public IDisplay {

        private:
            WINDOW * window;

        public:
            LinuxCliDisplay();
            ~LinuxCliDisplay();
            void reset() override;
            void draw_rectangle(int x, int y, int w, int h, Color color) override;
    };

}

#endif