#ifndef MENGER_SPONGE_H
#define MENGER_SPONGE_H

#include "../utilities/vector3.h"
#include "object.h"
#include "../utilities/color.h"

class MengerSponge : public Object {
public:
    MengerSponge();
    MengerSponge(const Vector3& center, float scale, int iterations, const Color& color, const Material& material);
    
    const Vector3& getCenter() const { return center_; }
    float getScale() const { return scale_; }
    int getIterations() const { return iterations_; }
    Color getColor() const { return color_; }
    Material getMaterial() const {return material_; }

private:
    Vector3 center_;
    float scale_;
    int iterations_;
    Color color_;
    Material material_;
};

#endif // MENGER_SPONGE_H
