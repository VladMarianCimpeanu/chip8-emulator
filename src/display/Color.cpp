#include <assert.h>
#include "display.h"


namespace chip8::display {
    
    Color::Color(int r, int g, int b) {
        assert( r <= 255 && r >= 0);
        assert( g <= 255 && g >= 0);
        assert( b <= 255 && b >= 0);

        this->r_ = r;
        this->g_ = g;
        this->b_ = b;
    }

    int Color::r() { return this->r_;}
    int Color::g() { return this->g_;}
    int Color::b() { return this->b_;}

    bool Color::is_white() {
        return this->r_ == 255 && this->b_ == 255 && this->g_ == 255;
    }

}      