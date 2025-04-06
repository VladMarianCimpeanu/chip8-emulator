#include "memory.h";
#include "display.h";


namespace chip8::cpu {
    class CPU {

        private:
            chip8::memory::Memory memory;
            chip8::display::Display display;

        public:
            CPU(chip8::memory::Memory memory, chip8::display::Display display){
                this->memory = memory;
                this->display = display;
            }

            void reset() {
                this->memory.reset();
                this->display.reset();
            }
    };
}
