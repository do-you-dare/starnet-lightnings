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

#define NTHREADS 4

using namespace std;

std::mutex mtx;

// Thread-safe writing to an open file
void save_to_file(string s, string fn, std::ofstream* f) {
    std::lock_guard<std::mutex> file_lock (mtx);
    std::cout << "Saving " << fn << " data" << std::endl;
    *f << s;
}

// TODO: use an array of filenames instead of a single file
void process_files( char* input, string (*process)(gzFile), std::ofstream* f ) {
    gzFile file = gzopen(input, "rb");

    if (file == NULL) {
        std::cout << "Error opening " << input << std::endl;
        return;
    }

    // Calling 
    string s = (*process)(file);

    save_to_file(s, input, f);
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


    // This is for easy testing only; read filenames from argv.
    char* data[] = {
        "data/2014-01-01.dat.gz",
        "data/2014-01-02.dat.gz",
        "data/2014-01-03.dat.gz",
        "data/2014-01-04.dat.gz",
        "data/2014-01-05.dat.gz"
    };

    std::thread threads[NTHREADS];

    // TODO: make subpartitions of arrays with filenames
    for (int i = 0; i < NTHREADS; ++i) {
        threads[i] = std::thread(process_files, data[i + 1], &test, &out_file);
    }

    for (auto& th : threads) th.join();

    out_file.close();

    return 0;
}
