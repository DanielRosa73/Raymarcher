#include "raymarcher.h"
#include "camera.h"
#include "lights/light.h"
#include "scene.h"
#include "../utilities/color.h"
#include "../utilities/vector3.h"
#include "../objects/sdf.h"
#include "../objects/sphere.h"
#include "../objects/cube.h"
#include "../objects/plane.h"
#include "../objects/cube.h"
#include "../objects/torus.h"
#include "../objects/cone.h"
#include "../objects/cubewithhole.h"
#include "../objects/mandelbulb.h"
#include "../objects/menger_sponge.h"
#include "../objects/frame.h"
#include "../objects/mandelbox.h"


#include <omp.h>

constexpr int MAX_MARCHING_STEPS = 400;
constexpr float MIN_HIT_DISTANCE = 1e-4f;
constexpr float MAX_DISTANCE = 1e14f;
constexpr float EPSILON = 1e-6f;
constexpr int SAMPLES_PER_PIXEL = 10;
constexpr float AMBIENT_LIGHT_INTENSITY = 0.2f;
constexpr int NUM_SHADOW_RAYS = 32;
constexpr float SHADOW_THRESHOLD = 0.1f;
constexpr float SHADOW_BIAS = 1e-3f;
constexpr float REFLECT_BIAS = 1e-3f;
constexpr int MAX_REFLECTION_BOUNCES = 8;



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



Color Raymarcher::trace(const Scene& scene, const Ray& ray, int depth) {
    Vector3 hit_point;
    std::shared_ptr<Object> hit_object;
    bool hit = raymarch(scene, ray, hit_point, hit_object);

    if (hit && hit_object) {
        Vector3 normal = estimateNormal(hit_point, scene.getObjects());
        Color object_color;
        Material object_material;
        if (auto sphere = std::dynamic_pointer_cast<Sphere>(hit_object)) {
            object_material = sphere->getMaterial();
            if (object_material.texture) {
                float u, v;
                sphere->getUV(hit_point, u, v);
                object_color = object_material.texture->getColor(u, v);
            } else {
                object_color = sphere->getColor();
            }
        } else if (auto plane = std::dynamic_pointer_cast<Plane>(hit_object)) {
            object_color = plane->getColor();
        } else if (auto cube = std::dynamic_pointer_cast<Cube>(hit_object)) {
            object_color = cube->getColor();
            object_material = cube->getMaterial();
        } else if (auto torus = std::dynamic_pointer_cast<Torus>(hit_object)) {
            object_color = torus->getColor();
            object_material = torus->getMaterial();
        } else if (auto cubeWithHole = std::dynamic_pointer_cast<CubeWithHole>(hit_object)) {
                object_color = cubeWithHole->getCube().getColor();
                object_material = cubeWithHole->getCube().getMaterial();
        } else if (auto mandelbulb = std::dynamic_pointer_cast<Mandelbulb>(hit_object)) {
            object_color = mandelbulb->getColor();
            object_material = mandelbulb->getMaterial();
        } else if (auto frame = std::dynamic_pointer_cast<Frame>(hit_object)) {
            object_color = frame->getCube().getColor();
            object_material = frame->getCube().getMaterial();
        } else if (auto mengerSponge = std::dynamic_pointer_cast<MengerSponge>(hit_object)) {
            object_color = mengerSponge->getColor();
            object_material = mengerSponge->getMaterial();
        } else if (auto mandelbox = std::dynamic_pointer_cast<Mandelbox>(hit_object)) {
            object_color = mandelbox->getColor();
            object_material = mandelbox->getMaterial();
        }

        // Phong shading for the local color.
        Color local_color = shade(scene, hit_point + EPSILON, normal, object_material, object_color, ray);

        // If the object is not reflective or we've hit the recursive limit, we're done.
        if (object_material.reflectivity <= 0 || depth >= MAX_REFLECTION_BOUNCES) {
            return local_color;
        }

        // Calculate reflection direction and new ray.
        Vector3 reflection_direction = ray.getDirection().reflect(normal).normalized();
        Ray reflection_ray(hit_point + reflection_direction, reflection_direction);

        // Recursive call for the reflected color.
        Color reflected_color = trace(scene, reflection_ray, depth + 1);

        // Combine local and reflected color based on material reflectivity.
        return local_color * (1.0f - object_material.reflectivity) + reflected_color * object_material.reflectivity;
    } 
    else {
        // Background color if no hit.
        return getBackgroundColor(ray);
    }
}




Color Raymarcher::shade(const Scene& scene, const Vector3& point, const Vector3& normal, const Material& object_material, const Color& object_color, const Ray& ray) {
    const auto& lights = scene.getLights();
    
    Color result = object_color * AMBIENT_LIGHT_INTENSITY; // Ambient light

    for (const auto& light : lights) {
        Vector3 light_direction = (light->getPosition() - point).normalized();
        Vector3 view_direction = (ray.getDirection() * -1.0f).normalized();
        Vector3 half_vector = (light_direction + view_direction).normalized();

        float shadow_ratio = shadow(scene, point, *light);  // Calculate shadow ratio

        // Only calculate lighting if the point is not fully in shadow
        if (shadow_ratio < 1.0f) {
            // Diffuse lighting
            float diffuse = std::max(normal.dot(light_direction), 0.0f);
            Color diffuse_color = object_color * diffuse * object_material.diffuse;

            // Specular lighting
            float shininess = object_material.specular;
            //float specular = std::pow(std::max(normal.dot(half_vector), 0.8f), shininess);
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



       
