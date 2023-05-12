#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include "scene.h"
#include "color.h"
#include "torus.h"
#include <vector>

class Raymarcher {
public:
    Raymarcher();

    void render(const Scene& scene, std::vector<std::vector<Color>>& framebuffer);
    void render_antialiasing(const Scene& scene, std::vector<std::vector<Color>>& framebuffer);

private:
    Color trace(const Scene& scene, const Ray& ray);
    Color shade(const Scene& scene, const Vector3& point, const Vector3& normal, const Material& object_material, const Color& object_color, const Ray& ray);
    Color getBackgroundColor(const Ray& ray) const;
    float shadow(const Scene& scene, const Vector3& point, const Light& light);
    const float EPSILON = 1e-4;
};

#endif // RAYMARCHER_H
