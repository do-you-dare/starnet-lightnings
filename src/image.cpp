#include "image.h"

CityData::CityData(char name[]) {
  image.read(name);
  r = g = b = 0;
}

int CityData::get_code(int x, int y) {
  tmp_color = image.pixelColor(x,4201 - y);
  r = tmp_color.red() * 255;
  g = tmp_color.green() * 255;
  b = tmp_color.blue() * 255;
  return b*(256*256) + g*256 + r;
}

