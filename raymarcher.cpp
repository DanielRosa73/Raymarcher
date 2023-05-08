#include "raymarcher.h"
#include "camera.h"
#include "light.h"
#include "sphere.h"
#include "scene.h"
#include "color.h"
#include "vector3.h"
#include "plane.h"

constexpr int MAX_MARCHING_STEPS = 400;
constexpr float MIN_HIT_DISTANCE = 1e-4f;
constexpr float MAX_DISTANCE = 1e14f;
constexpr float EPSILON = 1e-6f;
constexpr int SAMPLES_PER_PIXEL = 10;
constexpr float AMBIENT_LIGHT_INTENSITY = 0.2f;
constexpr int NUM_SHADOW_RAYS = 32;
constexpr float SHADOW_THRESHOLD = 0.5f;

float sphereSDF(const Vector3& point, const Sphere& sphere) {
    return (point - sphere.getCenter()).length() - sphere.getRadius();
}

float planeSDF(const Vector3& point, const Plane& plane) {
    return point.dot(plane.getNormal()) - plane.getDistance();
}

float sceneSDF(const Vector3& point, const std::vector<std::shared_ptr<Object>>& objects) {
    float min_distance = MAX_DISTANCE;
    for (const auto& object : objects) {
        if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
            min_distance = std::min(min_distance, sphereSDF(point, *sphere));
        } else if (auto plane = std::dynamic_pointer_cast<Plane>(object)) {
            min_distance = std::min(min_distance, planeSDF(point, *plane));
        }
    }
    return min_distance;
}


bool raymarch(const Scene& scene, const Ray& ray, Vector3& hit_point, std::shared_ptr<Object>& hit_object) {
    float total_distance = 0.0f;
    for (int i = 0; i < MAX_MARCHING_STEPS; ++i) {
        hit_point = ray.pointAtParameter(total_distance);
        float distance = sceneSDF(hit_point, scene.getObjects());
        if (distance < MIN_HIT_DISTANCE) {
            hit_object = nullptr;
            for (const auto& object : scene.getObjects()) {
                if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
                    if (std::abs(sphereSDF(hit_point, *sphere)) < MIN_HIT_DISTANCE) {
                        hit_object = sphere;
                        break;
                    }
                } else if (auto plane = std::dynamic_pointer_cast<Plane>(object)) {
                    if (std::abs(planeSDF(hit_point, *plane)) < MIN_HIT_DISTANCE) {
                        hit_object = plane;
                        break;
                    }
                }
            }
            return true;
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


Raymarcher::Raymarcher() {}

//NO ANTIALIASING
void Raymarcher::render(const Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
    const Camera& camera = scene.getCamera();
    int width = framebuffer.size();
    int height = framebuffer[0].size();

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            Ray ray = camera.getRay(u, v);
            framebuffer[i][j] = trace(scene, ray);
        }
    }
}

//ANTIALIASING
/*
void Raymarcher::render(const Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
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
                pixel_color += trace(scene, ray);
            }
            framebuffer[i][j] = pixel_color / float(SAMPLES_PER_PIXEL);
        }
    }
}
*/



Color Raymarcher::trace(const Scene& scene, const Ray& ray) {
    Vector3 hit_point;
    std::shared_ptr<Object> hit_object;
    Color accumulated_color(0.0f, 0.0f, 0.0f);
    Color transparency(1.0f, 1.0f, 1.0f);
    Ray current_ray = ray;

    if (raymarch(scene, current_ray, hit_point, hit_object)) {
        Vector3 normal = estimateNormal(hit_point, scene.getObjects());
        Color object_color;
        float object_opacity;

        if (auto sphere = std::dynamic_pointer_cast<Sphere>(hit_object)) {
            object_color = sphere->getColor();
            object_opacity = sphere->getOpacity();
        } else if (auto plane = std::dynamic_pointer_cast<Plane>(hit_object)) {
            object_color = plane->getColor();
            object_opacity = plane->getOpacity();
        }

        Color shaded_color = shade(scene, hit_point, normal, object_color, current_ray);
        accumulated_color += transparency * shaded_color * (1.0f - object_opacity);
        transparency *= Color(object_opacity, object_opacity, object_opacity);

        // If the object is transparent, trace a ray through it and add the color behind it
        if (object_opacity < 1.0f) {
            Ray refracted_ray(hit_point + current_ray.getDirection() * EPSILON, current_ray.getDirection());
            accumulated_color += transparency * trace(scene, refracted_ray);
        }
    } else {
        accumulated_color += transparency * getBackgroundColor(current_ray);
    }

    return accumulated_color;
}




Color Raymarcher::shade(const Scene& scene, const Vector3& point, const Vector3& normal, const Color& object_color, const Ray& ray) {
    const auto& lights = scene.getLights();
    
    //Uncomment for no ambient lightning
    //Color result(0.0f, 0.0f, 0.0f);

    //Comment for no ambient lighting
    Color result = object_color * AMBIENT_LIGHT_INTENSITY;

    for (const auto& light : lights) {
        Vector3 light_direction = (light->getPosition() - point).normalized();
        Vector3 view_direction = (ray.getDirection() * -1.0f).normalized();
        Vector3 half_vector = (light_direction + view_direction).normalized();
        if (!shadow(scene, point, *light)) {
            // Diffuse lighting
            float diffuse = std::max(normal.dot(light_direction), 0.0f);
            Color diffuse_color = object_color * light->getColor() * diffuse;

            // Specular lighting
            float shininess = 32.0f;
            float specular = std::pow(std::max(normal.dot(half_vector), 0.0f), shininess);
            Color specular_color = light->getColor() * specular;

            result = result + diffuse_color + specular_color;
        }
    }

    return result;
}





Color Raymarcher::getBackgroundColor(const Ray& ray) const {
    Vector3 unitDirection = ray.getDirection().normalized();
    float t = 0.5f * (unitDirection.y + 1.0f);
    return Color::lerp(Color(1.0f, 1.0f, 1.0f), Color(0.5f, 0.7f, 1.0f), t);
}



bool Raymarcher::shadow(const Scene& scene, const Vector3& point, const Light& light) {
    Vector3 light_direction = (light.getPosition() - point).normalized();
    Ray shadow_ray(point + light_direction * EPSILON, light_direction);
    float t = (light.getPosition() - point).length();
    const auto& objects = scene.getObjects();

    for (const auto& object : objects) {
        float temp_t;
        Vector3 temp_normal;
        Color temp_color;

        if (object->intersect(shadow_ray, temp_t, temp_normal, temp_color) && temp_t < t) {
            return true;
        }
    }

    return false;
    }