#include <cstdlib>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
#include <zlib.h>
#include <vector>

using namespace std;

class ThreadUtils {
    public:
        static void save_to_file(string, std::ofstream);
        static void process_file(char*, string (*f)(string), std::ofstream);

    private:
        static std::mutex mtx;
};

