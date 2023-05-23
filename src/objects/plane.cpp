#include "plane.h"

Plane::Plane() : m_normal(Vector3(0.0f, 1.0f, 0.0f)), m_distance(0.0f) {
        this->m_color = Color(0.5,0.5,0.5);
        this->m_material = Material();
}

Plane::Plane(const Vector3& normal, float distance, const Color& color, const Material& mat) : m_normal(normal.normalized()), m_distance(distance){
        this->m_color = color;
        this->m_material = mat;
}

float Plane::SDF(const Vector3& point) const {
    return point.dot(m_normal) - m_distance;
}
