#include "cone.h"

Cone::Cone() : m_center(Vector3()), m_height(1.0f), m_radius(1.0f), m_color(Color()) {}

Cone::Cone(const Vector3& center, float height, float radius, const Color& color) : m_center(center), m_height(height), m_radius(radius), m_color(color) {}

bool Cone::intersect(const Ray& ray, float& t, Vector3& normal, Color& color, Material& mat) const {
    // Placeholder implementation - this function is not used in the raymarching algorithm
    return false;
}
