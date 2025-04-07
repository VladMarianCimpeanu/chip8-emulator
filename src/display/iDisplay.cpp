#include "display.h"


namespace chip8::display {
    
    class IDisplay {
        
        public:
            virtual void reset() {};
            virtual void draw_rectangle(int x, int y, int w, int h, Color color);

    };

}
