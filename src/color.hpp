#ifndef COLOR_HPP
#define COLOR_HPP

#include "vector.hpp"
#include <algorithm>

void write_color(std::ostream &out, color the_color){

    auto r = the_color.getx();
    auto g = the_color.gety();
    auto b = the_color.getz();
    
    out<<static_cast<int>(255.999 * std::clamp(r, 0.0, 0.999))<<' '
    << static_cast<int>(255.999 * std::clamp(g, 0.0, 0.999))<<' '
    << static_cast<int>(255.999 * std::clamp(b, 0.0, 0.999))<<"\n";

};

#endif