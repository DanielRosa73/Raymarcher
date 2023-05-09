#include "cube.h"


Cube::Cube(const Vector3& center, const Vector3& dimensions, const Color& color, const Material& mat)
    : m_center(center), m_dimensions(dimensions), m_color(color), m_material(mat) {}

bool Cube::intersect(const Ray& ray, float& t, Vector3& normal, Color& color, Material& mat) const {
    // The raymarcher doesn't use this function, so it can be left empty
    return false;
}
