#include "city.h"

Cities::Cities(char* fname) {
    img = CImg<int>(fname);
    width = img.width();
    height = img.height();
}

int Cities::get_code(int x, int y) {
    int Y = img.height() - y;
    return 65536 * img(x, Y, 2) + 256 * img(x, Y, 1) + img(x, Y, 0);
};
