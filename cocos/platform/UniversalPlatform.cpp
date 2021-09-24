/****************************************************************************
 Copyright (c) 2018-2021 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#include "platform/UniversalPlatform.h"

#include "platform/os-interfaces/OSInterface.h"

#include "platform/os-interfaces/modules/IAccelerometer.h"
#include "platform/os-interfaces/modules/IBattery.h"
#include "platform/os-interfaces/modules/INetwork.h"
#include "platform/os-interfaces/modules/IScreen.h"
#include "platform/os-interfaces/modules/ISystem.h"
#include "platform/os-interfaces/modules/ISystemWindow.h"
#include "platform/os-interfaces/modules/IVibrate.h"

extern int  cocos_main(int argc, const char** argv);
extern void cocos_destory();

namespace cc {
UniversalPlatform::OSType UniversalPlatform::getOSType() const {
    return getOSInterface<ISystem>()->getOSType();
}

void UniversalPlatform::dispatchEvent(const OSEvent& ev) {
    bool isHandled = false;
    if (_handleEventCallback) {
        isHandled = (_handleEventCallback)(ev);
    }
    if (isHandled) {
        return;
    }
    if (_handleDefaultEventCallback) {
        isHandled = (_handleDefaultEventCallback)(ev);
    }
    if (!isHandled) {
        handleDefaultEvent(ev);
    }
}

void UniversalPlatform::dispatchTouchEvent(const OSEvent& ev) {
}

void UniversalPlatform::handleDefaultEvent(const OSEvent& ev) {
    //TODO
}

void UniversalPlatform::setHandleEventCallback(HandleEventCallback cb) {
    _handleEventCallback = cb;
}

void UniversalPlatform::setHandleDefaultEventCallback(HandleEventCallback cb) {
    _handleDefaultEventCallback = cb;
}

int32_t UniversalPlatform::init() {
    registerOSInterface(ISystemWindow::createSystemWindowInterface());
    registerOSInterface(ISystem::createSystemInterface());
    registerOSInterface(INetwork::createNetworkInterface());
    registerOSInterface(IScreen::createScreenInterface());
    registerOSInterface(IBattery::createBatteryInterface());
    registerOSInterface(IVibrate::createVibrateInterface());
    registerOSInterface(IAccelerometer::createAccelerometerInterface());
    return 0;
}

int32_t UniversalPlatform::run(int argc, const char** argv) {
    cocos_main(argc, argv);
    return loop();
}

void UniversalPlatform::destory() {
    cocos_destory();
}

int UniversalPlatform::getSdkVersion() const {
    return 0;
}

void UniversalPlatform::runInPlatformThread(ThreadCallback task, int32_t fps) {
    _mainTask = task;
}

int32_t UniversalPlatform::getFps() const {
    return _fps;
}

void UniversalPlatform::setFps(int32_t fps) {
    _fps = fps;
}

void UniversalPlatform::pollEvent() {
}

void UniversalPlatform::onPause() {
}

void UniversalPlatform::onResume() {
}

void UniversalPlatform::onClose() {
}

} // namespace cc