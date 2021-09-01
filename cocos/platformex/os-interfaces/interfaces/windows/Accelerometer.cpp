#include "platformex/os-interfaces/interfaces/windows/Accelerometer.h"

namespace cc {
void Accelerometer::setAccelerometerEnabled(bool isEnabled) {

}

void Accelerometer::setAccelerometerInterval(float interval) {

}

const Accelerometer::MotionValue& Accelerometer::getDeviceMotionValue() {
    static MotionValue __motionValue;
    return __motionValue;
}

}
