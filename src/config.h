// config.h
#ifndef CONFIG_H
#define CONFIG_H

constexpr int MAX_MARCHING_STEPS = 400;
constexpr float MIN_HIT_DISTANCE = 1e-4f;
constexpr float MAX_DISTANCE = 1e14f;
constexpr float EPSILON = 1e-6f;
constexpr int SAMPLES_PER_PIXEL = 9;
constexpr float AMBIENT_LIGHT_INTENSITY = 0.2f;
constexpr int NUM_SHADOW_RAYS = 1;
constexpr float SHADOW_THRESHOLD = 0.1f;
constexpr float SHADOW_BIAS = 1e-3f;
constexpr float REFLECT_BIAS = 1e-3f;
constexpr int MAX_REFLECTION_BOUNCES = 8;
constexpr bool NIGHT = false;

#endif // CONFIG_H
