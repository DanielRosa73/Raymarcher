#include "sphere.h"
#include <cmath>

Sphere::Sphere() : m_center(Vector3(0.0f, 0.0f, 0.0f)), m_radius(1.0f), m_color(Color(1.0f, 1.0f, 1.0f)), m_material(Material(0.5f, 0.5f,0.0f)) {}

Sphere::Sphere(const Vector3& center, float radius, const Color& color, const Material& mat) : m_center(center), m_radius(radius), m_color(color), m_material(mat) {}

void Sphere::getUV(const Vector3& p, float& u, float& v) const {
    Vector3 op = (p - m_center).normalized();
    u = (1 + atan2(op.z, op.x) / M_PI) * 0.5;
    v = acos(op.y) / M_PI;
}
