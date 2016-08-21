#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <cstdlib>
#include <string>
#include "image.h"

class Lightning {
  public:
  Lightning();
  void load(std::string&);
  std::string print();
  void set_code(CityData&);
  int get_code();
  int get_x();
  int get_y();
  int get_quality();
  std::string get_date();

  private:
  std::string date;
  float lat, lon;
  int x, y;
  int quality;
  int city_code;
  void format_date(std::string);
};

#endif
