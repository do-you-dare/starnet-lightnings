#include <cstdlib>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <vector>

using namespace std;

std::mutex mtx;

// Thread-safe writing to an open file
void save_to_file(std::string s, std::ofstream* f) {
    std::lock_guard<std::mutex> file_lock (mtx);
    *f << s;
}

// Threadable function
void process_file( char* in, string (*process)(string), std::ofstream* f ) {
  gzFile file = gzopen(in, "rb");
  char ln[50];
  string line;
  string s = "";

  while (!gzeof( file )) {
    gzgets(file, ln, 150);
    if ( ln == NULL ) { break; }
    line = ln;
    s += (*process)(line) + "\n";
  }
    save_to_file(s, f);
}

