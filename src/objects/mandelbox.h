#ifndef MANDELBOX_H
#define MANDELBOX_H

#include "../utilities/vector3.h"
#include "object.h"
#include "../utilities/color.h"

class Mandelbox : public Object {
public:
    Mandelbox();
    Mandelbox(const Vector3& center, float scale, const Color& color, const Material& material);
    
    const Vector3& getCenter() const { return center_; }
    float getScale() const { return scale_; }
    Color getColor() const { return color_; }
    Material getMaterial() const {return material_; }
    Vector3 boxFold(const Vector3& v) const;
    Vector3 sphereFold(const Vector3& v) const;

private:
    Vector3 center_;
    float scale_;
    Color color_;
    Material material_;
};

#endif // MANDELBOX_H
