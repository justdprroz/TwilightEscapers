#ifndef UTILS_HPP
#define UTILS_HPP

template <typename T>
T LinearInterpolation(T x, T minInputRange, T maxInputRange, T minOutputRange, T maxOutputRange) {
    return (x - minInputRange) / (maxInputRange - minInputRange) * (maxOutputRange - minOutputRange) + minOutputRange;
}

template <typename T>
bool InRange(T x, T minRange, T maxRange) {
    return (minRange <= x && x < maxRange);
}

#endif