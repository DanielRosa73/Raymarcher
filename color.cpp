#include "color.h"

Color::Color() : m_r(0.0f), m_g(0.0f), m_b(0.0f) {}

Color::Color(float r, float g, float b) : m_r(r), m_g(g), m_b(b) {}

float Color::r() const {
    return m_r;
}

float Color::g() const {
    return m_g;
}

float Color::b() const {
    return m_b;
}

Color Color::operator+(const Color& other) const {
    return Color(m_r + other.m_r, m_g + other.m_g, m_b + other.m_b);
}

Color Color::operator*(const Color& other) const {
    return Color(m_r * other.m_r, m_g * other.m_g, m_b * other.m_b);
}

Color Color::operator*(float scalar) const {
    return Color(m_r * scalar, m_g * scalar, m_b * scalar);
}

Color& Color::operator+=(const Color& other) {
    m_r += other.m_r;
    m_g += other.m_g;
    m_b += other.m_b;
    return *this;
}

Color Color::operator/(float scalar) const {
    float new_r = m_r / scalar;
    float new_g = m_g / scalar;
    float new_b = m_b / scalar;
    return Color(new_r,new_g,new_b);
}

Color& Color::operator*=(const Color& other) {
    m_r *= other.m_r;
    m_g *= other.m_g;
    m_b *= other.m_b;
    return *this;
}

float Color::length() const {
    return std::sqrt(m_r * m_r + m_g * m_g + m_b * m_b);
}
