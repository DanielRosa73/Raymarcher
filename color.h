#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include <cmath>

class Color {
public:
    Color();
    Color(float r, float g, float b);

    float r() const;
    float g() const;
    float b() const;

    Color operator+(const Color& other) const;
    Color operator*(const Color& other) const;
    Color operator*(float scalar) const;
    Color& operator+=(const Color& other);
    Color& operator*=(const Color& other);
    Color operator/(float scalar) const;

    float length() const;

    static Color lerp(const Color& a, const Color& b, float t) {
        return (a * (1.0f - t)) + (b * t);
    }

private:
    float m_r;
    float m_g;
    float m_b;
};

#endif // COLOR_H
