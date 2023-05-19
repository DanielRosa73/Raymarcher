#ifndef OBJECT_H
#define OBJECT_H

#include "../engine/ray.h"
#include "../utilities/vector3.h"
#include "../textures/image_texture.h"
#include "../utilities/color.h"
#include <memory>

struct Material {
    float diffuse;
    float specular;
    float reflectivity;
    std::shared_ptr<ImageTexture> texture;  // Optional texture

    Material() : diffuse(1), specular(1), reflectivity(0), texture(nullptr) {}
    Material(float diffuse, float specular, float reflectivity) : diffuse(diffuse), specular(specular), reflectivity(reflectivity), texture(nullptr) {}
    Material(float diffuse, float specular, float reflectivity, const std::shared_ptr<ImageTexture>& texture) : diffuse(diffuse), specular(specular), reflectivity(reflectivity), texture(texture) {}
};


class Object {
public:
    virtual ~Object() {}
};

#endif // OBJECT_H
