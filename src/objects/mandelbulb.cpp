#include "mandelbulb.h"

Mandelbulb::Mandelbulb() 
    : center_(Vector3(0.0f, 0.0f, 0.0f)), scale_(1.0f), power_(8), color_(Color(1.0f, 1.0f, 1.0f)), material_(Material()) {}

Mandelbulb::Mandelbulb(const Vector3& center, float scale, int power, const Color& color, const Material& material) 
    : center_(center), scale_(scale), power_(power), color_(color), material_(material) {}
