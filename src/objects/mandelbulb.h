#ifndef MANDELBULB_H
#define MANDELBULB_H

#include "../utilities/vector3.h"
#include "object.h"
#include "../utilities/color.h"


class Mandelbulb : public Object {
public:
    Mandelbulb();
    Mandelbulb(const Vector3& center, float scale, int power, const Color& color, const Material& material);
    
    const Vector3& getCenter() const { return center_; }
    float getScale() const { return scale_; }
    int getPower() const { return power_; }
    Color getColor() const { return color_; }
    Material getMaterial() const {return material_; }

private:
    Vector3 center_;
    float scale_;
    int power_;
    Color color_;
    Material material_;
};

#endif // MANDELBULB_H
