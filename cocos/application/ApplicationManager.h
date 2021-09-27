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
#include <vector>

#include "application/BaseApplication.h"

namespace cc {
class ApplicationManager {
public:
    static ApplicationManager* getInstance();

    using ApplcationPtr = std::shared_ptr<BaseApplication>;

    /*
     @bref Generate application entry.
     */
    template <class T>
    std::enable_if_t<std::is_base_of<BaseApplication, T>::value, ApplcationPtr>
    createApplication() {
        ApplcationPtr app(new T);
        _apps.push_back(app);
        _currentApp = app;
        return std::move(app);
    }

    /*
     @bref Release all generated applications.
     */
    void releseAllApplcation();
    /*
     @bref Get the current application, may get empty.
     */
    ApplcationPtr getCurrentApp() const;
    /*
     @bref Get the current application, make sure it is not empty.
     @bref Used to get the engine.
     */
    ApplcationPtr getCurrentAppSafe() const;

private:
    std::weak_ptr<BaseApplication> _currentApp;
    std::vector<ApplcationPtr>     _apps;
};
} // namespace cc

#define APPLICATION_MANAGER()        cc::ApplicationManager::getInstance()
#define CURRENT_APPLICATION()        APPLICATION_MANAGER()->getCurrentApp()
#define CURRENT_APPLICATION_SAFE()   APPLICATION_MANAGER()->getCurrentAppSafe()
#define CURRENT_ENGINE()             CURRENT_APPLICATION_SAFE()->getEngine()
#define GET_PLATFORM_INTERFACE(intf) CURRENT_ENGINE()->getOSInterface<intf>()

/*
 @bref Called at the user-defined main entry
 */
#define COCOST_START_APPLICATION(className)                               \
    do {                                                                  \
        auto app = APPLICATION_MANAGER()->createApplication<className>(); \
        if (app->init()) {                                                \
            return -1;                                                    \
        }                                                                 \
        return app->run(argc, argv);                                      \
    } while (0)

#define COCOS_APPLICATION_MAIN(className)         \
    int cocos_main(int argc, const char** argv) { \
        COCOST_START_APPLICATION(className);      \
    }
