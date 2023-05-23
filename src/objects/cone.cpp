#include "cone.h"

Cone::Cone() : m_center(Vector3()), m_height(1.0f), m_radius(1.0f) {
        this->m_color = Color(1,0,1);
        this->m_material = Material();
}

Cone::Cone(const Vector3& center, float height, float radius, const Color& color, const Material& mat) : m_center(center), m_height(height), m_radius(radius) {
        this->m_color = color;
        this->m_material = mat;
}

float Cone::SDF(const Vector3& point) const {
        Vector3 p = point - m_center;
        Vector3 c = Vector3(0, m_height, 0);  
        float q = (p - c * std::max(0.0f, p.dot(c) / c.dot(c))).length(); 
        return std::max(-q, p.y);
}


