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
#include <map>
#include <exception>

#include "lightning.h"

#define NTHREADS 4

using namespace std;

std::mutex mtx;

// Thread-safe writing to an open file
void save_to_file(string s, string fn, std::ofstream* f) {
    std::lock_guard<std::mutex> file_lock (mtx);
    std::cout << "Saving " << fn << " data" << std::endl;
    *f << s;
}


void process_files( char* input[], int start, int end,
        string (*process)(gzFile,Cities*), std::ofstream* f,
        Cities * img) {
    for (int i = start; i < end; ++i) {
        //cout << "File " << input[i] << endl;
        gzFile file = gzopen(input[i], "rb");

        if (file == NULL) {
            std::cout << "Error opening " << input[i] << std::endl;
            return;
        }

        string s = (*process)(file, img);

        save_to_file(s, input[i], f);
    }
}


std::string get_dayly_data ( gzFile file, Cities* img ) {
    char line[150];
    std::string tmp, result;
    std::map<int, int> cities;
    string date;

    while ( !gzeof(file) ) {
        gzgets(file, line, 150);
        if (line == NULL) { break; }

        tmp = line;
        Lightning lightning(img, tmp);
        if (lightning.code == 16777215 || lightning.code == -1) { continue; }
        if (lightning.quality == 2) { continue; }
        date = lightning.date;

        ++cities[lightning.code];
    }

    for (auto it = cities.begin(); it != cities.end(); ++it) {
        if (it->second > 10) {
            result += date + ", " + std::to_string(it->first) +
                ", " + std::to_string(it->second) + "\n";
        }
    }

    return result;
}


int main(int argc, char* argv[]) {
    std::ofstream out_file;
    out_file.open("out.txt");
    Cities img("data/brasil_1km.png");

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
                end, &get_dayly_data, &out_file, &img);
    }

    for (auto& th : threads) th.join();

    out_file.close();

    return 0;
}

