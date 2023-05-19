#ifndef MANDELBULB_H
#define MANDELBULB_H

#include "object.h"

class Mandelbulb : public Object {
public:
   Mandelbulb(const Vector3& center, int power, const Color& color, const Material& mat, float bailout)
    : m_center(center), m_power(power), m_color(color), m_material(mat),m_bailout(bailout) {}


    const Vector3& getCenter() const { return m_center; }
    int getPower() const { return m_power; }
    Color getColor() const {return m_color; }
    Material getMaterial() const {return m_material; }
    float getBailout() const { return m_bailout; }

private:
    Vector3 m_center;
    int m_power;
    Color m_color;
    Material m_material;
    float m_bailout;
};

#endif // MANDELBULB_H
