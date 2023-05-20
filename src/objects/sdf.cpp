#include "sdf.h"
#include <cmath>


namespace SDF {
    float sphereSDF(const Vector3& point, const Sphere& sphere) {
        return (point - sphere.getCenter()).length() - sphere.getRadius();
    }

    float planeSDF(const Vector3& point, const Plane& plane) {
        return point.dot(plane.getNormal()) - plane.getDistance();
    }

    float cubeSDF(const Vector3& point, const Cube& cube) {
        Vector3 p = point - cube.getCenter();
        Vector3 d = p.abs() - cube.getDimensions() * 0.5f;
        return d.max(0.0f).length() + std::min(std::max(d.x, std::max(d.y, d.z)), 0.0f);
    }

    float torusSDF(const Vector3& point, const Torus& torus) {
        Vector3 p = point - torus.getCenter();
        Vector3 q = Vector3((Vector3(p.x, 0.0f, p.z).length() - torus.getMajorRadius()), p.y, 0.0f);
        return q.length() - torus.getMinorRadius();
    }

    float coneSDF(const Vector3& point, const Cone& cone) {
        Vector3 p = point - cone.getCenter();
        Vector3 c = Vector3(0, cone.getHeight(), 0);  
        float q = (p - c * std::max(0.0f, p.dot(c) / c.dot(c))).length(); 
        return std::max(-q, p.y);
    }

    float cubeWithHoleSDF(const Vector3& point, const CubeWithHole& cubeWithHole) {
        const Cube& cube = cubeWithHole.getCube();
        const Sphere& sphere = cubeWithHole.getSphere();

        float cubeSdf = cubeSDF(point, cube);
        float sphereSdf = sphereSDF(point, sphere);

        return std::max(cubeSdf, -sphereSdf);
    }

    

    float frameSDF(const Vector3& point, const Frame& frame) {
        float FRAME_THICKNESS = 3;
        const Cube& cube = frame.getCube();
        Vector3 dimensions = cube.getDimensions();
        float dim = dimensions.x;
        Vector3 innerCenter = cube.getCenter();
        

        // Outer cube
        float outerSDF = cubeSDF(point, cube);

        std::vector<Vector3> rectangleDimensions = {
            Vector3(dim * 2, FRAME_THICKNESS + 0.001 , FRAME_THICKNESS + 0.001), // Along y-axis
            Vector3(FRAME_THICKNESS + 0.001, dim * 2, FRAME_THICKNESS + 0.001), // Along z-axis
            Vector3(FRAME_THICKNESS + 0.001, FRAME_THICKNESS + 0.001, dim * 2)  // Along x-axis
        };

        std::vector<float> SDFs = {outerSDF};

        for (int i = 0; i < 3; ++i) {
            Vector3 innerCubeCenter = innerCenter;
            Cube innerCube(innerCubeCenter, rectangleDimensions[i], cube.getColor(), cube.getMaterial());
            float innerSDF = cubeSDF(point, innerCube);

            // Subtract from outer SDF
            SDFs.push_back(-innerSDF);
        }

        // Take the maximum
        float result = *std::max_element(SDFs.begin(), SDFs.end());

        return result;
    }

    float mandelbulbDE(const Vector3& point, const Mandelbulb& mandelbulb) {
        Vector3 z = (point - mandelbulb.getCenter()) / mandelbulb.getScale();
        Vector3 c = z;
        float dr = 1.0;
        float r = 0.0;
        for (int i = 0; i < 1000 ; i++) {
            r = z.length();
            if (r>2) break;
        
            // convert to polar coordinates
            float theta = acos(z.z/r);
            float phi = atan2(z.y,z.x);
            dr =  pow( r, mandelbulb.getPower()-1.0)*mandelbulb.getPower()*dr + 1.0;
            
            // scale and rotate the point
            float zr = pow( r,mandelbulb.getPower());
            theta = theta*mandelbulb.getPower();
            phi = phi*mandelbulb.getPower();
            
            // convert back to cartesian coordinates
            z = zr*Vector3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
            z+=c;
        }
        return 0.5*log(r)*r/dr;
    }

    float mengerSpongeDE(const Vector3& point, const MengerSponge& mengerSponge) {
        Vector3 z = (point - mengerSponge.getCenter()) / mengerSponge.getScale();
        float scale = mengerSponge.getScale();
        int iterations = mengerSponge.getIterations();

        for (int i = 0; i < iterations; ++i) {
            z.x = fabs(z.x);
            z.y = fabs(z.y);
            z.z = fabs(z.z);

            if (z.x - z.y < 0) std::swap(z.x, z.y);
            if (z.x - z.z < 0) std::swap(z.x, z.z);
            if (z.y - z.z < 0) std::swap(z.y, z.z);

            z = scale*z - Vector3(scale-1, scale-1, 2*scale-2);

            if (z.z > -0.5*scale) {
                z.z -= scale;
            }
            else if (z.z < -1.5*scale) {
                z.z += 2*scale;
            }
        }

        return (z.length() - 2) / pow(scale, iterations);
    }

    float mandelboxDE(const Vector3& point, const Mandelbox& mandelbox) {
        Vector3 z = (point - mandelbox.getCenter()) / mandelbox.getScale();
        Vector3 c = z;
        float dr = 1.0;

        for (int i = 0; i < 100; i++) {
            z = mandelbox.boxFold(z);
            z = mandelbox.sphereFold(z);

            z = z * mandelbox.getScale() + c;
            dr = dr * abs(mandelbox.getScale()) + 1.0;

            if (z.length() > 2.0) break;
        }

        return 0.5 * log(z.length()) * z.length() / dr;
    }

}