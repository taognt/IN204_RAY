#ifndef COLOR_HPP
#define COLOR_HPP

#include "vector.hpp"
#include "utils.hpp"

double clamp(double x, double min_, double max_ ){
    if(x<min_) return min_;
    if(x>max_) return max_;
    return x;
}

void write_color(std::ostream &out, color the_color, int number_of_sample){

    auto r = the_color.getx();
    auto g = the_color.gety();
    auto b = the_color.getz();

    auto normalis = 1.0/number_of_sample;
    r *= normalis;
    g *= normalis;
    b *= normalis;
    
    out<<static_cast<int>(255.999 * clamp(r, 0.0, 0.999))<<' '
    << static_cast<int>(255.999 * clamp(g, 0.0, 0.999))<<' '
    << static_cast<int>(255.999 * clamp(b, 0.0, 0.999))<<"\n";

};

#endif