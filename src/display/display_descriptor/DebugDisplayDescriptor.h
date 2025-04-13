#ifndef DEBUG_DISPLAY_DESCRIPTOR
#define DEBUG_DISPLAY_DESCRIPTOR

#include "IDisplayCliDescriptor.h"

namespace display_descriptor
{

    class DebugDisplayDescriptor: public IDisplayCliDescriptor {
        public:
            DebugDisplayDescriptor();
            ~DebugDisplayDescriptor();
            void print_string(const char* string_to_print) override;
            void refresh() override;
            void clear() override;

    };
} // namespace display_descriptor



#endif