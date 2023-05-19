#ifndef FRAME_H
#define FRAME_H

#include "object.h"
#include "cube.h"

class Frame : public Object {
public:
    
    Frame(const Cube& cube);
    const Cube& getCube() const {return m_cube;}

private:
    Cube m_cube;
};

#endif // FRAME_H
