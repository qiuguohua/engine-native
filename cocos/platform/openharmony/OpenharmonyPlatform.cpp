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

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/js_native_api.h>
#include <napi/native_api.h>
#include <napi/node_api.h>

#include "application/ApplicationManager.h"
#include "application/CocosApplication.h"
#include "bindings/jswrapper/SeApi.h"
#include "platform/UniversalPlatform.h"
#include "platform/openharmony/modules/SystemWindow.h"

enum ContextType {
    APP_LIFECYCLE = 0,
    JS_PAGE_LIFECYCLE,
    XCOMPONENT_CONTEXT,
    XCOMPONENT_REGISTER_LIFECYCLE_CALLBACK,
    NATIVE_RENDER_API,
    WORKER_INIT,
    UV_ASYNC_SEND
};

namespace cc {
// NAPI Interface
napi_value OpenharmonyPlatform::GetContext(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value  exports;
    size_t      argc = 1;
    napi_value  args[1];
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

    if (argc != 1) {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return nullptr;
    }

    napi_valuetype valuetype;
    status = napi_typeof(env, args[0], &valuetype);
    if (status != napi_ok) {
        return nullptr;
    }
    if (valuetype != napi_number) {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return nullptr;
    }

    int64_t value;
    NAPI_CALL(env, napi_get_value_int64(env, args[0], &value));

    NAPI_CALL(env, napi_create_object(env, &exports));

    switch (value) {
        case APP_LIFECYCLE: {
            /**** 1. AppInit 对应 app.ets中的应用生命周期 onCreate, onShow, onHide, onDestroy ******/
            LOGE("kee cocos GetContext APP_LIFECYCLE");
            /**** Register App Lifecycle  ******/
            napi_property_descriptor desc[] = {
                DECLARE_NAPI_FUNCTION("onCreate", OpenharmonyPlatform::NapiOnCreate),
                DECLARE_NAPI_FUNCTION("onShow", OpenharmonyPlatform::NapiOnShow),
                DECLARE_NAPI_FUNCTION("onHide", OpenharmonyPlatform::NapiOnHide),
                DECLARE_NAPI_FUNCTION("onDestroy", OpenharmonyPlatform::NapiOnDestroy),
            };
            NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
        }

        break;
        case JS_PAGE_LIFECYCLE: {
            LOGE("kee cocos GetContext JS_PAGE_LIFECYCLE");
#ifdef _ARKUI_DECLARATIVE_
            /****************  声明式开发范式 JS Page 生命周期注册 ****************************/
            napi_property_descriptor desc[] = {
                DECLARE_NAPI_FUNCTION("onPageShow", OpenharmonyPlatform::NapiOnPageShow),
                DECLARE_NAPI_FUNCTION("onPageHide", OpenharmonyPlatform::NapiOnPageHide),
            };
            NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
#else
            /****************  类Web开发范式 JS Page 生命周期注册 ****************************/
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
#endif
        } break;
        case XCOMPONENT_REGISTER_LIFECYCLE_CALLBACK: {
            LOGE("kee cocos GetContext XCOMPONENT_REGISTER_LIFECYCLE_CALLBACK");
        } break;
        case NATIVE_RENDER_API: {
            LOGE("kee cocos GetContext NATIVE_RENDER_API");
            //napi_property_descriptor desc[] = {
            //    DECLARE_NAPI_FUNCTION("changeColor", OpenharmonyPlatform::NapiChangeColorWorker),
            //};
            //NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
        } break;
        case WORKER_INIT: {
            LOGE("kee cocos GetContext WORKER_INIT");
            napi_property_descriptor desc[] = {
                DECLARE_NAPI_FUNCTION("workerInit", OpenharmonyPlatform::NapiWorkerInit),
            };
            NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
        } break;
        case UV_ASYNC_SEND: {
            LOGE("kee cocos GetContext UV_ASYNC_SEND");
            napi_property_descriptor desc[] = {
                DECLARE_NAPI_FUNCTION("send", OpenharmonyPlatform::NapiASend),
            };
            NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
        } break;
        default:
            LOGE("kee cocos unknown type");
    }
    return exports;
}

OpenharmonyPlatform* OpenharmonyPlatform::getInstance() {
    return dynamic_cast<OpenharmonyPlatform*>(BasePlatform::getPlatform());
}

OpenharmonyPlatform::OpenharmonyPlatform() {
    UniversalPlatform::init();
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

napi_value OpenharmonyPlatform::NapiWorkerInit(napi_env env, napi_callback_info info) {
    uv_loop_t*  loop  = nullptr;
    uv_check_t* check = new uv_check_t;
    NAPI_CALL(env, napi_get_uv_event_loop(env, &loop));
    OpenharmonyPlatform::getInstance()->WorkerInit(env, loop);
    return nullptr;
}

napi_value OpenharmonyPlatform::NapiASend(napi_env env, napi_callback_info info) {
    LOGE("kee cocos OpenharmonyPlatform::NapiASend");
    uv_async_send(&(OpenharmonyPlatform::getInstance()->workerOnMessageSignal_));
    return nullptr;
}

// int32_t OpenharmonyPlatform::OnInitNative() {
//     isRunning = true;
//     START_PLATFORM(0, nullptr);
// }

// void OpenharmonyPlatform::OnReadyNative() {
//     LOGE("COCOS PluginManager::OnReadyNative");
//     return;
// }

// void OpenharmonyPlatform::OnShowNative() {
//     LOGE("COCOS PluginManager::OnShowNative");
//     // WindowEvent ev;
//     // ev.type = WindowEvent::Type::SHOW;
//     // dispatchEvent(ev);
//     return;
// }

// void OpenharmonyPlatform::OnHideNative() {
//     WindowEvent ev;
//     ev.type = WindowEvent::Type::HIDDEN;
//     dispatchEvent(ev);
//     return;
// }

// void OpenharmonyPlatform::OnDestroyNative() {
//     WindowEvent ev;
//     ev.type = WindowEvent::Type::CLOSE;
//     dispatchEvent(ev);
//     isRunning = false;
//     return;
// }

// void OpenharmonyPlatform::OnActiveNative() {
//     LOGE("COCOS PluginManager::OnActiveNative");
//     return;
// }

// void OpenharmonyPlatform::OnInactiveNative() {
//     LOGE("COCOS PluginManager::OnInactiveNative");
//     return;
// }

// NAPI Interface
bool OpenharmonyPlatform::Export(napi_env env, napi_value exports) {
    napi_status status;
    // Application/SDK etc. Init
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

void OpenharmonyPlatform::MainOnMessage(const uv_async_t* req) {
    LOGE("kee cocos MainOnMessage Triggered");
}

// static
void OpenharmonyPlatform::WorkerOnMessage(const uv_async_t* req) {
    LOGE("kee cocos WorkerOnMessage Triggered");
    NativeXComponent* nativexcomponet = nullptr;
    void*             window          = nullptr;
    OpenharmonyPlatform::getInstance()->workerMessageQ_.DeQueue(reinterpret_cast<MessageDataType*>(&nativexcomponet));
    LOGE("kee cocos WorkerOnMessage nativexcomponent = %p", nativexcomponet);
    int32_t  ret;
    char     idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize                           = XCOMPONENT_ID_LEN_MAX + 1;
    ret                                       = NativeXComponent_GetXComponentId(nativexcomponet, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    ret = NativeXComponent_GetNativeWindow(nativexcomponet, &window);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    SystemWindow* systemWindowIntf = getPlatform()->getInterface<SystemWindow>();
    CCASSERT(systemWindowIntf, "Invalid interface pointer");
    systemWindowIntf->OnSurfaceCreated(nativexcomponet, window);
    getPlatform()->run(0, nullptr);
}

napi_value OpenharmonyPlatform::NapiOnCreate(napi_env env, napi_callback_info info) {
    uv_loop_t*  loop  = nullptr;
    uv_check_t* check = new uv_check_t;
    NAPI_CALL(env, napi_get_uv_event_loop(env, &loop));
    OpenharmonyPlatform::getInstance()->OnCreateNative(env, loop);
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

void OpenharmonyPlatform::OnCreateNative(napi_env env, uv_loop_t* loop) {
    LOGE("kee cocos PluginManager::OnCreateNative");
    mainEnv_  = env;
    mainLoop_ = loop;
    LOGE("kee cocos PluginManager::OnCreateNative mainEnv = %p, mainLoop = %p", mainEnv_, mainLoop_);
    if (mainLoop_) {
        LOGE("kee cocos OnCreateNative uv_async_init");
        uv_async_init(mainLoop_, &mainOnMessageSignal_, reinterpret_cast<uv_async_cb>(OpenharmonyPlatform::MainOnMessage));
    }
}
void OpenharmonyPlatform::OnShowNative() {
    LOGE("kee cocos PluginManager::OnShowNative");
}
void OpenharmonyPlatform::OnHideNative() {
    LOGE("kee cocos PluginManager::OnHideNative");
}
void OpenharmonyPlatform::OnDestroyNative() {
    LOGE("kee cocos PluginManager::OnDestroyNative");
}

#ifdef _ARKUI_DECLARATIVE_
napi_value OpenharmonyPlatform::NapiOnPageShow(napi_env env, napi_callback_info info) {
    LOGE("kee cocos PluginManager::NapiOnPageShow");
    return nullptr;
}

napi_value OpenharmonyPlatform::NapiOnPageHide(napi_env env, napi_callback_info info) {
    LOGE("kee cocos PluginManager::NapiOnPageHide");
    return nullptr;
}

void OpenharmonyPlatform::OnPageShowNative() {
    LOGE("kee cocos PluginManager::OnPageShowNative");
}

void OpenharmonyPlatform::OnPageHideNative() {
    LOGE("kee cocos PluginManager::OnPageHideNative");
}

void OpenharmonyPlatform::WorkerInit(napi_env env, uv_loop_t* loop) {
    LOGE("kee cocos PluginManager::WorkerInit");
    workerEnv_  = env;
    workerLoop_ = loop;
    LOGE("kee cocos PluginManager::WorkerInit workerEnv = %p, workerLoop = %p", workerEnv_, workerLoop_);
    if (workerLoop_) {
        LOGE("kee cocos WorkerInit uv_async_init");
        uv_async_init(workerLoop_, &workerOnMessageSignal_, reinterpret_cast<uv_async_cb>(OpenharmonyPlatform::WorkerOnMessage));
    }
}
#endif

}; // namespace cc
