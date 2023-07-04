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
    Material soft(1,100000,0);



    Sphere sphere1(Vector3(-1.2, 0.25f, -1.6f), 1.6f, Color(0.37f, 0.06f, 0.57f), mat1);
    Sphere sphere11(Vector3(1.2f, 0.25f, -1.6f), 1.6f, Color(0.12f, 0.28f, 0.59f), mat1);

    Peanut peanut(sphere1, sphere11, 0.8f);


    
    Scene scene;
    scene.setCamera(camera);
    //scene.addObject(std::make_shared<Peanut>(peanut));
    scene.addObject(std::make_shared<Peanut>(peanut));
    





    
    
    scene.addLight(std::make_shared<Light>(light));
    


    std::vector<std::vector<Color>> framebuffer(width, std::vector<Color>(height));
    Raymarcher raymarcher;
    raymarcher.render_antialiasing(scene, framebuffer);

    write_ppm("output.ppm", framebuffer);

    return 0;
}
*/



// Main for the mandelbulb
int main() {
    int width = 800;
    int height = 600;


    //Camera camera(Vector3(0.0f, 0.0f, 0.4f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));
    
    //Zoomed in
    Camera camera(Vector3(0.0f, 1.0f, 0.2f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));

    //Bottom view
    //Camera camera(Vector3(0.0f, 0.0f, 2.4f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));

    //Full View
    //Camera camera(Vector3(0.0f, 2.0f, 2.4f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 60.0f, float(width) / float(height));

    Light light(Vector3(0.0f, 7.0f, 1.5f), Color(1,1,1), 0.2f);

    Material mat(1,10000,0.0f);

    Scene scene;
    scene.setCamera(camera);
    scene.addLight(std::make_shared<Light>(light));
    

    Mandelbulb mandelbulb(Vector3(0,0,0),1.0f, 8, Color(0.43f, 0.34f, 0.24f),mat);
    scene.addObject(std::make_shared<Mandelbulb>(mandelbulb));

    std::vector<std::vector<Color>> framebuffer(width, std::vector<Color>(height));
    Raymarcher raymarcher;
    raymarcher.render(scene, framebuffer);

    write_ppm("output.ppm", framebuffer);

    return 0;
}


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
/*
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
    raymarcher.render_MSAA(scene, framebuffer);
    write_ppm("output.ppm", framebuffer);
    return 0;
}
*/
