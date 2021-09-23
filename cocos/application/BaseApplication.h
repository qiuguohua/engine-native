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

#include <memory>

namespace cc {

class BaseApplication {
public:
    virtual ~BaseApplication() = default;
    /*
     *@bref Application initialization
     */
    virtual int32_t init() = 0;
    /*
     *@bref Application main business logic.
     */
    virtual int32_t run(int          argc,
                        const char** argv) = 0;
    /*
     *@bref Pause the application.
     */
    virtual void pause() = 0;
    /*
     *@bref Resume the application.
     */
    virtual void resume() = 0;
    /*
     *@bref Restart the application.
     */
    virtual void restart() = 0;
    /*
     *@bref Close the application.
     */
    virtual void close() = 0;
};
} // namespace cc

#define COCOS_START_APPLICATION_MAIN(className)                           \
    int cocos_main(int argc, const char** argv) {                         \
        static std::unique_ptr<cc::BaseApplication> app(new className()); \
        if (app->init()) {                                                \
            return -1;                                                    \
        }                                                                 \
        return app->run(argc, argv);                                      \
    }
