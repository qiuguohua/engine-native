#include <iostream>

#include "platform/BasePlatform.h"

int main(int argc, const char * argv[]) {
    cc::BasePlatform* platform = cc::BasePlatform::getPlatform();
    if (platform->init()) {
        CC_LOG_FATAL("Platform initialization failed");
        return -1;
    }
    return platform->run(argc, argv);
}
