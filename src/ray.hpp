#ifndef RAY_HPP
#define RAY_HPP

#include "vector.hpp"

class ray{
    private:
    Point origine; // Point de depart 
    vec dir; // vecteur directeur

    public:
    ray(){}
    //explicit ray(Point a, vec b):origine(a), dir(b) {}     //?? Ambigue, Point et Dir sont tout deux des vec
    ray(const Point& a, const vec& b): origine(a), dir(b){}


    Point get_origine() const { return origine;}
    vec get_dir() const {return dir;}

    Point at(double t){
        return origine + t*dir;
    }
    

};

#endif