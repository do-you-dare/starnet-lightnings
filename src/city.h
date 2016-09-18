#ifndef CITY_H
#define CITY_H

#include "CImg.h"
using namespace cimg_library;

class Cities {
    public:
        Cities(char * fname);

        int get_code(int x, int y);

    private:
        CImg<int> img;
};

#endif
