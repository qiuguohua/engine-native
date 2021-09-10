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

#pragma once

#include <string>
#include <android/native_window.h>
#include <android/asset_manager.h>
#include <mutex>
#include <condition_variable>

//#include "platform/os-interfaces/ISystemWindow.h"

namespace cc {

class MessagePipe;
class IEventDispatch;
class OSEvent;

class JniInteraction {
public:
    JniInteraction();
    static JniInteraction* getInstance();

    void init() ;
    void setSdkVersion(int sdkVersion);
    int getSdkVersion() const;
    void setObbPath(const std::string& path);
    void setAAssetManager(AAssetManager * assetManager);
    void setAppState(int8_t cmd);

    void writeCommand(int8_t cmd);
    int readCommand(int8_t &cmd);

    bool isRunning() const;
    void setRunning(bool isRunning);
    void waitRunning();

    void flushTasksOnGameThread() const;

    void setWindow(ANativeWindow *window);
    ANativeWindow * getWindowHandle();

    void execCommand();

    void setEventDispatch(IEventDispatch* eventDispatcher);
    void dispatchEvent(const OSEvent& ev);
    void dispatchTouchEvent(const OSEvent& ev);
    bool isPause() const;
private:
    void preExecCmd(int8_t cmd);
    void engineHandleCmd(int cmd);
    void postExecCmd(int8_t cmd);

    void handlePauseResume(int8_t cmd);

    IEventDispatch* _eventDispatcher;
    std::unique_ptr<MessagePipe> _messagePipe;
    AAssetManager *_assetManager = nullptr;
    ANativeWindow *_window = nullptr;
    std::string _obbPath;
    int _sdkVersion = 0;

    std::mutex _mutex;
    std::condition_variable _cond;
    ANativeWindow *_pendingWindow = nullptr;
    bool _animating = true;
    bool _running = false;

    // Current state of the app.  May be either APP_CMD_RESUME, APP_CMD_PAUSE.
    int _appState = 0;
    //SystemWindow* _systemWindow;
    //BasePlatform* _platform;
};
}

#define COCOS_INTERACTION() cc::JniInteraction::getInstance()