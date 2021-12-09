/****************************************************************************
 Copyright (c) 2021 Xiamen Yaji Software Co., Ltd.

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
#include "platform/openharmony/OpenharmonyPlatform.h"
#include "platform/openharmony/common/PluginCommon.h"
#include "platform/openharmony/render/PluginRender.h"

#include <napi/native_api.h>
#include <string>
#include <thread>

#include "platform/UniversalPlatform.h"
#include "application/ApplicationManager.h"
#include "application/CocosApplication.h"
#include "platform/openharmony/modules/SystemWindow.h"
#include "bindings/jswrapper/SeApi.h"
#ifdef __cplusplus
extern "C" {
#endif

namespace cc {
bool OpenharmonyPlatform::isInit = false;

OpenharmonyPlatform* OpenharmonyPlatform::getInstance() {
    return dynamic_cast<OpenharmonyPlatform*>(BasePlatform::getPlatform());
}

OpenharmonyPlatform::OpenharmonyPlatform() {
}

int OpenharmonyPlatform::getSdkVersion() const {
    return 0;
}

int32_t OpenharmonyPlatform::run(int argc, const char** argv) {
    std::thread mainLogicThread([this, argc, argv]() {
        CC_START_APPLICATION(CocosApplication);
        UniversalPlatform::run(argc, argv);
        onDestory();
    });
    mainLogicThread.detach();
    return 0;
}

void OpenharmonyPlatform::waitWindowInitialized() {
}

int32_t OpenharmonyPlatform::loop() {
    while (isRunning) {
        pollEvent();
        runTask();
    }
    return 0;
}

void OpenharmonyPlatform::pollEvent() {
}

int32_t OpenharmonyPlatform::OnInitNative() {
    isRunning = true;
    START_PLATFORM(0, nullptr);
}

void OpenharmonyPlatform::OnReadyNative() {
    LOGE("COCOS PluginManager::OnReadyNative");
    return;
}

void OpenharmonyPlatform::OnShowNative() {
    WindowEvent ev;
    ev.type = WindowEvent::Type::SHOW;
    dispatchEvent(ev);
    return;
}

void OpenharmonyPlatform::OnHideNative() {
    WindowEvent ev;
    ev.type = WindowEvent::Type::HIDDEN;
    dispatchEvent(ev);
    return;
}

void OpenharmonyPlatform::OnDestroyNative() {
    WindowEvent ev;
    ev.type = WindowEvent::Type::CLOSE;
    dispatchEvent(ev);
    OpenharmonyPlatform::isInit = false;
    isRunning = false; 
    return;
}

void OpenharmonyPlatform::OnActiveNative() {
    LOGE("COCOS PluginManager::OnActiveNative");
    return;
}

void OpenharmonyPlatform::OnInactiveNative() {
    LOGE("COCOS PluginManager::OnInactiveNative");
    return;
}

// NAPI Interface
bool OpenharmonyPlatform::Export(napi_env env, napi_value exports) {
    napi_status status;
    // Application/SDK etc. Init
    if (!OpenharmonyPlatform::isInit) {
        LOGE("COCOS PluginManager Init");
        OpenharmonyPlatform::isInit = true;
        
        se::Class::setExports(&exports);
        se::ScriptEngine::setEnv(env);

        // Register JS Lifecycle
        napi_property_descriptor desc[] = {
            DECLARE_NAPI_FUNCTION("onInit", OpenharmonyPlatform::NapiOnInit),
            DECLARE_NAPI_FUNCTION("onReady", OpenharmonyPlatform::NapiOnReady),
            DECLARE_NAPI_FUNCTION("onShow", OpenharmonyPlatform::NapiOnShow),
            DECLARE_NAPI_FUNCTION("onHide", OpenharmonyPlatform::NapiOnHide),
            DECLARE_NAPI_FUNCTION("onDestroy", OpenharmonyPlatform::NapiOnDestroy),
            DECLARE_NAPI_FUNCTION("onActive", OpenharmonyPlatform::NapiOnActive),
            DECLARE_NAPI_FUNCTION("onInactive", OpenharmonyPlatform::NapiOnInactive),
        };
        NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
        return true;
    }

    // XComponent Init:
    napi_value        exportInstance   = nullptr;
    NativeXComponent* nativeXComponent = nullptr;
    int32_t           ret;

    char     idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize                           = XCOMPONENT_ID_LEN_MAX + 1;

    status = napi_get_named_property(env, exports, NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return false;
    }

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));
    if (status != napi_ok) {
        return false;
    }

    ret = NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return false;
    }

    SystemWindow* systemWindowIntf = getInterface<SystemWindow>();
    CCASSERT(systemWindowIntf, "Invalid interface pointer");
    systemWindowIntf->SetNativeXComponent(nativeXComponent);
    return true;
}


//NAPI Lifecycle
napi_value OpenharmonyPlatform::NapiOnInit(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnInitNative();
    return nullptr;
}
napi_value OpenharmonyPlatform::NapiOnReady(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnReadyNative();
    return nullptr;
}
napi_value OpenharmonyPlatform::NapiOnShow(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnShowNative();
    return nullptr;
}
napi_value OpenharmonyPlatform::NapiOnHide(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnHideNative();
    return nullptr;
}
napi_value OpenharmonyPlatform::NapiOnDestroy(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnDestroyNative();
    return nullptr;
}
napi_value OpenharmonyPlatform::NapiOnActive(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnActiveNative();
    return nullptr;
}
napi_value OpenharmonyPlatform::NapiOnInactive(napi_env env, napi_callback_info info) {
    OpenharmonyPlatform::getInstance()->OnInactiveNative();
    return nullptr;
}

}; // namespace cc

#ifdef __cplusplus
}
#endif
