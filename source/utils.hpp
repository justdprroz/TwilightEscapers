#pragma once
#include <random>

double getRandomDouble(double a, double b) 
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::uniform_real_distribution<double> distrib(a, b);
    return distrib(randomGenerator);
}


struct vec2f {
    vec2f() = default;
    vec2f(const float _x, const float _y) : x(_x), y(_y) {};
    vec2f(const float _x) : x(_x), y(_x) {};
    float x;
    float y;
    bool operator==(vec2f other) {
        return (this->x == other.x && this->y == other.y);
    }
    bool operator!=(vec2f other) {
        return (this->x != other.x || this->y != other.y);
    }
    void operator+=(vec2f other) {
        this->x += other.x;
        this->y += other.y;
    }
	bool operator>=(vec2f other) {
        return (this->x >= other.x && this->y >= other.y);
    }
	bool operator<=(vec2f other) {
        return (this->x <= other.x && this->y <= other.y);
    }
    vec2f operator-(vec2f other) {
        return vec2f(this->x - other.x, this->y - other.y);
    }
    vec2f operator+(vec2f other) {
        return vec2f(this->x + other.x, this->y + other.y);
    }
    vec2f operator*(float num) {
        return vec2f(this->x * num, this->y * num);
    }
    vec2f operator*(vec2f other) {
        return vec2f(this->x * other.x, this->y * other.y);
    }
};

struct vec2i {
    vec2i() = default;
    vec2i(const int _x, const int _y) : x(_x), y(_y) {};
    vec2i(const int _x) : x(_x), y(_x) {};
    int x;
    int y;
    bool operator==(vec2i other) {
        return (this->x == other.x && this->y == other.y);
    }
    bool operator!=(vec2i other) {
        return (this->x != other.x || this->y != other.y);
    }
    void operator+=(vec2i other) {
        this->x += other.x;
        this->y += other.y;
    }
	bool operator>=(vec2i other) {
        return (this->x >= other.x && this->y >= other.y);
    }
	bool operator<=(vec2i other) {
        return (this->x <= other.x && this->y <= other.y);
    }
    vec2i operator-(vec2i other) {
        return vec2i(this->x - other.x, this->y - other.y);
    }
    vec2i operator+(vec2i other) {
        return vec2i(this->x + other.x, this->y + other.y);
    }
    vec2i operator*(float num) {
        return vec2i(this->x * num, this->y * num);
    }
    vec2i operator*(vec2i other) {
        return vec2i(this->x * other.x, this->y * other.y);
    }
};

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T>
bool inRange(T ll, T rl, T x){
   return ((rl - x) * (x - ll) >= 0);
}

bool inRange(vec2f ll, vec2f rl, vec2f pos){
   return ((rl - pos) * (pos - ll) >= vec2f(0,0));
}