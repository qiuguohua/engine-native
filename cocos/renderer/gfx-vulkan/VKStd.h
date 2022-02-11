/****************************************************************************
 Copyright (c) 2020-2022 Xiamen Yaji Software Co., Ltd.

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

#include "base/CoreStd.h"

#if (CC_PLATFORM == CC_PLATFORM_WINDOWS || CC_PLATFORM == CC_PLATFORM_NX_WINDOWS)
    #if defined(CC_STATIC)
        #define CC_VULKAN_API
    #else
        #ifdef CC_VK_EXPORTS
            #define CC_VULKAN_API __declspec(dllexport)
        #else
            #define CC_VULKAN_API __declspec(dllimport)
        #endif
    #endif
#else
    #define CC_VULKAN_API
#endif

#if CC_DEBUG > 0
    #define VK_CHECK(x)                                                \
        do {                                                           \
            VkResult err = x;                                          \
            if (err) {                                                 \
                CC_LOG_ERROR("%s returned Vulkan error: %d", #x, err); \
                CCASSERT(false, "Vulkan Error");                       \
            }                                                          \
        } while (0)

#else
    #define VK_CHECK(x) x
#endif
