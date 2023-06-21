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

//This mapping applyes the same square to all 6 faces of the cube
void Cube::getUV(const Vector3& p, float& u, float& v) const {
    Vector3 pos = (p - m_center) / m_dimensions;
    float ax = std::abs(pos.x);
    float ay = std::abs(pos.y);
    float az = std::abs(pos.z);
    
    if (ax > ay && ax > az) {
       
        u = pos.z / ax;
        v = pos.y / ax;
    } else if (ay > ax && ay > az) {
      
        u = pos.x / ay;
        v = pos.z / ay;
    } else {
      
        u = pos.x / az;
        v = pos.y / az;
    }
    

    u = u / 2.0 + 0.5;
    v = v / 2.0 + 0.5;
}
