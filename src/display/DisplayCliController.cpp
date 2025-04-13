#include <assert.h>
#include <string>
#include <vector>

#include "DisplayCliController.h"
#include "display.h"
#include "display_descriptor/IDisplayCliDescriptor.h"



namespace chip8::display{

    const std::string BLACK_CHAR = "$";
    const std::string WHITE_CHAR = ".";

    const chip8::display::Color BLACK_COLOR(0, 0, 0);
    const chip8::display::Color WHITE_COLOR(255, 255, 255);
    
    DisplayCliController::DisplayCliController(
            int window_width, int window_height, display_descriptor::IDisplayCliDescriptor& display_d
        ): 
        window_width(window_width),
        window_height(window_height),
        display_d(display_d),
        display_matrix(window_width, std::vector<chip8::display::Color>(window_height, WHITE_COLOR))
    {} 
        

    DisplayCliController::~DisplayCliController() {}
    
    void DisplayCliController::clear_screen() {

        for (int x = 0; x < this->window_width; x ++) {
            for (int y = 0; y < this->window_height; y ++) {
                if (!this->display_matrix[x][y].is_white()) {
                    this->display_matrix[x][y] = WHITE_COLOR;
                }
            }
        }

        this->display_d.clear();
        this->display_d.refresh();

    }

    void DisplayCliController::refresh_display() {

        this->display_d.clear();
        chip8::display::Color* current_color;

        for (int x = 0; x < this->window_width; x ++) {
            for(int y = 0; y < this->window_height  ; y ++) {
                current_color = &(this->display_matrix[x][y]);

                if (current_color->is_white()){
                    this->display_d.print_string(WHITE_CHAR.c_str());
                }
                else {
                    this->display_d.print_string(BLACK_CHAR.c_str());
                }
            }
            this->display_d.print_string("\n");
        }
        this->display_d.refresh();
    }

    void DisplayCliController::draw_rectangle(int  x, int y, int w, int h, Color color) {
        assert(x >= 0 && x < this->window_width);
        assert(x + w <= this->window_width);

        assert(y >= 0 && y < this->window_height);
        assert(y + h <= this->window_height);

        int max_x = x + w;
        int max_y = y + h;

        for (; x < max_x; x ++) {
            for (int temp_y = y; temp_y < max_y; temp_y ++) {
                this->display_matrix[x][temp_y] = color;
            }
        }

        this->refresh_display();
    }


}