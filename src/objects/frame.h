#ifndef FRAME_H
#define FRAME_H

#include "object.h"
#include "cube.h"

class Frame : public Object {
public:
    
    Frame(const Cube& cube);
    const Cube& getCube() const {return m_cube;}
    float SDF(const Vector3& point) const override;
    void getUV(const Vector3& p, float& u, float& v) const override {std::cout << p.x << u << v << std::endl;}

private:
    Cube m_cube;
};

#endif // FRAME_H
