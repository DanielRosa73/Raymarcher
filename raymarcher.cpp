#include "raymarcher.h"
#include "camera.h"
#include "light.h"
#include "sphere.h"
#include "cube.h"
#include "scene.h"
#include "color.h"
#include "vector3.h"
#include "plane.h"
#include "cube.h"
#include "torus.h"
#include "cone.h"
#include "cubewithhole.h"


constexpr int MAX_MARCHING_STEPS = 400;
constexpr float MIN_HIT_DISTANCE = 1e-4f;
constexpr float MAX_DISTANCE = 1e14f;
constexpr float EPSILON = 1e-6f;
constexpr int SAMPLES_PER_PIXEL = 10;
constexpr float AMBIENT_LIGHT_INTENSITY = 0.2f;
constexpr int NUM_SHADOW_RAYS = 1;
constexpr float SHADOW_THRESHOLD = 0.1f;
constexpr float SHADOW_BIAS = 1e-3f;
constexpr float REFLECT_BIAS = 1e-3f;
constexpr int MAX_REFLECTION_BOUNCES = 8;

float sphereSDF(const Vector3& point, const Sphere& sphere) {
    return (point - sphere.getCenter()).length() - sphere.getRadius();
}

float planeSDF(const Vector3& point, const Plane& plane) {
    return point.dot(plane.getNormal()) - plane.getDistance();
}


float cubeSDF(const Vector3& point, const Cube& cube) {
    Vector3 p = point - cube.getCenter();
    Vector3 d = p.abs() - cube.getDimensions() * 0.5f;
    return d.max(0.0f).length() + std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);
}

float torusSDF(const Vector3& point, const Torus& torus) {
    Vector3 p = point - torus.getCenter();
    Vector3 q = Vector3((Vector3(p.x, 0.0f, p.z).length() - torus.getMajorRadius()), p.y, 0.0f);
    return q.length() - torus.getMinorRadius();
}

float coneSDF(const Vector3& point, const Cone& cone) {
    Vector3 p = point - cone.getCenter();
    Vector3 c = Vector3(0, cone.getHeight(), 0);  // cone tip at origin and in positive y direction
    float q = (p - c * std::max(0.0f, p.dot(c) / c.dot(c))).length(); // distance from p to line (c, c + cone direction)
    return std::max(-q, p.y); // q for region outside of cone, p.y for region inside cone
}


float cubeWithHoleSDF(const Vector3& point, const CubeWithHole& cubeWithHole) {
    const Cube& cube = cubeWithHole.getCube();
    const Sphere& sphere = cubeWithHole.getSphere();

    float cubeSdf = cubeSDF(point, cube);
    float sphereSdf = sphereSDF(point, sphere);

    return std::max(cubeSdf, -sphereSdf);
}


float sceneSDF(const Vector3& point, const std::vector<std::shared_ptr<Object>>& objects) {
    float min_distance = MAX_DISTANCE;
    for (const auto& object : objects) {
        float distance;
        if (auto sphere = std::dynamic_pointer_cast<Sphere>(object)) {
            distance = sphereSDF(point, *sphere);
        } else if (auto plane = std::dynamic_pointer_cast<Plane>(object)) {
            distance = planeSDF(point, *plane);
        } else if (auto cube = std::dynamic_pointer_cast<Cube>(object)) {
            distance = cubeSDF(point, *cube);
        } else if (auto torus = std::dynamic_pointer_cast<Torus>(object)) {
            distance = torusSDF(point, *torus);
        } else if (auto cone = std::dynamic_pointer_cast<Cone>(object)) {
            distance = coneSDF(point, *cone);
        } else if (auto cubeWithHole = std::dynamic_pointer_cast<CubeWithHole>(object)) {
            distance = cubeWithHoleSDF(point, *cubeWithHole);
        }
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
                } else if (auto cube = std::dynamic_pointer_cast<Cube>(object)) {
                    if (std::abs(cubeSDF(hit_point, *cube)) < MIN_HIT_DISTANCE) {
                        hit_object = cube;
                        break;
                    }
                } else if (auto torus = std::dynamic_pointer_cast<Torus>(object)) {
                    if (std::abs(torusSDF(hit_point, *torus)) < MIN_HIT_DISTANCE) {
                        hit_object = torus;
                        break;
                    }
                } else if (auto cone = std::dynamic_pointer_cast<Cone>(object)) {
                    if (std::abs(coneSDF(hit_point, *cone)) < MIN_HIT_DISTANCE) {
                        hit_object = cone;
                        break;
                    }
                } else if (auto cubeWithHole = std::dynamic_pointer_cast<CubeWithHole>(object)) {
                    if (std::abs(cubeWithHoleSDF(hit_point, *cubeWithHole)) < MIN_HIT_DISTANCE) {
                        hit_object = cubeWithHole;
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

Ray calculateReflectionRay(const Ray& ray, const Vector3& hit_point, const Vector3& normal) {
    Vector3 reflect_dir = ray.getDirection() - 2.0f * ray.getDirection().dot(normal) * normal;
    return Ray(hit_point + reflect_dir, reflect_dir);
}

Raymarcher::Raymarcher() {}



void Raymarcher::render(const Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
    const Camera& camera = scene.getCamera();
    int width = framebuffer.size();
    int height = framebuffer[0].size();

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
            object_color = sphere->getColor();
            object_material = sphere->getMaterial();
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
    return Color::lerp(Color(1.0f, 1.0f, 1.0f), Color(0.5f, 0.7f, 1.0f), t);
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







       
