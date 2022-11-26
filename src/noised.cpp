#include <cmath>
#include <iostream>
#include <algorithm>
#include <external/noise.hpp>

int main()
{
    int width = 1024;
    int height = 1024;
    std::cout << "P3\n";
    std::cout << width << " " << height << "\n";
    std::cout << "255\n";

    PerlinNoise noisegen(69);

    float apm, freq, oct;
    std::cin >> apm >> freq >> oct;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            double noise = noisegen.EvaluateFBM(x, y, 0, apm, freq, oct);
            // std::cout << x << " " << y << " " << noise << "\n";
            int c = 255 * noise;
            c = c / 4 * 4;
            std::cout << c << " " << c << " " << c << "\n";
        }
    }
}