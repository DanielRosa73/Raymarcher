#include "camera.h"

Camera::Camera() {   
    m_position = Vector3();
    m_lower_left_corner = Vector3();
    m_horizontal = Vector3();
    m_vertical = Vector3();
}

Camera::Camera(const Vector3& position, const Vector3& look_at, const Vector3& up, float fov, float aspect_ratio)
    : m_position(position) {
    float theta = fov * 3.14159265358979323846f / 180.0f;
    float half_height = std::tan(theta / 2.0f);
    float half_width = aspect_ratio * half_height;

    Vector3 w = (position - look_at).normalized();
    Vector3 u = up.cross(w).normalized();
    Vector3 v = w.cross(u);

    m_lower_left_corner = m_position - (u * half_width) - (v * half_height) - w;
    m_horizontal = u * (2.0f * half_width);
    m_vertical = v * (2.0f * half_height);
}

Ray Camera::getRay(float u, float v) const {
    Vector3 ray_direction = m_lower_left_corner + (m_horizontal * u) + (m_vertical * v) - m_position;
    return Ray(m_position, ray_direction);
}

