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

#pragma once 

#include "platformex/os-interfaces/OSAbstractInterface.h"
#include "base/Data.h"
#include "base/Macros.h"
#include "math/Vec4.h"

namespace cc {

class DeviceInterface : public OSAbstractInterface {
public:
    virtual ~DeviceInterface() = default;

    struct MotionValue {
        float accelerationX = 0.0F;
        float accelerationY = 0.0F;
        float accelerationZ = 0.0F;

        float accelerationIncludingGravityX = 0.0F;
        float accelerationIncludingGravityY = 0.0F;
        float accelerationIncludingGravityZ = 0.0F;

        float rotationRateAlpha = 0.0F;
        float rotationRateBeta  = 0.0F;
        float rotationRateGamma = 0.0F;
    };
    /**
     * To enable or disable accelerometer.
     */
    static void setAccelerometerEnabled(bool isEnabled);

    /**
     *  Sets the interval of accelerometer.
     */
    static void setAccelerometerInterval(float interval);

    /**
     *  Gets the motion value of current device.
     */
    static const MotionValue &getDeviceMotionValue();

    /**
     *  Gets device model information.
     */
    static std::string getDeviceModel();
    /**
     * Vibrate for the specified amount of time.
     * If vibrate is not supported, then invoking this method has no effect.
     * Some platforms limit to a maximum duration of 5 seconds.
     * Duration is ignored on iOS due to API limitations.
     * @param duration The duration in seconds.
     */
    static void vibrate(float duration);

    /**
     * Gets battery level, only avaiable on iOS and Android.
     * @return 0.0 ~ 1.0
     */
    static float getBatteryLevel();
    enum class NetworkType {
        NONE,
        LAN,
        WWAN
    };
    static NetworkType getNetworkType();

    /*
     * Gets the SafeArea edge.
     * Vec4(x, y, z, w) means Edge(top, left, bottom, right)
     */
    static Vec4 getSafeAreaEdge();

private:
};

}