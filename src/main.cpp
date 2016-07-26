#include <cstdlib>
#include <iostream>
#include <zlib.h>
#include <string>
#include "lightning.h"
#include "image.h"

int main(int argc, char *argv[]){

  if (argc != 2) {
    std::cout << "ERROR: Wrong number of arguments" << std::endl;
    return -1;
  }
  char imagename[] = "../raios/data/brasil_1km.png";

  gzFile file = gzopen(argv[1], "rb");
  if (file == NULL) {
    std::cout << "ERROR: Couldn't open " << argv[1] << std::endl;
    return -1;
  }

  CityData city(imagename);

  char line[150];
  std::string st;
  Lightning lightning;

  while (!gzeof(file)) {
    gzgets(file, line, 150);
    if (line == NULL) { break; }

    st = line;
    lightning.load(st);
    lightning.set_code(city);
    if (lightning.get_code() == 16777215) { continue; }
    if (lightning.get_x() < 0 || lightning.get_y() < 0) { continue; }
    if (lightning.get_x() > 4101 || lightning.get_y() > 4201) { continue; }
    if (lightning.get_quality() == 2) { continue; }
    std::cout << lightning.print() << std::endl;
  }

  return 0;
}

