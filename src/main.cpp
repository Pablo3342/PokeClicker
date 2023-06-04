#include "sdlinit/sdlinit.hpp"
#include "app/app.hpp"

#undef main

int main(int argc, char* argv[]) {
    sdlinit::initLibraries();
    
    App app;
    app.init();
    app.run();

    sdlinit::closeLibraries();

    return 0;
}