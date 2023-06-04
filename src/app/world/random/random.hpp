#pragma once

/// BASIC
#include <random>
#include <chrono>

class Random {
public:
    Random();
    ~Random();

    std::mt19937 gen;

    // uniform distribution
    int getInt(int min, int max);
    template<typename Type> Type getReal(Type min, Type max);
    template<typename Type> Type getReal(Type max);
    float getDefault();

    // binomial distribution
    int getBinomial(int n, float p);

private:
    std::uniform_real_distribution<float> defaultDist;
    std::mt19937 initGen();
};

#include "random_impl.hpp"