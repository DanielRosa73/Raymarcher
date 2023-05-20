#include "menger_sponge.h"

MengerSponge::MengerSponge()
    : center_(0, 0, 0), scale_(1), iterations_(0), color_(0, 0, 0), material_(Material()) {}

MengerSponge::MengerSponge(const Vector3& center, float scale, int iterations, const Color& color, const Material& material)
    : center_(center), scale_(scale), iterations_(iterations), color_(color), material_(material) {}
