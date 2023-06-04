#pragma once

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

template<size_t WIDTH, size_t HEIGHT>
class Level {
public:
    Level(std::string t_filepath);
    ~Level();

    int map[HEIGHT][WIDTH];

private:
    std::string filepath;

    void init();
};

#include "level_impl.hpp"