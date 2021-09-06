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

#include "platformex/BasePlatform.h"
#include "platformex/IEventDispathch.h"

namespace cc {
class UniversalPlatform : public BasePlatform, public IEventDispatch {
public:
    /**
     *@bref Implement basic platform initialization.
     */
    int32_t init() override;
    /**
     *@bref Start base platform initialization.
     */
    int32_t run(int argc, char** argv) override;
    /**
     *@bref Implement the main logic of the base platform.
     */
    int32_t main(int argc, char** argv) override;
    /**
     *@bref Implement the destruction of the base platform.
     */
    void destory() override;
    /**
     *@bref Get targe platform type.
     */
    OSType getOSType() override;
    /**
     *@bref Set the event handling callback.
     */
    void setEventHandleCallback(EventHandleCallback cb) override;
    /**
     *@bref Set the event to handle callbacks by default.
     */
    void setDefaultEventHandleCallback(EventHandleCallback cb) override;
    /**
     *@bref Implement dispatch event interface.
     */
    void dispatchEvent(OSEventType type, const OSEvent& ev) override;
    /**
     *@bref Implement dispatch event interface.
     */
    void dispatchTouchEvent(const OSEvent& ev) override;
    /**
     *@bref Implement dispatch event interface.
     */
    void defaultEventHandle(OSEventType type, const OSEvent& ev) override;

private:
    EventHandleCallback _eventHandleCallback;
    EventHandleCallback _eventDefaultHandleCallback;
};

} // namespace cc