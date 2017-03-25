#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <cstdlib>
#include <string>

#include "city.h"

#define LAT0 -35
#define LON0 -75
#define RES 0.01

class Lightning {
    public:
        Lightning(Cities * cities, std::string st) {
            format_date(st.substr(0, 11));
            lat = std::stof(st.substr(29, 9));
            lon = std::stof(st.substr(40, 9));
            x = (lat - LAT0) / RES;
            y = (lon - LON0) / RES;
            quality = std::stoi(st.substr(69, 1));
            used = x > 0 && x < cities->width &&
                   y > 0 && y < cities->height;
            if (used) {
                code = (*cities).get_code(x, y);
            } else {
                code = -1;
            }
        }

        std::string date;
        int x, y;
        int quality;
        int code;
        bool used;
        float lat, lon;

    private:
        void format_date(std::string s) {
            date = s.replace(4,2,"/").replace(7,1,"/");
        }
};

#endif

