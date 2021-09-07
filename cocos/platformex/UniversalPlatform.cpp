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

#include "platformex/UniversalPlatform.h"

#include "platformex/os-interfaces/OSInterface.h"

#include "platformex/os-interfaces/modules/IAccelerometer.h"
#include "platformex/os-interfaces/modules/IBattery.h"
#include "platformex/os-interfaces/modules/INetwork.h"
#include "platformex/os-interfaces/modules/IScreen.h"
#include "platformex/os-interfaces/modules/ISystem.h"
#include "platformex/os-interfaces/modules/ISystemWindow.h"
#include "platformex/os-interfaces/modules/IVibrate.h"

extern int cocos_main(int argc, char** argv);

namespace cc {
UniversalPlatform::OSType UniversalPlatform::getOSType() {
    return getOSInterface<ISystem>()->getOSType();
}

void UniversalPlatform::dispatchEvent(OSEventType type, const OSEvent& ev) {
    bool isHandled = false;
    if (_eventHandleCallback) {
        isHandled = (_eventHandleCallback)(type, ev);
    }
    if (isHandled) {
        return;
    }
    if (_eventDefaultHandleCallback) {
        isHandled = (_eventDefaultHandleCallback)(type, ev);
    }
    if (!isHandled)
        defaultEventHandle(type, ev);
}

void UniversalPlatform::dispatchTouchEvent(const OSEvent& ev) {
}

void UniversalPlatform::defaultEventHandle(OSEventType type, const OSEvent& ev) {
    // To do :
}

void UniversalPlatform::setEventHandleCallback(EventHandleCallback cb) {
    _eventHandleCallback = cb;
}

void UniversalPlatform::setDefaultEventHandleCallback(EventHandleCallback cb) {
    _eventDefaultHandleCallback = cb;
}

int32_t UniversalPlatform::init() {
    registerOSInterface(ISystemWindow::createSystemWindowInterface(this));
    registerOSInterface(ISystem::createSystemInterface());
    registerOSInterface(INetwork::createNetworkInterface());
    registerOSInterface(IScreen::createScreenInterface());
    registerOSInterface(IBattery::createBatteryInterface());
    registerOSInterface(IVibrate::createVibrateInterface());
    registerOSInterface(IAccelerometer::createAccelerometerInterface());
    //registerOSInterface(ICanvasRenderingContext2D::getInterface());
    //OSAbstractInterface::OSAbstractInterfaces intfs =
    //    OSAbstractInterface::getSystemSupportInterfaces(this);
    //for (auto it : intfs) {
    //    registerOSInterface(it);
    //}
    return 0;
}

int32_t UniversalPlatform::main(int argc, char** argv) {
    return cocos_main(0, nullptr);
}

int32_t UniversalPlatform::run(int argc, char** argv) {
    return main(argc, argv);
}

void UniversalPlatform::destory() {
}

} // namespace cc