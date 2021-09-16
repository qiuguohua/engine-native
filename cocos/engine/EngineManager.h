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

#include "base/Macros.h"
#include "engine/BaseEngine.h"

namespace cc {

class EngineManager {
public:
    /*
     @bref Get engine manager instance.
     */
    static EngineManager* getInstance();
    /*
     @bref Get engine manager instance.
     @return current running engine
     */
    std::shared_ptr<BaseEngine> getCurrentEngine();
    /*
     @bref Sets the currently running engine.
     @engine current running engine
     */
    void setCurrentEngine(const BaseEngine::Ptr& engine);

private:
    EngineManager();

    std::weak_ptr<BaseEngine> _currentEngine;
    DISABLE_COPY_AND_MOVE(EngineManager);
};

} // namespace cc

#define CURRENT_ENGINE() cc::EngineManager::getInstance()->getCurrentEngine()

#define GET_PLATFORM_INTERFACE(intf) CURRENT_ENGINE()->getOSInterface<intf>()