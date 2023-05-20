#ifndef SDF_H
#define SDF_H

#include "../utilities/vector3.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "torus.h"
#include "cone.h"
#include "cubewithhole.h"
#include "mandelbulb.h"
#include "frame.h"
#include "menger_sponge.h"
#include "mandelbox.h"
#include <vector>
#include <algorithm>

namespace SDF {

    float sphereSDF(const Vector3& point, const Sphere& sphere);
    float planeSDF(const Vector3& point, const Plane& plane);
    float cubeSDF(const Vector3& point, const Cube& cube);
    float torusSDF(const Vector3& point, const Torus& torus);
    float coneSDF(const Vector3& point, const Cone& cone);
    float cubeWithHoleSDF(const Vector3& point, const CubeWithHole& cubeWithHole);
    float mandelbulbDE(const Vector3& point, const Mandelbulb& mandelbulb);
    float frameSDF(const Vector3& point, const Frame& frame);
    float mandelbulbDE(const Vector3& point, const Mandelbulb& mandelbulb);
    float mengerSpongeDE(const Vector3& point, const MengerSponge& mengerSponge);
    float mandelboxDE(const Vector3& point, const Mandelbox& mandelbox);
};

#endif // SDF_H
