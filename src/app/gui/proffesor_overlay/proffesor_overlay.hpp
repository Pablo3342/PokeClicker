#pragma once

#include "../../app_manager/app_manager.hpp"
#include "../../../config/config.hpp"
#include "../../../struct/struct.hpp"

using namespace config;

class ProffesorOverlay {
public:
    ProffesorOverlay(AppManager* t_appManager);
    ~ProffesorOverlay();

    std::vector<std::string> text;

private:
    AppManager* appManager;

    void init();
};