#ifndef CAMERA_H
#define CAMERA_H

#include "../utilities/vector3.h"
#include "ray.h"

class Camera {
public:
    Camera();
    Camera(const Vector3& position, const Vector3& target, const Vector3& up, float fov, float aspect_ratio);

    Ray getRay(float u, float v) const;

private:
    Vector3 m_position;
    Vector3 m_lower_left_corner;
    Vector3 m_horizontal;
    Vector3 m_vertical;
};

#endif // CAMERA_H
