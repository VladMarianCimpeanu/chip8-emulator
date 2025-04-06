#include <vector>

class Display {
    private:
        int height;
        int width;
        int pixel_size;
        std::vector<std::vector<int>> matrix;
        

    public:
        Display(int width, int height, int pixel_size) {
            this->height = height;
            this->width = width;
            this->pixel_size = pixel_size;
        }

        void reset() {
            
        }

};