#ifndef MANDELBULB_H
#define MANDELBULB_H

#include "../utilities/vector3.h"
#include "object.h"
#include "../utilities/color.h"


class Mandelbulb : public Object {
public:
    Mandelbulb();
    Mandelbulb(const Vector3& center, float scale, int power, const Color& color, const Material& material);
    
    const Vector3& getCenter() const { return m_center; }
    float getScale() const { return m_scale; }
    int getPower() const { return m_power; }
    float SDF(const Vector3& point) const override;

private:
    Vector3 m_center;
    float m_scale;
    int m_power;
};

#endif // MANDELBULB_H
