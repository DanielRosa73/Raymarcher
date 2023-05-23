#include "sphere.h"
#include <cmath>

Sphere::Sphere() : m_center(Vector3(0.0f, 0.0f, 0.0f)), m_radius(1.0f) {
        this->m_color = Color(1,0,1);
        this->m_material = Material();
}

Sphere::Sphere(const Vector3& center, float radius, const Color& color, const Material& mat) : m_center(center), m_radius(radius) {
        this->m_color = color;
        this->m_material = mat;
}

void Sphere::getUV(const Vector3& p, float& u, float& v) const {
    Vector3 op = (p - m_center).normalized();
    u = (1 + atan2(op.z, op.x) / M_PI) * 0.5;
    v = acos(op.y) / M_PI;
}

float Sphere::SDF(const Vector3& point) const{
    return (point - m_center).length() - m_radius;
}

