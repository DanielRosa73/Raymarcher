#include "torus.h"

Torus::Torus(const Vector3& center, float major_radius, float minor_radius, const Color& color, const Material& mat)
    : m_center(center), m_major_radius(major_radius), m_minor_radius(minor_radius) {
        this->m_color = color;
        this->m_material = mat;
    }



float Torus::SDF(const Vector3& point) const {
        Vector3 p = point - m_center;
        Vector3 q = Vector3((Vector3(p.x, 0.0f, p.z).length() - m_major_radius), p.y, 0.0f);
        return q.length() - m_minor_radius;
}