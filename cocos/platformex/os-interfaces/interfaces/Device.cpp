/****************************************************************************
 Copyright (c) 2017-2021 Xiamen Yaji Software Co., Ltd.

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
#include "platformex/os-interfaces/interfaces/Device.h"

#include "engine/EngineManager.h"
#include "platformex/os-interfaces/interfaces/AccelerometerInterface.h"
#include "platformex/os-interfaces/interfaces/BatteryInterface.h"
#include "platformex/os-interfaces/interfaces/MonitorInterface.h"
#include "platformex/os-interfaces/interfaces/NetworkInterface.h"
#include "platformex/os-interfaces/interfaces/VibrateInterface.h"

namespace cc {

int Device::getDPI() {
    return GET_PLATFORM_INTERFACE(MonitorInterface)->getDPI();
}

float Device::getDevicePixelRatio() {
    return GET_PLATFORM_INTERFACE(MonitorInterface)->getDevicePixelRatio();
}

void Device::setKeepScreenOn(bool keepScreenOn) {
    return GET_PLATFORM_INTERFACE(MonitorInterface)->setKeepScreenOn(keepScreenOn);
}

void Device::setAccelerometerEnabled(bool isEnabled) {
    return GET_PLATFORM_INTERFACE(AccelerometerInterface)->setAccelerometerEnabled(isEnabled);
}

void Device::setAccelerometerInterval(float interval) {
    return GET_PLATFORM_INTERFACE(AccelerometerInterface)->setAccelerometerInterval(interval);
}

const AccelerometerInterface::MotionValue &Device::getDeviceMotionValue() {
    return GET_PLATFORM_INTERFACE(AccelerometerInterface)->getDeviceMotionValue();
}

MonitorInterface::Orientation Device::getDeviceOrientation() {
    return GET_PLATFORM_INTERFACE(MonitorInterface)->getDeviceOrientation();
}

std::string Device::getDeviceModel() {
    return GET_PLATFORM_INTERFACE(SystemInterface)->getDeviceModel();
}

void Device::vibrate(float duration) {
    return GET_PLATFORM_INTERFACE(VibrateInterface)->vibrate(duration);
}

float Device::getBatteryLevel() {
    return GET_PLATFORM_INTERFACE(BatteryInterface)->getBatteryLevel();
}

NetworkInterface::NetworkType Device::getNetworkType() {
    return GET_PLATFORM_INTERFACE(NetworkInterface)->getNetworkType();
}

Vec4 Device::getSafeAreaEdge() {
    return GET_PLATFORM_INTERFACE(MonitorInterface)->getSafeAreaEdge();
}

} // namespace cc