#ifndef IDISPLAY_DESCRIPTOR
#define IDISPLAY_DESCRIPTOR

namespace display_descriptor
{

    class IDisplayCliDescriptor {
        public:
            virtual ~IDisplayCliDescriptor() = default;
            virtual void print_string(const char* string_to_print) = 0;
            virtual void refresh() = 0;
            virtual void clear() = 0;

    };
} // namespace display_descriptor



#endif