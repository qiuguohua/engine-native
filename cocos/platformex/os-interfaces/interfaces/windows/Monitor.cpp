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

#include "platformex/os-interfaces/interfaces/windows/Monitor.h"
#include "base/Macros.h"

#include <Windows.h>

namespace cc {

int Monitor::getDPI() {
    static int dpi = -1;
    if (dpi == -1) {
        HDC hScreenDC = GetDC(nullptr);
        int PixelsX   = GetDeviceCaps(hScreenDC, HORZRES);
        int MMX       = GetDeviceCaps(hScreenDC, HORZSIZE);
        ReleaseDC(nullptr, hScreenDC);
        dpi = static_cast<int>(254.0f * PixelsX / MMX / 10);
    }
    return dpi;
}

float Monitor::getDevicePixelRatio() {
    return 1;
}

void Monitor::setKeepScreenOn(bool value) {
    CC_UNUSED_PARAM(value);
}

Monitor::Orientation Monitor::getDeviceOrientation() {
    return Orientation::LANDSCAPE_RIGHT;
}

Vec4 Monitor::getSafeAreaEdge() {
    return cc::Vec4();
}

}