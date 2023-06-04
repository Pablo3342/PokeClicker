#pragma once

template<size_t WIDTH, size_t HEIGHT>
Level<WIDTH, HEIGHT>::Level(std::string t_filepath)
	:	filepath(t_filepath)
{
    init();
}

template<size_t WIDTH, size_t HEIGHT>
Level<WIDTH, HEIGHT>::~Level()
{}

template<size_t WIDTH, size_t HEIGHT>
void Level<WIDTH, HEIGHT>::init() {
    std::fstream file;
    file.open(filepath.c_str(), std::ios::in | std::ios::app);

    if (!file.good()) {
        printf("Error, couldn't open file\n");
    }

    char input = '\0';
    std::string inputString;
    int w = 0, h = 0;

    while (!file.eof()) {
        file.read(&input, 1);

        if (file.eof()) {
            break;
        }

        if (input != ',' && input != '\n') {
            inputString += input;
        }
        else {
            map[h][w] = std::stoi(inputString);
            //std::cout << map[h][w] << ":(" << h << ", " << w << ")";
            inputString = std::string();
            ++w;
        }

        if (input == '\n') {
            w = 0;
            ++h;
        }
    }

    file.close();
}