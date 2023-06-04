#pragma once

template<typename Type>
Type Random::getReal(Type min, Type max) {
    std::uniform_real_distribution<Type> dist(min, max);
    return dist(gen);
}

template<typename Type>
Type Random::getReal(Type max) {
    return defaultDist(gen) * max;
}