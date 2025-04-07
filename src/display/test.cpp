#include "display.h"
#include <ncurses.h>
#include <thread>
#include <chrono>


int main() {

    chip8::display::LinuxCliDisplay display = chip8::display::LinuxCliDisplay();

    display.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    printw("print after 1 reset");
    display.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    printw("print after 2 reset");
    display.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

}