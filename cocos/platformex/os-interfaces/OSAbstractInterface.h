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
#include "platformex/EventDispathchInterface.h"
#include <vector>

namespace cc {

class OSAbstractInterface {
public:
    using Ptr = std::shared_ptr<OSAbstractInterface>;
    /**
     @brief Constructor of OSAbstractInterface.
     */
    OSAbstractInterface();
    /**
     @brief Constructor of OSAbstractInterface.
     */
    OSAbstractInterface(EventDispatchInterface*);
    /**
     @brief Destructor of OSAbstractInterface.
     */
    virtual ~OSAbstractInterface() = default;

    /**
     @brief Dispatch event.
     @param type : event type
     @param type : event information
     */
    void dispatchEvent(OSEventType type, const OSEvent& ev);
    /**
     @brief Dispatch touch event.
     @param type : event type
     */
    void dispatchTouchEvent(const OSEvent& ev);

    //enum class InterfaceType {
    //};

protected:
    // InterfaceType _type;
    EventDispatchInterface* _evDispatchInterface;

private:
    DISABLE_COPY_AND_MOVE(OSAbstractInterface);
};

} // namespace cc