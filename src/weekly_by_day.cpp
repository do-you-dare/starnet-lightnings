#include <cstdlib>
#include <iostream>
#include <zlib.h>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include "lightning.h"
#include "image.h"

#define NCITIES 4

bool allowed_city(int x, int (&a)[NCITIES]) {
  return std::find(std::begin(a), std::end(a), x) != std::end(a);
}

int main(int argc, char *argv[]){

  if (argc != 2) {
    std::cout << "ERROR: Wrong number of arguments" << std::endl;
    return -1;
  }

  // FIXME: don't hardcode file names. Specially like this.
  char imagename[] = "../raios/data/brasil_1km.png";

  gzFile file = gzopen(argv[1], "rb");
  if (file == NULL) {
    std::cout << "ERROR: Couldn't open " << argv[1] << std::endl;
    return -1;
  }

  CityData city(imagename);

  // Code of studied cities
  int cities[] = {3550308, 3304557, 4106902, 1302603};

  // Number of lightnings per city
  std::map<int, int> n_lightnings;

  char line[150];
  std::string st;
  Lightning lightning;
  int code;

  while (!gzeof(file)) {
    gzgets(file, line, 150);
    if (line == NULL) { break; }

    st = line;
    lightning.load(st);
    lightning.set_code(city);
    code = lightning.get_code();
    if (code == 16777215 || !allowed_city(code, cities)) { continue; }
    if (lightning.get_x() < 0 || lightning.get_y() < 0) { continue; }
    if (lightning.get_x() > 4101 || lightning.get_y() > 4201) { continue; }
    if (lightning.get_quality() == 2) { continue; }
    ++n_lightnings[lightning.get_code()];
  }

  std::cout << lightning.get_date();
  for (int c : cities) { std::cout << ", "<< n_lightnings[c]; }
  std::cout << std::endl;

  return 0;
}


