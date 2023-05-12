#include "sphere.h"

Sphere::Sphere() : m_center(Vector3(0.0f, 0.0f, 0.0f)), m_radius(1.0f), m_color(Color(1.0f, 1.0f, 1.0f)), m_material(Material(0.5f, 0.5f,0.0f)) {}

Sphere::Sphere(const Vector3& center, float radius, const Color& color, const Material& mat) : m_center(center), m_radius(radius), m_color(color), m_material(mat) {}


