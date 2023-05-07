#ifndef RAY_H
#define RAY_H

#include "vector3.h"

class Ray {
public:
    Ray();
    Ray(const Vector3& origin, const Vector3& direction);

    const Vector3& getOrigin() const;
    const Vector3& getDirection() const;

    Vector3 pointAtParameter(float t) const;

private:
    Vector3 m_origin;
    Vector3 m_direction;
};

#endif // RAY_H
