#include <iostream>
#include "platformex/AbstractPlatform.h"
#include "sdl2/SDL_main.h"

int SDL_main(int argc, char** argv) {
    cc::AbstratctPlatform* platform = cc::AbstratctPlatform::GetPlatform();
    if (platform->init()) {
        CC_LOG_FATAL("Platform initialization failed");
        return -1;
    }
    return platform->start(argc, argv);
}