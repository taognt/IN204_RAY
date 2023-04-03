#include "../include/inc/vector.hpp"
#include "../include/inc/color.hpp"
#include "../include/inc/ray.hpp"
#include "../include/inc/sphere.hpp"
#include "../include/inc/hittalbe.hpp"
#include "../include/inc/list_hittable.hpp"
#include "../include/inc/Plan.hpp"
#include "../include/inc/camera.hpp"
#include "../include/inc/material.hpp"
#include <iostream>
#include <chrono>
#include <fstream>

#include<omp.h>

#include "../include/inc/utils.hpp"
#include "../include/extern/json.hpp"

using json = nlohmann::json;

color BG_color(1.0,1.0,1.0);  // Color of the background

color ray_color(ray& the_ray, const list_hittable& Shapes, int depth){

    if(depth<=0){
        return color(0.0,0.0,0.0);
    } 

    data_hit rec;
    if(Shapes.hit(the_ray, 0.001, infinity, rec)){
        ray scattered;
        color attenuation;
        //The ray is diffused or reflected depeding on the material
        if(rec.mat_ptr->scatter(the_ray, rec, attenuation, scattered)){
            return attenuation*ray_color(scattered, Shapes, depth-1);
        }
        else{
            return BG_color;
        }
    }
    return BG_color;
}

//Function that take the json file and create the scene
void create_scene(const json j, list_hittable& Shapes, camera &the_camera, float &ratio, int &width, int &height, int &max_depth, int &number_of_sample){
    // Parse image
    const auto& image = j["image"];
    width = image["width"].get<int>();
    max_depth = image["max_depth"].get<int>();
    number_of_sample = image["number_of_sample"].get<int>();
    float aspect_ratio_x = image["aspect_ratio"][0].get<float>();
    float aspect_ratio_y = image["aspect_ratio"][1].get<float>();

    //add image data to scene
    ratio = aspect_ratio_x/aspect_ratio_y; 
    height = static_cast<int>(width/ratio);
    std::cerr<<"\nWidth : "<<width<<"\nHeight: "<<height<<"\n";
    std::cerr << "max_depth: " << max_depth << std::endl;

    // Parse camera
    const auto& cam = j["camera"];
    float view_height = cam["height"].get<float>(); //used directely
    float cx = cam["origin"][0].get<float>();
    float cy = cam["origin"][1].get<float>();
    float cz = cam["origin"][2].get<float>();
    float lx = cam["look_at"][0].get<float>();
    float ly = cam["look_at"][1].get<float>();
    float lz = cam["look_at"][2].get<float>();
    float ux = cam["up_vector"][0].get<float>();
    float uy = cam["up_vector"][1].get<float>();
    float uz = cam["up_vector"][2].get<float>();
    float focal = cam["focal"].get<float>(); //used directely

    //adding to scene
    Point origin = Point(cx,cy,cz);
    auto view_width = ratio*view_height;
    std::cerr<<"\nView_width : "<<view_width<<"\n"<<std::flush;
    auto hor = vec(view_width,0,0);
    auto vert = vec(0,view_height,0);
    vec lower_left_corner = origin - hor/2 - vert/2 - vec(0, 0, focal);
    camera inter_cam(origin, ratio, view_height, focal); 
    the_camera = inter_cam;

    // Parse objects
    const auto& objects = j["objects"];
    for (const auto& object : objects) {
        std::string object_type = object["object_type"].get<std::string>();
        if (object_type == "sphere") {
            float sx = object["center"][0].get<float>();
            float sy = object["center"][1].get<float>();
            float sz = object["center"][2].get<float>();
            float radius = object["radius"].get<float>();
            float c1 = object["color"][0].get<float>();
            float c2 = object["color"][1].get<float>();
            float c3 = object["color"][2].get<float>();
            std::string mat_type = object["material"].get<std::string>();
            //Add sphere to scene
            double rayon = 4;
            Point center_sphere(sx,sy,sz);
            color the_sphere_color(c1,c2,c3);
            if(mat_type == "diffuse"){

                //print material type
                std::cerr<<"Material type : "<<mat_type<<" of "<< object_type <<std::endl;

                auto MAT = make_shared<diffuse>(the_sphere_color);
                sphere the_sphere(center_sphere, radius,MAT);
                Shapes.add(make_shared<sphere>(the_sphere));
            }
            else if(mat_type == "metal"){

                //print material type
                std::cerr<<"Material type : "<<mat_type<<" of "<< object_type <<std::endl;

                auto MAT = make_shared<metal>(the_sphere_color);
                sphere the_sphere(center_sphere, radius,MAT);
                Shapes.add(make_shared<sphere>(the_sphere));
            }
            else{
                std::cerr<<"Error: material not found"<<std::endl;
            }

        } else if (object_type == "plane") {
            float Ox = object["origin"][0].get<float>();
            float Oy = object["origin"][1].get<float>();
            float Oz = object["origin"][2].get<float>();
            float nx = object["normal"][0].get<float>();
            float ny = object["normal"][1].get<float>();
            float nz = object["normal"][2].get<float>();
            float c1 = object["color"][0].get<float>();
            float c2 = object["color"][1].get<float>();
            float c3 = object["color"][2].get<float>();
            std::string mat_type = object["material"].get<std::string>();
            //Add plane to scene
            vec the_normal(nx,ny,nz);
            the_normal = unit_vect(the_normal);
            Point the_origin(Ox,Oy,Oz);
            color the_plan_color(c1,c2,c3);
            if(mat_type == "diffuse"){

                //print material type
                std::cerr<<"Material type : "<<mat_type<<" of "<< object_type <<std::endl;

                auto MAT = make_shared<diffuse>(the_plan_color);
                plan the_plan(the_normal, the_origin, MAT);
                Shapes.add(make_shared<plan>(the_plan));
            }
            else if(mat_type == "metal"){

                //print material type
                std::cerr<<"Material type : "<<mat_type<<" of "<< object_type <<std::endl;

                auto MAT = make_shared<metal>(the_plan_color);
                plan the_plan(the_normal, the_origin, MAT);
                Shapes.add(make_shared<plan>(the_plan));
            }
            else{
                std::cerr<<"Error: material not found"<<std::endl;
            }
        
        } else {
            std::cerr << "Invalid object type: " << object_type << std::endl;
            exit(1);
        }
    }
}

// Creation d'une image au format PPM
int main(int argc, char *argv[]){
    //check and read json in argv
    if (argc != 2 ) {
        std::cerr << " No .json input file OR too many inputs \n Exit \n" << std::endl;
        return 1;
    }
    if(argc > 2){
        std::cerr << " Too many inputs files\n Exit \n" << std::endl;
        return 1;
    }

    std::cerr<<"Usage of : "<< argv[1] << " <json_file> as scene description"<<std::endl;
    std::ifstream f(argv[1]);
    //check if the stream is open
    if(!f.is_open()){
        std::cerr<<"Error : file "<<argv[1]<<" not found"<<std::endl;
        return 1;
    }

    json j = json::parse(f);

    camera the_camera;

    // Parse objects
    list_hittable Shapes;
    int max_depth ;
    float ratio;
    int width;
    int height;
    int number_of_sample;
    create_scene(j, Shapes, the_camera,ratio, width, height, max_depth, number_of_sample);

    std::cerr<<"Nbr objects : "<< Shapes.objects.size() <<std::endl;

    //Init of PPM file
    std::ofstream myfile;
    std::string buffer = "../outputs/image_of_"+std::to_string(width)+"_"+std::to_string(Shapes.objects.size())+"_obj_.ppm";

    myfile.open(buffer);
    myfile<<"P3\n"<<width<<" "<<height<<"\n255\n";

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //Granularité trop faible pour la parralelisation
    //#pragma omp parralel for
    for(int j = height-1; j>=0; j--){
        for(int i= 0; i<width; i++){
            color pixel_color(0.0,0.0,0.0);
            for(int k=0; k<number_of_sample; ++k){
                auto u = double(i+random_double()) / (width-1);
                auto v = double(j+random_double()) / (height-1);
                ray r = the_camera.get_ray(u,v);
                pixel_color = pixel_color + ray_color(r, Shapes, max_depth);
            }
            
            write_color(myfile,pixel_color, number_of_sample);
        }
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cerr << "\nTemps calcul image : " << elapsed_seconds.count() <<" seconds"<< std::endl;
    std::cerr<<"\nDone.\n";

    myfile.close();
    return EXIT_SUCCESS;

}