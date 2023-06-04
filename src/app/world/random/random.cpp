#include "random.hpp"

Random::Random()
    : defaultDist(0, 1)
{
    gen = initGen();
}

Random::~Random()
{}

int Random::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

int Random::getBinomial(int n, float p) {
    std::binomial_distribution<int> dist(n, p);
    return dist(gen);
}

float Random::getDefault() {
    return defaultDist(gen);
}

std::mt19937 Random::initGen() {
    std::random_device rd;
    uint32_t seed = rd() ^ std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 t_randGen(seed);

    return t_randGen;
}