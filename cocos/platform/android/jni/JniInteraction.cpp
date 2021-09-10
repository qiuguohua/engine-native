/****************************************************************************
 Copyright (c) 2020-2021 Xiamen Yaji Software Co., Ltd.

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

#include "platform/android/jni/JniInteraction.h"
#include "platform/android/jni/MessagePipe.h"

#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/native_window_jni.h>
#include <android_native_app_glue.h>
#include <fcntl.h>
#include <jni.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include "JniCocosActivity.h"
#include "cocos/bindings/event/CustomEventTypes.h"
#include "platform/BasePlatform.h"
#include "platform/IEventDispatch.h"
#include "platform/android/FileUtils-android.h"
#include "platform/android/jni/JniCocosActivity.h"
#include "platform/java/jni/JniHelper.h"


namespace cc {
JniInteraction::JniInteraction() = default;

void JniInteraction::init() {
    _messagePipe = std::make_unique<MessagePipe>();

    BasePlatform* platform = cc::BasePlatform::getPlatform();
    if (platform->init()) {
        LOGV("Platform initialization failed");
    }
    platform->run(0, nullptr);
}

JniInteraction* JniInteraction::getInstance() {
    static JniInteraction cocosInstaction;
    return &cocosInstaction;
}

bool JniInteraction::isRunning() const {
    return _running;
}

void JniInteraction::setSdkVersion(int sdkVersion) {
    _sdkVersion = sdkVersion;
}

int JniInteraction::getSdkVersion() const {
    return _sdkVersion;
}

void JniInteraction::setObbPath(const std::string& path) {
    _obbPath = path;
}

void JniInteraction::setAAssetManager(AAssetManager* assetManager) {
    _assetManager = assetManager;
}

void JniInteraction::setWindow(ANativeWindow* window) {
    std::unique_lock<std::mutex> lk(_mutex);
    if (_pendingWindow) {
        writeCommand(APP_CMD_TERM_WINDOW);
    }
    _pendingWindow = window;
    if (window) {
        writeCommand(APP_CMD_INIT_WINDOW);
    }
    while (_window != _pendingWindow) {
        _cond.wait(lk);
    }
}

void JniInteraction::writeCommand(int8_t cmd) {
    _messagePipe->writeCommand(cmd);
}

int JniInteraction::readCommand(int8_t& cmd) {
    return _messagePipe->readCommand(cmd);
}

void JniInteraction::preExecCmd(int8_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            LOGV("APP_CMD_INIT_WINDOW");
            std::unique_lock<std::mutex> lk(_mutex);
            _window = _pendingWindow;
            lk.unlock();
            _cond.notify_all();
            _animating = true;
        } break;
        case APP_CMD_TERM_WINDOW:
            LOGV("APP_CMD_TERM_WINDOW");
            _animating = false;
            _cond.notify_all();
            break;
        case APP_CMD_RESUME:
            LOGV("APP_CMD_RESUME");
            handlePauseResume(cmd);
            break;
        case APP_CMD_PAUSE:
            LOGV("APP_CMD_PAUSE");
            handlePauseResume(cmd);
            break;
        default:
            break;
    }
}

void JniInteraction::postExecCmd(int8_t cmd) {
    switch (cmd) {
        case APP_CMD_TERM_WINDOW: {
            std::unique_lock<std::mutex> lk(_mutex);
            _window = nullptr;
            lk.unlock();
            _cond.notify_all();
        } break;
        default:
            break;
    }
}

void JniInteraction::setAppState(int8_t cmd) {
    std::unique_lock<std::mutex> lk(_mutex);
    writeCommand(cmd);
    while (_appState != cmd) {
        _cond.wait(lk);
    }
}

void JniInteraction::handlePauseResume(int8_t cmd) {
    LOGV("appState=%d", cmd);
    std::unique_lock<std::mutex> lk(_mutex);
    _appState = static_cast<uint8_t>(cmd);
    lk.unlock();
    _cond.notify_all();
}

void JniInteraction::flushTasksOnGameThread() const {
    if (_animating) {
        // Handle java events send by UI thread. Input events are handled here too.
        cc::JniHelper::callStaticVoidMethod(JCLS_HELPER,
                                            "flushTasksOnGameThread");
    }
}

void JniInteraction::setRunning(bool isRunning) {
    std::unique_lock<std::mutex> lk(_mutex);
    _running = isRunning;
    lk.unlock();
    _cond.notify_all();
    //_systemWindow = _platform->getInterfaces<SystemWindow>();
}

void JniInteraction::waitRunning() {
    std::unique_lock<std::mutex> lk(_mutex);
    while (!_running) {
        _cond.wait(lk);
    }
}

ANativeWindow* JniInteraction::getWindowHandle() {
    std::unique_lock<std::mutex> lk(_mutex);
    return _window;
}

void JniInteraction::setEventDispatch(IEventDispatch* eventDispatcher) {
    _eventDispatcher = eventDispatcher;
}

void JniInteraction::dispatchEvent(const OSEvent& ev) {
    if (_eventDispatcher) {
        _eventDispatcher->dispatchEvent(ev);
    }
}

void JniInteraction::dispatchTouchEvent(const OSEvent& ev) {
    if (_eventDispatcher) {
        _eventDispatcher->dispatchTouchEvent(ev);
    }
}

void JniInteraction::execCommand() {
    int8_t cmd = 0;
    if (readCommand(cmd) > 0) {
        preExecCmd(cmd);
        engineHandleCmd(cmd);
        postExecCmd(cmd);
    }
}

bool JniInteraction::isPause() const {
    if (!_animating) {
        return true;
    }
    if (_appState == APP_CMD_PAUSE) {
        return true;
    }
    return false;
}

void JniInteraction::engineHandleCmd(int cmd) {
    static bool isWindowInitialized = false;
    // Handle CMD here if needed.
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            if (!isWindowInitialized) {
                isWindowInitialized = true;
                return;
            }
            cc::CustomEvent event;
            event.name         = EVENT_RECREATE_WINDOW;
            event.args->ptrVal = reinterpret_cast<void*>(getWindowHandle());
            dispatchEvent(event);
        } break;
        case APP_CMD_TERM_WINDOW: {
            cc::CustomEvent event;
            event.name         = EVENT_DESTROY_WINDOW;
            event.args->ptrVal = reinterpret_cast<void*>(getWindowHandle());
            dispatchEvent(event);
        } break;
        case APP_CMD_RESUME: {
            WindowEvent ev;
            ev.type = WindowEvent::Type::SHOW;
            dispatchEvent(ev);
        } break;
        case APP_CMD_PAUSE: {
            WindowEvent ev;
            ev.type = WindowEvent::Type::HIDDEN;
            dispatchEvent(ev);
        } break;
        case APP_CMD_DESTROY: {
            LOGV("APP_CMD_DESTROY");
            WindowEvent ev;
            ev.type = WindowEvent::Type::CLOSE;
            dispatchEvent(ev);
        } break;
        case APP_CMD_LOW_MEMORY: {
            DeviceEvent ev;
            ev.type = DeviceEvent::Type::DEVICE_MEMORY;
            dispatchEvent(ev);
            break;
        }
        default:
            break;
    }
}

} // namespace cc
