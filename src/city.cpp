#define _USE_MATH_DEFINES
#define LAT0 -35

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include "csv.h"

using std::map;
using std::vector;
using std::string;

int distance(int, float, float, map<int, vector<float>>&);

int main(int argc, char* argv[]) {
  if (argc != 4) { return -1; }
  string city_file(argv[1]);
  string light_file(argv[2]);
  string out_file(argv[3]);
  map<int, std::vector<float>> city;
  map <int, map<string, string>> data;
  io::CSVReader<3> city_data (city_file);
  city_data.read_header(io::ignore_extra_column, "LATITUDE", "LONGITUDE","IBGE");

  int code;
  float lat, lon;
  while (city_data.read_row(lat, lon, code)) {
    city[code] = {lat, lon};
  }

  io::CSVReader<4> in(light_file);
  in.read_header(io::ignore_extra_column, "dia", "cidade", "lat", "lon");

  string day;
  int dist;
  // Change to first load the data, process, and then print
  while (in.read_row(day, code, lat, lon)) {
    dist = distance(code, lat, lon, city);
    if (dist != -1) {
      data[code][day] += std::to_string(dist) + " ";
    }
  }

  // Check number of lightnings and print data
  std::ofstream file (out_file);
  char running[] = "|\\-|-/";
  int count = 0;
  for (auto it = data.begin(); it != data.end(); ++it) {
    std::cout << "\b" << running[count] << std::flush;
    count = ++count % 7;
    for (auto i = it->second.begin(); i != it->second.end(); ++i) {
      size_t n_lightnings = std::count(i->second.begin(), i->second.end(), ' ');
      if (n_lightnings > 10) {
        // Save data for the day
        file << i->first << ", " << it->first << ", "  \
          << n_lightnings << ", " << i->second << std::endl;
      }
    }
  }

  std::cout << "\b " << std::endl;

  return 0;
}

int distance(int key, float lat, float lon, std::map<int,std::vector<float>>& city) {
  std::vector<float> c_coord = city[key];
  if (c_coord.empty()) {return -1;}
  float lat_c = c_coord[0];
  float lon_c = c_coord[1];
  float gama, siglat, siglon, lon1, lon2, lat1, lat2, c1, lat_plus, lat_minus;
  float yxa1, yxb1, b1, r1, a1, s, r, zeta, lla1;
  float to_rad = M_PI / 180;

  gama = 0;
  siglat = lat - lat_c;
  siglon = lon - lon_c;
  int quad = 0;
  if (siglon < 0) { quad = -1; }

  lon1 = lon_c * to_rad; // All in radians
  lon2 = lon * to_rad;
  lat1 = lat_c * to_rad;
  lat2 = lat * to_rad;

  c1 = lon2 - lon1;
  if (c1 != 0) {
    c1 = 1 / tan(c1 / 2.0);
    lat_plus  = lat2 + lat1;
    lat_minus = lat2 - lat1;

    if (lat_minus != 0) {
      yxa1 = c1 * sin(lat_minus) / cos(lat_plus);
      yxb1 = c1 * cos(lat_minus) / sin(lat_plus);
      yxa1 = atan(yxa1) * to_rad;
      yxb1 = atan(yxb1) * to_rad;

      r1 = atan(tan(lat_minus / 2) * sin(yxb1) / sin(yxa1)) * 2 * to_rad;

      b1 = r1;
      a1 = M_PI_2 - lat1;
      c1 = M_PI_2 - lat2;
      s = (a1 + b1 + c1) / 2;
      r = sqrt(abs(abs(sin(s - a1) * sin(s - b1) * sin(s - c1) / sin(s))));

      if ((s - c1) != 0) { gama = 2 * atan(r / sin(s - c1) * to_rad); }
      gama <- gama * 180/M_PI;
      zeta <- gama;
      if (quad == -1) { gama = 360 - gama; }

    } else {
      // Right Spherical Triangle Rule
      lla1 = M_PI_2 - lat2;
      c1 = lon2 - lon1;
      r1 = sin(c1) * sin(lla1);
      r1 = asin(r1) * to_rad;
      if (siglon > 0) { gama = 90; }
      if (siglon < 0) { gama = 270; }
      zeta = gama;
    }
  } else {
    r1 = abs(lat1 - LAT0);
  }
  r1 = r1 * 180/M_PI;
  r1 = abs(r1 * 111.195); // Conversion from degrees to km
  return (int) r1;
}
