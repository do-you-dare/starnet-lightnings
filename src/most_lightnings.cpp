#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "csv.h"

using std::string;
using std::map;
using std::pair;

bool element_compare(const pair<int,int>& p1, const pair<int,int>&p2) {
  return p1.second < p2.second;
}

int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cout << "Wrong number of parameters." << std::endl;
    return -1;
  }

  string data_file (argv[1]);
  string city_data (argv[2]);

  map<int, int> total_lightnings;
  map<int, int> days_of_lightning;
  map<int, float> lightning_density;
  map<int, string> city_names;
  map<int, std::vector<float>> city_coordinates;
  map<int, float> city_area;

  io::CSVReader<5> city_csv (city_data);
  city_csv.read_header(io::ignore_extra_column, "Nome do município", "IBGE",
                       "LATITUDE", "LONGITUDE", "AREA");

  string name;
  int code;
  float lat, lon, area;
  while (city_csv.read_row(name, code, lat, lon, area)) {
    city_names[code] = name;
    city_coordinates[code] = {lat, lon};
    city_area[code] = area;
  }

  io::CSVReader<4> in (data_file);
  in.read_header(io::ignore_extra_column, "dia", "cidade", "raios", "dist");

  int city, lightnings;
  string day, dist;
  while (in.read_row(day, city, lightnings, dist))
  {
    total_lightnings[city] += lightnings;
    days_of_lightning[city] += 1;
  }

  for (auto it = total_lightnings.begin();it != total_lightnings.end();++it) {
    lightning_density[it->first] = it->second / city_area[it->first];
  }

  std::ofstream day_data ("most_days.csv");
  std::ofstream total_data ("most_lightnings.csv");
  std::ofstream density_data ("most_density.csv");

  day_data << "Cidade, Dias, Latitude, Longitude" << std::endl;
  total_data << "Cidade, Raios, Latitude, Longitude" << std::endl;
  density_data << "Cidade, Densidade, Latitude, Longitude" << std::endl;

  for (int i = 0; i < 10; ++i) {
    map<int,int>::iterator max_d, max_t;
    map<int,float>::iterator max_de;
    max_d = std::max_element(days_of_lightning.begin(),
            days_of_lightning.end(), element_compare);
    max_t = std::max_element(total_lightnings.begin(),
            total_lightnings.end(), element_compare);
    max_de = std::max_element(lightning_density.begin(),
            lightning_density.end(), element_compare);

    day_data << city_names[max_d->first] << ", " << max_d->second <<
                ", " << city_coordinates[max_d->first][0] << ", " <<
                city_coordinates[max_d->first][1]<< std::endl;
    total_data << city_names[max_t->first] << ", " << max_t->second <<
                ", " << city_coordinates[max_t->first][0] << ", " <<
                city_coordinates[max_t->first][1]<< std::endl;
    density_data << city_names[max_de->first] << ", " << max_de->second <<
                ", " << city_coordinates[max_de->first][0] << ", " <<
                city_coordinates[max_de->first][1]<< std::endl;

    days_of_lightning.erase(max_d);
    total_lightnings.erase(max_t);
    lightning_density.erase(max_de);
  }

  return 0;
}
