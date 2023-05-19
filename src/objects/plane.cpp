#include "plane.h"

Plane::Plane() : m_normal(Vector3(0.0f, 1.0f, 0.0f)), m_distance(0.0f), m_color(Color(0.5f, 0.5f, 0.5f)) {}

Plane::Plane(const Vector3& normal, float distance, const Color& color) : m_normal(normal.normalized()), m_distance(distance), m_color(color) {}


