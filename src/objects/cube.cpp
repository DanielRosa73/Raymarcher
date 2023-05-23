#include "cube.h"


Cube::Cube(const Vector3& center, const Vector3& dimensions, const Color& color, const Material& mat)
    : m_center(center), m_dimensions(dimensions) {
        this->m_color = color;
        this->m_material = mat;
    }


float Cube::SDF(const Vector3& point) const{
        Vector3 p = point - m_center;
        Vector3 d = p.abs() - m_dimensions * 0.5f;
        return d.max(0.0f).length() + std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);
}
