#include "engine/camera.h"
#include "lights/light.h"
#include "objects/sphere.h"
#include "engine/scene.h"
#include "engine/raymarcher.h"
#include "utilities/color.h"
#include "utilities/vector3.h"
#include "objects/plane.h"
#include "objects/cube.h"
#include "objects/torus.h"
#include "objects/cubewithhole.h"
#include "objects/mandelbulb.h"
#include "objects/frame.h"
#include "objects/mandelbox.h"
#include "objects/peanut.h"
#include "include/stb_image.h"

#include <vector>
#include <iostream>
#include <fstream>

void write_ppm(const std::string& filename, const std::vector<std::vector<Color>>& framebuffer) {
    int width = framebuffer.size();
    int height = framebuffer[0].size();

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open the output file." << std::endl;
        return;
    }

    file << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            int r = int(255.99f * framebuffer[i][j].r());
            int g = int(255.99f * framebuffer[i][j].g());
            int b = int(255.99f * framebuffer[i][j].b());
            file << r << " " << g << " " << b << " ";
        }
        file << "\n";
    }

    file.close();
}


/*
int main() {
    int width = 800;
    int height = 600;

    Camera camera(Vector3(0.0f, 1.5f, 3.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));
    Light light(Vector3(1.0f, 3.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), 0.0f);

    Material mat1(1, 1000,0.2f);
    Material mat11(1, 1000,0.2f);
    Material mat2(1, 10000,0.0f);
    Material mat3(1, 10000,0.0f);
    Material mat4(1, 10000,0.0f);
    Material mat5(1, 10000,0.0f);
    Material mat6(1, 10000,0.0f);
    Material mat7(1, 10000,0.0f);
    Material mat8(1, 10000,0.0f);
    Material mat(1, 10000,0.0f);

    


    Sphere sphere1(Vector3(-1.2, 0.25f, -1.6f), 1.6f, Color(0.1f, 0.1f, 0.9f), mat1);
    Sphere sphere11(Vector3(1.2f, 0.25f, -1.6f), 1.6f, Color(0.12f, 0.28f, 0.59f), mat1);
    Sphere sphere2(Vector3(0.0f, 0.4f, 1.0f), 0.5f, Color(0.0f, 1.0f, 1.0f), mat2);
    Sphere sphere3(Vector3(-2.3f, -1.0f, 1.9f), 0.5f, Color(1.0f, 0.0f, 1.0f), mat3);
    Sphere sphere4(Vector3(-0.5f, -1.0f, 3.2f), 0.5f, Color(0.2f, 0.8f, 0.2f), mat4);
    Sphere sphere5(Vector3(0.5f, -1.0f, 0.6f), 0.5f, Color(1.0f, 0.64f, 0.0f), mat5);
    Sphere sphere6(Vector3(0.75f, -1.0f, 2.3f), 0.5f, Color(0.3f, 0.4f, 0.3f), mat6);
    Sphere sphere7(Vector3(2.4f, -1.0f, 1.0f), 0.5f, Color(0.1f, 0.5f, 0.4f), mat7);
    Sphere sphere8(Vector3(2.0f, -1.0f, 2.7f), 0.5f, Color(1.0f, 0.84f, 0.4f), mat8);

    Peanut peanut(sphere1, sphere11, 0.4f);
    
    Scene scene;
    scene.setCamera(camera);
    scene.addObject(std::make_shared<Peanut>(peanut));
    scene.addObject(std::make_shared<Sphere>(sphere2));

    
    
    scene.addLight(std::make_shared<Light>(light));
    
    scene.addObject(std::make_shared<Sphere>(sphere1));
    scene.addObject(std::make_shared<Sphere>(sphere11));
    scene.addObject(std::make_shared<Sphere>(sphere2));
    scene.addObject(std::make_shared<Sphere>(sphere3));
    scene.addObject(std::make_shared<Sphere>(sphere4));
    scene.addObject(std::make_shared<Sphere>(sphere5));
    scene.addObject(std::make_shared<Sphere>(sphere6));
    scene.addObject(std::make_shared<Sphere>(sphere7));
    scene.addObject(std::make_shared<Sphere>(sphere8));
    

    
    
    Vector3 cubeCenterL(-5, 1 , -3);
    Vector3 cubeCenterR(5, 1 , -3);
    Vector3 cubeCenterBack(0, 1 , -5);
    Vector3 cubeCenterBot(0, -2 , 0);
    Vector3 cubeCenterTop(0, 5 , 0);
    Vector3 cubeDimensions(1, 10, 10);
    Vector3 cubeDimensions2(10, 10, 1);
    Vector3 cubeDimensions3(10, 1, 10);
    Color cubeColor1(0.05, 0.05, 0.05);
    Color cubeColor2(0.05, 0.05, 0.05);
    
    std::shared_ptr<Cube> cubeL = std::make_shared<Cube>(cubeCenterL, cubeDimensions, cubeColor1, mat);
    std::shared_ptr<Cube> cubeR = std::make_shared<Cube>(cubeCenterR, cubeDimensions, cubeColor1, mat);
    std::shared_ptr<Cube> cubeBack = std::make_shared<Cube>(cubeCenterBack, cubeDimensions2, cubeColor2, mat);
    std::shared_ptr<Cube> cubeBot = std::make_shared<Cube>(cubeCenterBot, cubeDimensions3, cubeColor2, mat);
    std::shared_ptr<Cube> cubeTop = std::make_shared<Cube>(cubeCenterTop, cubeDimensions3, cubeColor2, mat);
    scene.addObject(cubeL);
    scene.addObject(cubeR);
    scene.addObject(cubeBack);
    scene.addObject(cubeBot);
    scene.addObject(cubeTop);
    


    std::vector<std::vector<Color>> framebuffer(width, std::vector<Color>(height));
    Raymarcher raymarcher;
    raymarcher.render_MSAA(scene, framebuffer);

    write_ppm("output.ppm", framebuffer);

    return 0;
}
*/



// Main for the mandelbulb
/*
int main() {
    int width = 800;
    int height = 600;


    //Camera camera(Vector3(0.0f, 0.0f, 0.4f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));
    
    //Zoomed in
    //Camera camera(Vector3(0.0f, 1.0f, 0.2f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));

    //Bottom view
    //Camera camera(Vector3(0.0f, 0.0f, 2.4f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));

    //Full View
    Camera camera(Vector3(0.0f, 2.0f, 2.4f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));

    Light light(Vector3(0.0f, 7.0f, 1.5f), Color(1.0f, 1.0f, 1.0f), 0.2f);

    Material mat(1,10000,0.0f);

    Scene scene;
    scene.setCamera(camera);
    scene.addLight(std::make_shared<Light>(light));
    

    Mandelbulb mandelbulb(Vector3(0,0,0),1.0f, 8, Color(1.0f,0.0f,1.0f),mat);
    scene.addObject(std::make_shared<Mandelbulb>(mandelbulb));

    std::vector<std::vector<Color>> framebuffer(width, std::vector<Color>(height));
    Raymarcher raymarcher;
    raymarcher.render(scene, framebuffer);

    write_ppm("output.ppm", framebuffer);

    return 0;
}
*/


//Main for Space
/*
int main() {
    int width = 800;
    int height = 600;

    Camera camera(Vector3(0.0f, 1.5f, 7.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));
    Light light(Vector3(-2.5f, 7.0f, -1.8f), Color(1.0f, 1.0f, 1.0f), 1.0f);


    Material soft(1, 10000,0.0f);
   
    std::shared_ptr<ImageTexture> moon = std::make_shared<ImageTexture>("src/textures/moon.png");
    std::shared_ptr<ImageTexture> mars = std::make_shared<ImageTexture>("src/textures/mars.png");
    Material mooon(1,1000,0, moon);
    Material maars(1,100000000,0, mars);

 
    Sphere sphere1(Vector3(0.9, -0.1f, 4.6f), 1.8f, Color(1.0f, 0.84f, 0.0f), maars);
    Sphere sphere11(Vector3(-2.5f, 3.9f, -3.0f), 0.8f, Color(0.75f, 0.75f, 0.575f), mooon);
    Torus torus(Vector3(0.9, 0.4f, 4.6f), 2.4, 0.2 ,Color(0.7,0.2,0.17), soft);
 
    Scene scene;
    scene.setCamera(camera);

    scene.addLight(std::make_shared<Light>(light));
    
    scene.addObject(std::make_shared<Sphere>(sphere1));
    scene.addObject(std::make_shared<Torus>(torus));
    scene.addObject(std::make_shared<Sphere>(sphere11));

    std::vector<std::vector<Color>> framebuffer(width, std::vector<Color>(height));
    Raymarcher raymarcher;
    raymarcher.render(scene, framebuffer);
    write_ppm("output.ppm", framebuffer);
    return 0;
}
*/


//Main for cube
int main() {
    int width = 800;
    int height = 600;

    Camera camera(Vector3(0.0f, 1.5f, 5.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));
    Light light(Vector3(-2.5f, 7.0f, -1.8f), Color(1.0f, 1.0f, 1.0f), 1.0f);


    Material soft(1, 10000,0.0f);
   
    std::shared_ptr<ImageTexture> cob = std::make_shared<ImageTexture>("src/textures/cobble.png");
    Material cobble(1,1000,0, cob);

    std::shared_ptr<ImageTexture> obs = std::make_shared<ImageTexture>("src/textures/obsidian.png");
    Material obsidian(1,1000,0, obs);

    std::shared_ptr<ImageTexture> dia = std::make_shared<ImageTexture>("src/textures/diamond.png");
    Material diamond(1,1000,0, dia);

    std::shared_ptr<ImageTexture> gol = std::make_shared<ImageTexture>("src/textures/gold.png");
    Material gold(1,1000,0.1, gol);


    Cube cube1(Vector3(0.7,0.51,0), Vector3(1,1,1), Color(1,0,0), cobble);
    Cube cube2(Vector3(-0.7,0.51,0), Vector3(1,1,1), Color(1,0,0), diamond);
    Cube cube3(Vector3(-2.1,0.51,0), Vector3(1,1,1), Color(1,0,0), obsidian);
    Cube cube4(Vector3(2.1,0.51,0), Vector3(1,1,1), Color(1,0,0), gold);


    
    Scene scene;
    scene.setCamera(camera);

    scene.addLight(std::make_shared<Light>(light));
    
    Plane plane(Vector3(0,1,0), 0, Color(0.5,0.5,0.5), soft);
    scene.addObject(std::make_shared<Plane>(plane));

    scene.addObject(std::make_shared<Cube>(cube1));
    scene.addObject(std::make_shared<Cube>(cube2));
    scene.addObject(std::make_shared<Cube>(cube3));
    scene.addObject(std::make_shared<Cube>(cube4));



    std::vector<std::vector<Color>> framebuffer(width, std::vector<Color>(height));
    Raymarcher raymarcher;
    raymarcher.render(scene, framebuffer);
    write_ppm("output.ppm", framebuffer);
    return 0;
}
