#ifndef VECT_H
#define VECT_H  

#include <cmath>
#include <iostream>
#include <random>
#include <cstdlib>
#include <ctime>  

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

    bool near_null() const{
        auto epsilon = 1e-5;
        return (fabs(x)<epsilon) && (fabs(y)<epsilon) && (fabs(z)<epsilon);
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

inline vec operator*(const vec &u, const vec &v){
    return vec(u.getx()*v.getx(), u.gety()*v.gety(), u.getz()*v.getz());
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

vec reflection(vec v, vec n){
    return v - 2*dot(v,n)*n;
}


typedef vec Point;
using color = vec; //RGB color



inline double random_double(){
    return rand()/(RAND_MAX+1.0);
}

inline double random_double(double min_,double max_){
    return min_+(max_-min_)*random_double();
}


//return a vec having its directions randomly chosen between min_ and max_
inline static vec random(double min_, double max_){
    return vec(random_double(min_, max_),random_double(min_, max_),random_double(min_, max_));
};

//return a random unit vec (random between min_ and max_), having scalar product positive with normal_
vec random_vec_unit(){   
    while(true){
        auto vect_ = random(-1, 1); 
        if(vect_.lenght_squared()>=1) continue;
        return vect_;
    }
};



#endif 