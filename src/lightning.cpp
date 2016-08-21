#include "lightning.h"

#define LAT0 -35
#define LON0 -75
#define RES 0.01


Lightning::Lightning(){}

void Lightning::load(std::string& st){
  format_date(st.substr(0, 11)); // Improve format
  lat = std::stof(st.substr(29, 9));
  lon = std::stof(st.substr(40, 9));
  x = (lat - LAT0) / RES;
  y = (lon - LON0) / RES;
  quality = std::stoi(st.substr(69, 1));
  city_code = -1;
}

std::string Lightning::print(){
  return date + ", " + std::to_string(city_code) +
    ", " + std::to_string(lat) + ", " + std::to_string(lon);
}

int Lightning::get_code(){
  return city_code;
}

int Lightning::get_x(){
  return x;
}

int Lightning::get_y(){
  return y;
}

int Lightning::get_quality(){
  return quality;
}

std::string Lightning::get_date() {
  return date;
}

void Lightning::set_code(CityData &city) {
  city_code = city.get_code(x, y);
}

void Lightning::format_date(std::string s) {
  date = s.replace(4,2,"/").replace(7,1,"/");
}

