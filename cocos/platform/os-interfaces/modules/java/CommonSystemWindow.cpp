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

#include "platform/os-interfaces/modules/java/CommonSystemWindow.h"
#include <fcntl.h>
#include <jni.h>
#include <unistd.h>
#include <functional>
#include <thread>
#include <vector>
#include "base/Log.h"
#include "base/Macros.h"

#include "platform/java/jni/JniImp.h"

namespace {

} // namespace

namespace cc {
CommonSystemWindow::CommonSystemWindow(IEventDispatch *platform)
: ISystemWindow(platform) {
}

CommonSystemWindow::~CommonSystemWindow() = default;

void CommonSystemWindow::pollEvent() {
}

bool CommonSystemWindow::createWindow(const char *title,
                                    int x, int y, int w,
                                    int h, int flags) {
    CC_UNUSED_PARAM(title);
    CC_UNUSED_PARAM(x);
    CC_UNUSED_PARAM(y);
    CC_UNUSED_PARAM(w);
    CC_UNUSED_PARAM(h);
    CC_UNUSED_PARAM(flags);
    return true;
}

void CommonSystemWindow::setCursorEnabled(bool value) {
}

void CommonSystemWindow::copyTextToClipboard(const std::string &text) {
    copyTextToClipboardJNI(text);
}

std::array<int, 2> CommonSystemWindow::getViewSize() const {
    return std::array<int, 2>{_width, _height};
}

void CommonSystemWindow::setHeight(int32_t height) {
    _height = height;
}

void CommonSystemWindow::setWidth(int32_t width) {
    _width = width;
}

} // namespace cc