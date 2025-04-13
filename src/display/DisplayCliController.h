#ifndef LINUX_CLI_HEADER
#define LINUX_CLI_HEADER


#include <vector>

#include "display.h"
#include "display_descriptor/IDisplayCliDescriptor.h"


namespace chip8::display{

    class DisplayCliController: public IDisplay {

        private:
            int window_width;
            int window_height;
            std::vector<std::vector<chip8::display::Color>> display_matrix;
            display_descriptor::IDisplayCliDescriptor& display_d;
            
            void refresh_display();
        

        public:
            DisplayCliController(
                int window_width,
                int window_height, 
                display_descriptor::IDisplayCliDescriptor& display_d
            );
            ~DisplayCliController();
            void clear_screen() override;
            void draw_rectangle(int x, int y, int w, int h, Color color) override;
    };

}

#endif