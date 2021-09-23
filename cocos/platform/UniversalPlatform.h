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

#include "platform/BasePlatform.h"
#include "platform/IEventDispatch.h"

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
    int32_t run(int argc, const char** argv) override;
    /**
     *@bref Implement the main logic of the base platform.
     */
    int32_t main(int argc, const char** argv) override;
    /**
     *@bref Implement the destruction of the base platform.
     */
    void destory() override;
    /**
     *@bref Get targe platform type.
     */
    OSType getOSType() const override;
    /**
     *@bref Set the event handling callback.
     */
    void setHandleEventCallback(HandleEventCallback cb) override;
    /**
     *@bref Set the event to handle callbacks by default.
     */
    void setHandleDefaultEventCallback(HandleEventCallback cb) override;
    /**
     *@bref Implement dispatch event interface.
     */
    void dispatchEvent(const OSEvent& ev) override;
    /**
     *@bref Implement dispatch touch event interface.
     */
    void dispatchTouchEvent(const OSEvent& ev) override;
    /**
     *@bref Implement handle default event interface.
     */
    void handleDefaultEvent(const OSEvent& ev) override;
    int  getSdkVersion() const override;
    void pollEvent() override;
    
    void runInPlatform(PlatformThreadCallback task, int32_t fps) override;
    int32_t getFps() override;
    void setFps(int32_t fps) override;
    

protected:
    PlatformThreadCallback _mainTask;
    
private:
    int32_t _fps;

    HandleEventCallback _handleEventCallback;
    HandleEventCallback _handleDefaultEventCallback;
};

} // namespace cc