#ifndef VECT_H
#define VECT_H  

#include <cmath>
#include <iostream>

using std::sqrt;

class vec{
    private:
        double x;
        double y;
        double z;

    public:
    vec():x(0),y(0),z(0){}
    vec(double a, double b, double c):x(a), y(b), z(c){}
    
    double getx() const {return x;};
    double gety() const {return y;};
    double getz() const {return z;};

    // show the vector :
    void show(){
        std::cerr<<"("<<x<<", "<<y<<", "<<z<<")"<<std::endl;
    }


    // operators

    vec operator-() const{
      return vec(-x, -y, -z);  
    };

    double operator[](int i){
        double val;
        if(i%3==0){
            val = x;
        }
        else if (i%1)
        {
            val = y;
        }
        else{
            val = z;
        }
        return val;
        
    }


    vec& operator+=(vec &v){
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec& operator*=(const double t){
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    vec& operator/=(const double t){
        return *this *= 1/t;
    }

    double lenght_squared() const{
        return x*x+y*y+z*z;
    }

    double lenght() const{
        return sqrt(lenght_squared());
    }


};

inline vec operator*(double t, vec v){
    return vec(t*v.getx(), t*v.gety(), t*v.getz());
}

inline vec operator*( vec v, double t){
    return vec(t*v.getx(), t*v.gety(), t*v.getz());
}

inline std::ostream& operator<<(std::ostream &out, vec &v){
    return out<<"("<<v.getx()<<' '<<v.gety()<<' '<<v.getz()<<")";
}



//----------------------------------------------------------

inline vec operator+(const vec &u, const vec &v){
    return vec(u.getx()+v.getx(), u.gety()+v.gety(),u.getz()+v.getz());
}


inline vec operator-(const vec &u, const vec &v){
    return vec(u.getx()-v.getx(), u.gety()-v.gety(),u.getz()-v.getz());
}



inline vec operator/(const vec &u, double t){
    return (1/t)*u;
}


inline double dot(const vec &u, const vec &v){
    return u.getx()*v.getx()+u.gety()*v.gety()+u.getz()*v.getz();
}

inline vec cross(vec &u, vec &v){
    return vec(u.gety()*v.getz()-u.getz()*v.gety(),
                u.getz()*v.getx()-u.getx()*v.getz(),
                u.getx()*v.gety()-u.gety()*v.getx());
}

inline vec unit_vect(vec v){
    return v/v.lenght();
}


typedef vec Point;
using color = vec; //RGB color


#endif 