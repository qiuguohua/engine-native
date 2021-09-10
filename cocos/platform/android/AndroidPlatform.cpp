//
// Created by Administrator on 9/8/2021.
//

#include <android_native_app_glue.h>
#include <thread>

#include "platform/android/AndroidPlatform.h"
#include "platform/android/jni/JniInteraction.h"
#include "platform/os-interfaces/modules/android/SystemWindow.h"

namespace cc {
AndroidPlatform::AndroidPlatform() {
    _jniInteraction = JniInteraction::getInstance();
}

int AndroidPlatform::getSdkVersion() const {
    return _jniInteraction->getSdkVersion();
}

int32_t AndroidPlatform::run(int argc, char** argv) {
    std::thread mainLogicThread([this, argc, argv]() {
        checkWindowHandleInit();
        main(argc, argv);
    });
    mainLogicThread.detach();
    _jniInteraction->waitRunning();
    return 0;
}

void AndroidPlatform::checkWindowHandleInit() {
    _jniInteraction->setRunning(true);
    while (_jniInteraction->isRunning()) {
        pollEvent();
        ANativeWindow* wndHandle = _jniInteraction->getWindowHandle();
        if (wndHandle != nullptr) {
            getOSInterface<SystemWindow>()->setWindowHandle(wndHandle);
            break;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(100));
    }
    _jniInteraction->setEventDispatch(this);
}

void AndroidPlatform::pollEvent() {
    _jniInteraction->execCommand();
    if (!_jniInteraction->isPause()) {
        std::this_thread::yield();
    }
    _jniInteraction->flushTasksOnGameThread();
}

}; // namespace cc