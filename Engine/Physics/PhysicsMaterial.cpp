#include "PhysicsMaterial.h"

namespace Good
{

    PhysicsMaterial::PhysicsMaterial(float32 f, float32 b, float32 d)
        : friction(f), bounciness(b), density(d) {}

    PhysicsMaterial PhysicsMaterial::Default() { return {0.5f,  0.0f, 1.0f}; }
    PhysicsMaterial PhysicsMaterial::Ice()     { return {0.05f, 0.0f, 0.9f}; }
    PhysicsMaterial PhysicsMaterial::Rubber()  { return {1.0f,  0.8f, 1.3f}; }
    PhysicsMaterial PhysicsMaterial::Metal()   { return {0.3f,  0.1f, 7.8f}; }
    PhysicsMaterial PhysicsMaterial::Wood()    { return {0.6f,  0.2f, 0.6f}; }

} // namespace Good