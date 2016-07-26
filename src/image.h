#ifndef CITY_DATA_H
#define CITY_DATA_H
#include <Magick++.h>

using namespace Magick;

class CityData {
  public:
  CityData(char[]);
  int get_code(int, int);

  private:
  Image image;
  ColorRGB tmp_color;
  int r, g, b;
};

#endif
