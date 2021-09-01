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

#include "platformex/BasePlatform.h"
#include "platformex/os-interfaces/OSAbstractInterface.h"
#include "platformex/os-interfaces/interfaces/BatteryInterface.h"
#include "platformex/os-interfaces/interfaces/CanvasRenderingContext2DInterface.h"
#include "platformex/os-interfaces/interfaces/MonitorInterface.h"
#include "platformex/os-interfaces/interfaces/NetworkInterface.h"
#include "platformex/os-interfaces/interfaces/SystemInterface.h"
#include "platformex/os-interfaces/interfaces/SystemWindowUI.h"
#include "platformex/os-interfaces/interfaces/SystemInterface.h"
#include "platformex/os-interfaces/interfaces/VibrateInterface.h"

extern int cocos_main(int argc, char** argv);

namespace cc {
BasePlatform::OSType BasePlatform::getOSType() {
    return getOSInterface<SystemInterface>()->getOSType();
}

void BasePlatform::dispatchEvent(OSEventType type, const OSEvent& ev) {
    bool is_handled = false;
    if (_eventHandleCallback) {
        is_handled = (_eventHandleCallback)(type, ev);
    }
    if (is_handled) {
        return;
    }
    if (_eventDefaultHandleCallback) {
        is_handled = (_eventDefaultHandleCallback)(type, ev);
    }
    if (!is_handled)
        defaultEventHandle(type, ev);
}

void BasePlatform::dispatchTouchEvent(const OSEvent& ev) {
}

void BasePlatform::defaultEventHandle(OSEventType type, const OSEvent& ev) {
    // To do :
}

void BasePlatform::setEventHandleCallback(EventHandleCallback cb) {
    _eventHandleCallback = cb;
}

void BasePlatform::setDefaultEventHandleCallback(EventHandleCallback cb) {
    _eventDefaultHandleCallback = cb;
}

int BasePlatform::init() {
    registerOSInterface(SystemWindowUI::GetSystemWindowUI(this));
    registerOSInterface(SystemInterface::getSystemInterface());
    registerOSInterface(NetworkInterface::getNetworkInterface());
    registerOSInterface(MonitorInterface::getMonitorInterface());
    registerOSInterface(BatteryInterface::getBatteryInterface());
    registerOSInterface(VibrateInterface::getVibrateInterface());
    registerOSInterface(CanvasRenderingContext2DInterface::getInterface());
    //OSAbstractInterface::OSAbstractInterfaces intfs =
    //    OSAbstractInterface::getSystemSupportInterfaces(this);
    //for (auto it : intfs) {
    //    registerOSInterface(it);
    //}
    return 0;
}

int BasePlatform::start(int argc, char** argv) {
    return run(argc, argv);
}

int BasePlatform::run(int argc, char** argv) {
    return cocos_main(0, nullptr);
}

void BasePlatform::destory() {
}

} // namespace cc