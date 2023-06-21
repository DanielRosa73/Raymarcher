#include "raymarcher.h"
#include "camera.h"
#include "lights/light.h"
#include "scene.h"
#include "../utilities/color.h"
#include "../utilities/vector3.h"
#include "../objects/sphere.h"
#include "../objects/cube.h"
#include "../config.h"
#include "../objects/plane.h"
#include "../objects/cube.h"
#include "../objects/torus.h"
#include "../objects/cubewithhole.h"
#include "../objects/mandelbulb.h"
#include "../objects/frame.h"
#include "../objects/mandelbox.h"
#include "../objects/peanut.h"



#include <omp.h>





float sceneSDF(const Vector3& point, const std::vector<std::shared_ptr<Object>>& objects) {
    float min_distance = MAX_DISTANCE;
    for (const auto& object : objects) {
        float distance = object->SDF(point);
        min_distance = std::min(min_distance, distance);
    }
    return min_distance;
}


bool raymarch(const Scene& scene, const Ray& ray, Vector3& hit_point, std::shared_ptr<Object>& hit_object) {
    float total_distance = 0.0f;

    for (int i = 0; i < MAX_MARCHING_STEPS; ++i) {
        hit_point = ray.pointAtParameter(total_distance);
        float distance = sceneSDF(hit_point, scene.getObjects());

        if (distance < MIN_HIT_DISTANCE) {
            for (const auto& object : scene.getObjects()) {
                if (std::abs(object->SDF(hit_point)) < MIN_HIT_DISTANCE) {
                    hit_object = object;
                    return true;
                }
            }
        }

        total_distance += distance;

        if (total_distance >= MAX_DISTANCE) {
            break;
        }
    }

    return false;
}



Vector3 estimateNormal(const Vector3& point, const std::vector<std::shared_ptr<Object>>& objects) {
    Vector3 normal(
        sceneSDF(Vector3(point.x + EPSILON, point.y, point.z), objects) - sceneSDF(Vector3(point.x - EPSILON, point.y, point.z), objects),
        sceneSDF(Vector3(point.x, point.y + EPSILON, point.z), objects) - sceneSDF(Vector3(point.x, point.y - EPSILON, point.z), objects),
        sceneSDF(Vector3(point.x, point.y, point.z + EPSILON), objects) - sceneSDF(Vector3(point.x, point.y, point.z - EPSILON), objects)
    );
    return normal.normalized();
}

Ray calculateReflectionRay(const Ray& ray, const Vector3& hit_point, const Vector3& normal) {
    Vector3 reflect_dir = ray.getDirection() - 2.0f * ray.getDirection().dot(normal) * normal;
    return Ray(hit_point + reflect_dir, reflect_dir);
}

Raymarcher::Raymarcher() {}


void Raymarcher::render(const Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
    const Camera& camera = scene.getCamera();
    int width = framebuffer.size();
    int height = framebuffer[0].size();

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            Ray ray = camera.getRay(u, v);
            framebuffer[i][j] = trace(scene, ray,0);
        }
    }
}



void Raymarcher::render_antialiasing(const Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
    const Camera& camera = scene.getCamera();
    int width = framebuffer.size();
    int height = framebuffer[0].size();

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Color pixel_color(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
                float u = float(i + (rand() / (RAND_MAX + 1.0))) / float(width);
                float v = float(j + (rand() / (RAND_MAX + 1.0))) / float(height);
                Ray ray = camera.getRay(u, v);
                pixel_color += trace(scene, ray,0);
            }
            framebuffer[i][j] = pixel_color / float(SAMPLES_PER_PIXEL);
        }
    }
}

void Raymarcher::render_MSAA(const Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
    const Camera& camera = scene.getCamera();
    int width = framebuffer.size();
    int height = framebuffer[0].size();

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            Color pixel_color(0.0f, 0.0f, 0.0f);
            
            
            for (int m = 0; m < sqrt(SAMPLES_PER_PIXEL); ++m) {
                for (int n = 0; n < sqrt(SAMPLES_PER_PIXEL); ++n) {
                    float u = (i + (m / sqrt(SAMPLES_PER_PIXEL)) + (rand() / (RAND_MAX + 1.0))) / width;
                    float v = (j + (n / sqrt(SAMPLES_PER_PIXEL)) + (rand() / (RAND_MAX + 1.0))) / height;
                    Ray ray = camera.getRay(u, v);
                    pixel_color += trace(scene, ray, 0);
                }
            }
            framebuffer[i][j] = pixel_color / float(SAMPLES_PER_PIXEL);
        }
    }
}




Color Raymarcher::trace(const Scene& scene, const Ray& ray, int depth) {
    Vector3 hit_point;
    std::shared_ptr<Object> hit_object;
    bool hit = raymarch(scene, ray, hit_point, hit_object);

    if (hit && hit_object) {
        Vector3 normal = estimateNormal(hit_point, scene.getObjects());
        Material object_material = hit_object->getMaterial();
        Color object_color;

        
        if (object_material.texture) {
            float u, v;
            hit_object->getUV(hit_point, u, v);
            object_color = object_material.texture->getColor(u, v);
        } else {
            object_color = hit_object->getColor();
        }

        
        Color local_color = shade(scene, hit_point + EPSILON, normal, object_material, object_color, ray);

       
        if (object_material.reflectivity <= 0 || depth >= MAX_REFLECTION_BOUNCES) {
            return local_color;
        }

        
        Vector3 reflection_direction = ray.getDirection().reflect(normal).normalized();
        Ray reflection_ray(hit_point + reflection_direction, reflection_direction);

       
        Color reflected_color = trace(scene, reflection_ray, depth + 1);

       
        return local_color * (1.0f - object_material.reflectivity) + reflected_color * object_material.reflectivity;
    } 
    else {
        if (BLACK == true)
            return getBackgroundColorBlack(ray);
        if (NIGHT == true)
            return getNightBackgroundColor(ray);
        return getBackgroundColor(ray);
    }
}





Color Raymarcher::shade(const Scene& scene, const Vector3& point, const Vector3& normal, const Material& object_material, const Color& object_color, const Ray& ray) {
    const auto& lights = scene.getLights();
    
    Color result = object_color * AMBIENT_LIGHT_INTENSITY; 

    for (const auto& light : lights) {
        Vector3 light_direction = (light->getPosition() - point).normalized();
        Vector3 view_direction = (ray.getDirection() * -1.0f).normalized();
        Vector3 half_vector = (light_direction + view_direction).normalized();

        float shadow_ratio = shadow(scene, point, *light);  

       
        if (shadow_ratio < 1.0f) {
        
            float diffuse = std::max(normal.dot(light_direction), 0.0f);
            Color diffuse_color = object_color * diffuse * object_material.diffuse;

            
            float shininess = object_material.specular;
         
            float specular = std::pow(std::max(normal.dot(half_vector), 0.0f), shininess);

            Color specular_color = light->getColor() * specular * (1.0f - shadow_ratio);

            result = result + diffuse_color + specular_color;
        }
    }

    return result;
}




Color Raymarcher::getBackgroundColor(const Ray& ray) const {
    Vector3 unitDirection = ray.getDirection().normalized();
    float t = 0.5f * (unitDirection.y + 1.0f);
    Color sky = Color(0.04f,0.28f,0.68f);
    return Color::lerp(Color(1.0f, 1.0f, 1.0f), sky, t);
}

Color Raymarcher::getNightBackgroundColor(const Ray& ray) const {
    Vector3 unitDirection = ray.getDirection().normalized();
    float t = 0.5f * (unitDirection.y + 1.0f);
    Color nightSky = Color(0.005f, 0.005f, 0.02f); 
    Color starColor = Color(1.0f, 1.0f, 1.0f); 

    Color color = Color::lerp(starColor, nightSky, t);

    float random = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < 0.01f) ? 1.0f : 0.0f;

    color = color * (0.3f - random) + starColor * random;

    return color;
}

Color Raymarcher::getBackgroundColorBlack(const Ray& ray) const {
    
    Color starColor = Color(1.0f, 1.0f, 1.0f); 

    Color color(0,0,0);

    float random = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) < 0.001f) ? 1.0f : 0.0f;

    color = color * (0.3f - random) + starColor * random;
    return color;
}

float Raymarcher::shadow(const Scene& scene, const Vector3& point, const Light& light) {
    int shadowCount = 0;

    for (int i = 0; i < NUM_SHADOW_RAYS; ++i) {
        Vector3 light_position = light.getRandomPoint();
        Vector3 light_direction = (light_position - point).normalized();
        Vector3 startPoint = point + light_direction * SHADOW_BIAS;
        float distanceToLight = (light_position - point).length();
        float totalDistance = 0.0f;

        for (int j = 0; j < MAX_MARCHING_STEPS; ++j) {
            Vector3 currentPoint = startPoint + light_direction * totalDistance;
            float distance = sceneSDF(currentPoint, scene.getObjects());

            if (distance < MIN_HIT_DISTANCE) {
                shadowCount += 1;
                break;
            }

            totalDistance += distance;

            if (totalDistance >= distanceToLight) {
                break;
            }
        }
    }

    return (float)shadowCount / NUM_SHADOW_RAYS;
}



       
