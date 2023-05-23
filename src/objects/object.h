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
    //virtual ~Object() {}
    virtual float SDF(const Vector3& point) const = 0;
    virtual Color getColor() const {return m_color; }
    virtual Material getMaterial() const {return m_material; }
    virtual void getUV(const Vector3& p, float& u, float& v) const = 0;
 
protected:
    Color m_color;
    Material m_material;
};

#endif // OBJECT_H
