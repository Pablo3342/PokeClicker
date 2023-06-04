#include "proffesor_overlay.hpp"

ProffesorOverlay::ProffesorOverlay(AppManager* t_appManager)
    :   appManager(t_appManager)
{
    init();
}

ProffesorOverlay::~ProffesorOverlay()
{}

void ProffesorOverlay::init() {
    text.push_back("Hello, Trainer!");
    text.push_back("I'm Oak, Professor Oak.");
    text.push_back("I have an extraordinary challenge for you, but only the most outstanding Pokémon trainers can meet it.");
    text.push_back("The situation is as follows: you must embark on a journey through all the surrounding locations.");
    text.push_back("Your task is clear...");
    text.push_back("CATCH THEM ALL");
    text.push_back("I'm glad you've taken on this difficult mission.");
    text.push_back("Good luck!");
}