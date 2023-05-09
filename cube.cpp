#include "cube.h"

Cube::Cube() {}

Cube::Cube(const Vector3& center, const Vector3& dimensions, const Color& color)
    : m_center(center), m_dimensions(dimensions), m_color(color) {}

bool Cube::intersect(const Ray& ray, float& t, Vector3& normal, Color& color) const {
    // The raymarcher doesn't use this function, so it can be left empty
    return false;
}
