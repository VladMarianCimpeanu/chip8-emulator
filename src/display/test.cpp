#include <thread>
#include <chrono>
#include <iostream>
#include "display.h"
#include "DisplayCliController.h"
#include "display_descriptor/DebugDisplayDescriptor.h"
#include "display_descriptor/NCursesDisplayDescriptor.h"


int main() {

    //display_descriptor::DebugDisplayDescriptor display_d = display_descriptor::DebugDisplayDescriptor();

    display_descriptor::NCursesDisplayDescriptor display_d = display_descriptor::NCursesDisplayDescriptor();
    chip8::display::DisplayCliController display = chip8::display::DisplayCliController(10, 10, display_d);
    chip8::display::Color black_color (0, 0, 0);

    display.clear_screen();

    for (int i = 0; i < 5; i ++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        display.draw_rectangle(i, i, 1, 1, black_color);
    }
    
}