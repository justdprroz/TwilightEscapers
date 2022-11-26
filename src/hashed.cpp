#include <cmath>
#include <iostream>
#include <algorithm>
#include <external/noise.hpp>
#include <fstream>

#define LL long long

LL pair(LL a, LL b, LL C1, LL C2, LL C3, LL C4, LL C5)
{
    return (a * C1 + b * C2 + a * b * C3 + (a + b) * 0) % C5;
}

int main()
{
    int width = 1 << 13;
    int height = 1 << 13;

    LL C1, C2, C3, C4, C5;
    std::cin >> C1 >> C2 >> C3 >> C4 >> C5;

    std::ofstream file;
    file.open ("example.txt");

    file << "P3\n";
    file << width << " " << height << "\n";
    file << "255\n";

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int c = pair(x, y, C1, C2, C3, C4, C5) % 16777216;
            int b = c % 256;
            int g = (c >> 8) % 256;
            int r = (c >> 16) % 256;
            file << r << " " << g << " " << b << "\n";
            // std::cout <<  << " " << (c >> 8) % 256 << " " << (c >> 8) % 256 << "\n";
        }
    }
    file.close();
}