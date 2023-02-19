#include "vector.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "hittalbe.hpp"
#include "list_hittable.hpp"
#include <iostream>

#include "utils.hpp"

color BG_color(1.0,1.0,1.0);  // Color of the background


double is_hit(ray the_ray, sphere the_sphere){     // the_ray = a+t*b
    vec b = the_ray.get_dir();                  
    Point a = the_ray.get_origine();
    Point c = the_sphere.get_center();
    double a_1 = dot(b,b);
    double b_1 = 2*dot(b, a - c);
    double c_1 = dot(a-c, a-c) - the_sphere.get_rayon()*the_sphere.get_rayon();
    double delta = b_1*b_1 - 4*a_1*c_1;

    //return(delta >=0);
    if(delta>=0){
        return((-b_1-sqrt(delta))/2*a_1); 
    }
    else{
        return(-1);
    }
}

 color ray_color_the_sphere(ray the_ray, sphere the_sphere){
    auto t_hit = is_hit(the_ray, the_sphere);

    //vec dir1 = the_ray.get_dir();
    //vec direction = unit_vect(dir1);
    //double t = 0.5*(direction.gety()+ 1.0);
    color the_color;// = (1.0-t)*color(1.0,1.0,1.0)+t*color(0.5,0.0,0.0);

    if(t_hit >=0.0){
        vec N = unit_vect(the_ray.at(t_hit) - vec(0.0,0.0,-1.0));
        the_color = 0.6*color(N.getx()+1, N.gety()+1, N.getz()+1);
        }
        else{
            the_color = BG_color;
        }

    return the_color;
} 

 color ray_color(ray& the_ray, const list_hittable& Shapes){
    data_hit data_buffer;
    //auto hitt = Shapes.hit(the_ray, 0, infinity, data_buffer);
    if(Shapes.hit(the_ray, 0, infinity, data_buffer)){
        //double t_hit = data_buffer.t;
        //vec N = data_buffer.normal_hit;
        //the_color =  0.6*color(N+color(1,1,1));
        return 0.5*(data_buffer.normal_hit+color(1,1,1));

    }
    std::cerr<<"Not hit\n";
    vec unit_direction = unit_vect(the_ray.get_dir());
    auto t = 0.5*(unit_direction.gety()+1.0);
    return (1.0-t)*color(1.0,1.0,1.0)+t*color(0.5,0.7,1.0);

} 

// Function to draw the picture, change "the_sphere" to a list of hittable
void draw_picture(int i,int j,const int width,const int height, auto hor, auto vert, vec lower_left_corner,vec origin,list_hittable Shapes){
    auto u = double(i) / (width-1);
    auto v = double(j) / (height-1);
    vec direction = lower_left_corner + u*hor+v*vert;//- origin;
    ray r(origin, direction);
    color the_color;// = ray_color(r);
    the_color = ray_color(r, Shapes);
    write_color(std::cout,the_color);
}

void draw_picture_the_sphere(int i,int j,const int width,const int height, auto hor, auto vert, vec lower_left_corner,vec origin,sphere the_sphere){
    auto u = double(i) / (width-1);
    auto v = double(j) / (height-1);
    vec direction = lower_left_corner + u*hor+v*vert - origin;
    ray r(origin, direction);
    color the_color;// = ray_color(r);
    the_color = ray_color_the_sphere(r, the_sphere);
    write_color(std::cout,the_color);
}

// Creation d'une image au format PPM, exemple
int main(){
    const auto ratio = 16.0/9.0;
    const int width = 1920;
    const int height = static_cast<int>(width/ratio);
    std::cerr<<"\n Height: "<<height<<"\n"<<std::flush;


    //camera
    auto view_height = 2.0;
    auto view_width = ratio*view_height;
    std::cerr<<"\n View_width : "<<view_width<<"\n"<<std::flush;
    auto focal = 1.0;

    Point origin = Point(0.0,0.0,0.0);
    auto hor = vec(view_width,0,0);
    auto vert = vec(0,view_height,0);
    vec lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);


    std::cout<<"P3\n"<<width<<" "<<height<<"\n255\n";
    // Ajout d'une sphère
    double rayon = 4;
    Point center_sphere(0.0,0.0,-10.0);
    color the_sphere_color(8.0,0.0,0.0);
    sphere the_sphere(Point(0,0,-1), 0.5,the_sphere_color);

    //List of hittable
    list_hittable Shapes;
    Shapes.add(make_shared<sphere> (the_sphere));
    
    for(int j = height-1; j>=0; j--){

        

        // Progressindicatorto the error output
        //std::cerr<<"\n Lines remaining: "<<j<<' '<<std::flush;


        // Parralelisons le parcours en longueur : (a faire plus tard)
        for(int i= 0; i<width; i++){
            draw_picture(i,j,width, height,hor,vert, lower_left_corner,origin,Shapes);
            //draw_picture_the_sphere(i,j,width, height,hor,vert, lower_left_corner,origin,the_sphere);
        }
    }
    std::cerr<<"\nDone.\n";
}