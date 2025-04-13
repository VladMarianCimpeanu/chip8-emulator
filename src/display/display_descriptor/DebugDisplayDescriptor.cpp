#include <iostream>

#include "DebugDisplayDescriptor.h"


namespace display_descriptor
{

    DebugDisplayDescriptor::DebugDisplayDescriptor() {};
    DebugDisplayDescriptor::~DebugDisplayDescriptor() {};
    
    void DebugDisplayDescriptor::print_string(const char* string_to_print) {
        std::cout << string_to_print;
    }
    
    void DebugDisplayDescriptor::refresh() {
        std::cout << "\n" << "REFRESH" <<"\n";
    }
    
    void DebugDisplayDescriptor::clear() {
        std::cout << "\n" << "CLEAR SCREEN" << "\n";
    };

}
