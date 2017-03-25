/*
 * Usage:
 * Call with the paths of the files to be processed. The output is hardcoded to distance.txt.
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

#define NTHREADS 8

using namespace std;
// city_id -> week_day -> distances
using Data = std::map<int, std::map<int, std::map<int, int>>>;
using cData = std::map<int, std::pair<int, int>>;

std::mutex mtx;

Data distance_table;
cData city_data;
int cities[] = { 3550308, 3304557 };

// Thread-safe writing to a map
void save_partial_data(Data partial_data, Data& data) {
    std::lock_guard<std::mutex> variable_lock (mtx);
    std::cout << "Saved" << "\n";
    for (auto const &entry1 : partial_data) {
        for (auto const &entry2 : entry1.second) {
            for (auto const &entry3 : entry2.second) {
                data[entry1.first][entry2.first][entry3.first] += entry3.second;
            }
        }
    }
}

// TODO: actually load the data
void load_city_data() {
    city_data = new cData;
}

void process_files(char* input[], int start, int end,
        Data (*process)(gzFile,Cities*), std::ofstream* f,
        Cities * img) {
    for (int i = start; i < end; ++i) {
        //cout << "File " << input[i] << endl;
        gzFile file = gzopen(input[i], "rb");

        if (file == NULL) {
            std::cout << "Error opening " << input[i] << std::endl;
            return;
        }

        Data tmpData = (*process)(file, img);

        save_partial_data(tmpData, distance_table);
    }
}

// Sakamoto's algorithm
int day_of_the_week(int y, int m, int d) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    y -= m < 3;
    return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

// TODO: Implement distance routine
float polar_distance ( float x1, float y1, float x2, float y2 ) {
    return .2f;
}

// TODO: load city_data [or, why is it not here yet?]
Data get_distance_data ( gzFile file, Cities* img ) {
    char line[150];
    std::string tmp, result;
    std::map<int, int> cities;
    string date;
    Data tmp_distance_table;

    while ( !gzeof(file) ) {
        gzgets(file, line, 150);
        if (line == NULL) { break; }

        tmp = line;
        Lightning lightning(img, tmp);
        if (lightning.code == 16777215 || lightning.code == -1) { continue; }
        if (lightning.quality == 2) { continue; }
        date = lightning.date;
        int week_day = day_of_the_week(stoi(date.substr(0, 4)),  // Year
                                       stoi(date.substr(5, 2)),  // Month
                                       stoi(date.substr(8, 2))); // Day

        for (int i = 0; i < cities.size(); ++i) {
            float city_lat = city_data[i].first;
            float city_lon = city_data[i].second;
            float dist = polar_distance(city_lat, city_lon, lightning.lat,
                                        lightning.lon);

            tmp_distance_table[cities[i]][week_day][round(dist)]++;
        }
    }

    return tmp_distance_table;
}


int main(int argc, char* argv[]) {
    std::ofstream out_file;
    out_file.open("distance.txt");
    Cities img("data/brasil_1km.png");
    load_city_data();

    if (argc == 1) {
        std::cout << "No files specified." << std::endl;
        return -1;
    }

    std::thread threads[NTHREADS];

    int size = argc - 1;
    int step = size / NTHREADS;

    for (int i = 0; i < NTHREADS; ++i) {
        int end = (i != NTHREADS - 1)? (i + 1) * step + 1 : argc;
        threads[i] = std::thread(process_files, argv, i * step + 1,
                end, &get_distance_data, &out_file, &img);
    }

    for (auto& th : threads) th.join();

    // Write content to file
    for (auto const &c : distance_table) {
        for (auto const &d : c.second) {
            for (auto const &i : d.second) {
                out_file << c.first << ", " << d.first << ", " << i.first <<
                    ", " << i.second << endl;
            }
        }
    }

    out_file.close();

    return 0;
}
