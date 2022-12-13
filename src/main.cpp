#include <iostream>
#include "vector.hpp"
#include "color.hpp"
#include "ray.hpp"

color ray_color(ray the_ray){
    vec direction = unit_vect(the_ray.get_dir());
    double t = 0.5*(direction.gety()+ 1);
    color the_color = (1.0-t)*color(1.0,1.0,1.0)+t*color(1.0,0.2,0.3);
}

// Creation d'une image au format PPM, exemple
int main(){
    const auto ratio = 16.0/9.0;
    const int width = 1920;
    const int height = static_cast<int>(width/ratio);

    //camera
    auto view_height = 2.0;
    auto view_width = ratio*view_height;
    auto focal = 1.0;

    auto origin = Point(0.0,0.0,0.0);
    auto hor = vec(view_width,0,0);
    auto vert = vec(0,view_height,0);
    auto lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);


    std::cout<<"P3\n"<<width<<" "<<height<<"\n255\n";
    for(int j = height-1; j>=0; j--){
        // Progressindicatorto the error output
        std::cerr<<"\n Lines remaining: "<<j<<' '<<std::flush;
        for(int i= 0; i<width; i++){
            auto u = double(i) / (width-1);
            auto v = double(j) / (height-1);
            ray r(origin, lower_left_corner + u*hor+v*vert - origin);
            color the_color = ray_color(r);
            write_color(std::cout,the_color);
        }
    }
    std::cerr<<"\nDone.\n";
}