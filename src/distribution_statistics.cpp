/*
 * Usage: 
 * $ ./dist_statistics datafile output_folder
 * The output folder needs to be created first
 * */
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>
#include "csv.h"

#define NCITIES 4

int main(int argc, char *argv[]){

  if (argc != 3) {
    std::cout << "ERROR: Wrong number of arguments. Usage:" << std::endl;
    std::cout << "$ ./dist_statistics datafile output_folder" << std::endl;
    return -1;
  }

  char* datafile = argv[1];
  char* out_folder = argv[2];

  std::vector<std::map<int, std::vector<double>>> lightnings;
  std::vector<std::map<int, int>> days;
  std::vector<std::map<int, double>> mean;
  std::vector<std::map<int, double>> sd;

  lightnings.resize(7);
  days.resize(7);
  mean.resize(7);
  sd.resize(7);

  // The initial value reflect the first weekday from data.
  // '0' means sunday, '1' monday, and so on.
  int day = 0;

  io::CSVReader<NCITIES> data (datafile);
  data.read_header(io::ignore_extra_column, "c1", "c2", "c3", "c4");

  // Read the data
  int c1, c2, c3, c4;
  while (data.read_row(c1, c2, c3, c4)) {
    int c[] = {c1, c2, c3, c4};
    for (int i = 0; i < NCITIES; ++i) {
      lightnings[day][i].push_back(c[i]);
      if (c[i] != 0) { ++days[day][i]; }
    }
    day = ++day % 7;
  }

  // Calculate mean and standard deviation
  for (int day=0; day < 7; ++day) {
    for (int i=0; i < NCITIES; ++i) {
      int sum = 0;
      for (int n : lightnings[day][i]) { sum += n; }
      double m = double(sum) / double(days[day][i]);
      sum = 0;
      for (int n : lightnings[day][i]) {
        if (n != 0) { sum += pow(n - m, 2); }
      }
      mean[day][i] = m;
      sd[day][i] = sqrt(double(sum) / double(days[day][i]));
    }
  }

  // FIXME: this should output to files, not std::cout
  for (int i=0; i < NCITIES; ++i) {
    std::cout << std::endl;
    for (int d=0; d < 7; ++d) {
      std::cout << d << ", " << mean[d][i] << ", " << sd[d][i] << std::endl;
    }
  }

  return 0;
}

