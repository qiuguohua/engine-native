/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
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

#include "base/Log.h"
#include "base/Macros.h"

#include "bindings/event/EventDispatcher.h"
#include "platform/os-interfaces/modules/ISystem.h"

#include <functional>
#include <vector>

namespace cc {

class OSInterface;

class BasePlatform {
public:
    /**
     @brief Destructor of AbstratctPlatform.
     */
    virtual ~BasePlatform();
    /**
     @brief Get default system platform.
     */
    static BasePlatform* getPlatform();
    /**
     @brief Initialize system platform.
     */
    virtual int32_t init() = 0;
    /**
     @brief Run system platform.
     */
    virtual int32_t run(int argc, const char** argv) = 0;
    /**
     @brief Main business logic.
     */
    virtual int32_t main(int argc, const char** argv) = 0;
    /**
     @brief Destory system platform.
     */
    virtual void destory() = 0;

    /**
     @brief Polling event.
     */
    virtual void pollEvent() = 0;

    /**
     @brief Get target system type.
     */
    using OSType = ISystem::OSType;

    virtual OSType getOSType() const = 0;

    /**
     @brief Set event handling callback function.
     */
    using HandleEventCallback = std::function<bool(const OSEvent&)>;

    virtual void setHandleEventCallback(HandleEventCallback cb) = 0;

    /**
     @brief Set default event handling callback function.
     */
    virtual void setHandleDefaultEventCallback(HandleEventCallback cb) = 0;
    /**
     @brief Default event handling.
     */
    virtual void handleDefaultEvent(const OSEvent& ev) = 0;
    /**
     @brief Get the SDK version for Android.Other systems also have sdk versions, 
            but they are not currently used.
     */
    virtual int getSdkVersion() const = 0;
   
    using ThreadCallback = std::function<void(void)>;
    virtual void runInPlatformThread(ThreadCallback callback, int32_t fps) = 0;
    virtual int32_t getFps() const = 0;
    virtual void setFps(int32_t fps) = 0;
    
    virtual void onPause() = 0;
    virtual void onResume() = 0;
    virtual void onClose() = 0;
    /**
     @brief Get target system interface.
     @ Non thread safe.
     */
    template <class T>
    std::enable_if_t<std::is_base_of<OSInterface, T>::value, T*>
    getOSInterface() const {
        for (auto& it : _osInterfaces) {
            T* intf = dynamic_cast<T*>(it.get());
            if (intf) {
                return intf;
            }
        }
        CC_ASSERT(false);
        return nullptr;
    }

    /**
     @brief Registration system interface.
     */
    bool registerOSInterface(const OSInterface::Ptr& osInterface) {
        CC_ASSERT(osInterface != nullptr);
        auto it = std::find(_osInterfaces.begin(), _osInterfaces.end(), osInterface);
        if (it != _osInterfaces.end()) {
            CC_LOG_WARNING("Duplicate registration interface");
            return false;
        }
        _osInterfaces.push_back(osInterface);
        return true;
    }
    /**
     @brief Unregistration system interface.
     */
    void unRegisterOSInterface(const OSInterface::Ptr& osInterface) {
        CC_ASSERT(osInterface != nullptr);
        auto it = std::find(_osInterfaces.begin(), _osInterfaces.end(), osInterface);
        if (it != _osInterfaces.end()) {
            CC_LOG_WARNING("Interface is not registrated");
            return;
        }
        _osInterfaces.erase(it);
    }

private:
    std::vector<OSInterface::Ptr> _osInterfaces;
};
} // namespace cc