#ifndef RAY_HPP
#define RY_HPP

#include "vector.hpp"

class ray{
    private:
    Point origine; // Point de depart 
    vec dir; // vecteur directeur

    public:
    ray(){}
    explicit ray(Point a, vec b):origine(a) {}     //?? Ambigue, Point et Dir sont tout deux des vec
    ray(Point& a, vec& b): origine(a), dir(b){}

    Point get_origine(){ return origine;}
    vec get_dir(){return dir;}

    Point at(double t){
        return origine + t*dir;
    }
    

};

#endif