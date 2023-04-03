#include <fstream>
#include <iostream>
#include <vector>
#include "../include/extern/json.hpp"

using json = nlohmann::json;

int main() {
    std::ifstream ifs("test.json");
    json j = json::parse(ifs);

    // Parse image
    const auto& image = j["image"];
    int width = image["width"].get<int>();
    int max_depth = image["max_depth"].get<int>();
    auto aspect_ratio = image["aspect_ratio"];
    // TODO: add image data to scene
    //print all value above
    std::cout << "width: " << width << std::endl;
    std::cout << "max_depth: " << max_depth << std::endl;
    std::cout << "aspect_ratio (vector): " << aspect_ratio << std::endl;
    

    // Parse camera
    const auto& camera = j["camera"];
    float cx = camera["origin"][0].get<float>();
    float cy = camera["origin"][1].get<float>();
    float cz = camera["origin"][2].get<float>();
    float lx = camera["look_at"][0].get<float>();
    float ly = camera["look_at"][1].get<float>();
    float lz = camera["look_at"][2].get<float>();
    float ux = camera["up_vector"][0].get<float>();
    float uy = camera["up_vector"][1].get<float>();
    float uz = camera["up_vector"][2].get<float>();
    float fov = camera["vertical_fov"].get<float>();


    // Parse objects
    const auto& objects = j["objects"];
    for (const auto& object : objects) {
        std::string object_type = object["object_type"].get<std::string>();
        if (object_type == "sphere") {
            float sx = object["center"][0].get<float>();
            float sy = object["center"][1].get<float>();
            float sz = object["center"][2].get<float>();
            float radius = object["radius"].get<float>();
            float r = object["color"][0].get<float>();
            float g = object["color"][1].get<float>();
            float b = object["color"][2].get<float>();
            // TODO: add sphere to scene
        } else if (object_type == "plane") {
            float px = object["origin"][0].get<float>();
            float py = object["origin"][1].get<float>();
            float pz = object["origin"][2].get<float>();
            float nx = object["normal"][0].get<float>();
            float ny = object["normal"][1].get<float>();
            float nz = object["normal"][2].get<float>();
            float r = object["color"][0].get<float>();
            float g = object["color"][1].get<float>();
            float b = object["color"][2].get<float>();
            // TODO: add plane to scene
        } else {
            std::cerr << "Invalid object type: " << object_type << std::endl;
        }
    }

    return 0;
}
