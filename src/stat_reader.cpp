/*
 * Usage:
 * Call with the paths of the files to be processed. The output is hardcoded to out.txt.
 *
 * */

#include <cstdlib>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <vector>
#include <exception>

#include "city.h"

#define NTHREADS 4

using namespace std;

std::mutex mtx;

//char image_file[] = "data/brasil_1km.png";
//Cities cities(image_file);

// Thread-safe writing to an open file
void save_to_file(string s, string fn, std::ofstream* f) {
    std::lock_guard<std::mutex> file_lock (mtx);
    std::cout << "Saving " << fn << " data" << std::endl;
    *f << s;
}


void process_files( char* input[], int start, int end,
                    string (*process)(gzFile), std::ofstream* f ) {
    for (int i = start; i < end; ++i) {
        gzFile file = gzopen(input[i], "rb");

        if (file == NULL) {
            std::cout << "Error opening " << input[i] << std::endl;
            return;
        }

        string s = (*process)(file);

        save_to_file(s, input[i], f);
    }
}


// Dummy test function
std::string test ( gzFile file ) {
    char ln[150];
    string line;
    string s = "";

    while (!gzeof( file )) {
        gzgets(file, ln, 150);
        if ( ln == NULL ) { std::cout << "here"; break; }
        line = ln;
    }
    return "HA";
}


int main(int argc, char* argv[]) {
    std::ofstream out_file;
    out_file.open("out.txt");

    if (argc == 1) {
        std::cout << "No files specified." << std::endl;
        return -1;
    }

    std::thread threads[NTHREADS];

    int size = argc - 1;
    int step = size / NTHREADS;

    for (int i = 0; i < NTHREADS; ++i) {
        int end = (i != NTHREADS - 1)? (i + 1) * step + 1 : argc;
        //cout << i << " : " << i * step + 1 << "-" << end - 1<< endl;
        threads[i] = std::thread(process_files, argv, i * step + 1,
                                 end, &test, &out_file);
    }

    for (auto& th : threads) th.join();

    out_file.close();

    return 0;
}

