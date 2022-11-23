#ifndef SEQUOIA_ENGINE_SRC_EXTERNAL_NOISE_
#define SEQUOIA_ENGINE_SRC_EXTERNAL_NOISE_

#include <cstdint>
#include <cmath>
#include <random>
#include <limits>

class PerlinNoise
{
public:
    PerlinNoise(int seed)
    {
        std::mt19937 rng(seed);
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);
        for (int i = 0; i < 512; i++)
        {
            m_permutations[i] = dist(rng);
        }
    };
    ~PerlinNoise(){};

    double EvaluateFBM(double x, double y, 
                  double amplitude, double frequency,
                  int octaveCount, double persistence = 0.5, double lacunarity = 2.0) {

    double value = 0;

    for (int i = 0; i < octaveCount; i++) {
        value += amplitude * noise(x * frequency, y * frequency);
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    return value;
}

    double noise(const double x = 0.0, const double y = 0.0, const double z = 0.0) const
    {
        const auto unit_x = (int)std::floor(x) & 255,
                   unit_y = (int)std::floor(y) & 255,
                   unit_z = (int)std::floor(z) & 255;
        const auto sub_x = x - std::floor(x),
                   sub_y = y - std::floor(y),
                   sub_z = z - std::floor(z);
        const auto u = fade(sub_x),
                   v = fade(sub_y),
                   w = fade(sub_z);
        const auto a = m_permutations[unit_x] + unit_y,
                   aa = m_permutations[a] + unit_z,
                   ab = m_permutations[a + 1] + unit_z,
                   b = m_permutations[unit_x + 1] + unit_y,
                   ba = m_permutations[b] + unit_z,
                   bb = m_permutations[b + 1] + unit_z;
        return lerp(w,
                    lerp(
                        v,
                        lerp(
                            u,
                            grad(
                                m_permutations[aa],
                                sub_x,
                                sub_y,
                                sub_z),
                            grad(
                                m_permutations[ba],
                                sub_x - 1,
                                sub_y,
                                sub_z)),
                        lerp(
                            u,
                            grad(
                                m_permutations[ab],
                                sub_x,
                                sub_y - 1,
                                sub_z),
                            grad(
                                m_permutations[bb],
                                sub_x - 1,
                                sub_y - 1,
                                sub_z))),
                    lerp(
                        v,
                        lerp(
                            u,
                            grad(
                                m_permutations[aa + 1],
                                sub_x,
                                sub_y,
                                sub_z - 1),
                            grad(
                                m_permutations[ba + 1],
                                sub_x - 1,
                                sub_y,
                                sub_z - 1)),
                        lerp(
                            u,
                            grad(
                                m_permutations[ab + 1],
                                sub_x,
                                sub_y - 1,
                                sub_z - 1),
                            grad(
                                m_permutations[bb + 1],
                                sub_x - 1,
                                sub_y - 1,
                                sub_z - 1))));
    }

private:
    int m_permutations[512];
    double fade(const double t) const
    {
        return pow(t, 3) * (t * (t * 6 - 15) + 10);
    }
    double lerp(const double t, const double a, const double b) const
    {
        return a + t * (b - a);
    }
    double grad(const int hash, const double x, const double y, const double z) const
    {
        auto h = hash & 15;
        auto u = h < 8 ? x : y,
             v = h < 4 ? y : h == 12 || h == 14 ? x
                                                : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};

#endif /* SEQUOIA_ENGINE_SRC_EXTERNAL_NOISE_ */
