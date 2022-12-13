#include "vector.hpp"

void write_color(std::ostream &out, color the_color){
    out<<static_cast<int>(255.999 * the_color.getx())<<' '
    << static_cast<int>(255.999 * the_color.gety())<<' '
    << static_cast<int>(255.999 * the_color.getz())<<"\n";

};