/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "plugin_manager.h"
#include "plugin_common.h"
#include <napi/js_native_api.h>
#include <napi/native_api.h>
#include <napi/node_api.h>
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <stdint.h>
#include <string>
#include <stdio.h>


enum ContextType {
    APP_LIFECYCLE = 0,
    JS_PAGE_LIFECYCLE,
    XCOMPONENT_CONTEXT,
    XCOMPONENT_REGISTER_LIFECYCLE_CALLBACK,
    NATIVE_RENDER_API,
    WORKER_INIT,
    UV_ASYNC_SEND
};

PluginManager PluginManager::manager_;

// NAPI Interface

napi_value PluginManager::GetContext(napi_env env, napi_callback_info info)
{
    napi_status status;
    napi_value exports;
    size_t argc = 1;
    napi_value args[1];
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
        case APP_LIFECYCLE:
            {
                /**** 1. AppInit 对应 app.ets中的应用生命周期 onCreate, onShow, onHide, onDestroy ******/
                LOGE("kee cocos GetContext APP_LIFECYCLE");
                /**** Register App Lifecycle  ******/
                napi_property_descriptor desc[] = {
                    DECLARE_NAPI_FUNCTION("onCreate", PluginManager::NapiOnCreate),
                    DECLARE_NAPI_FUNCTION("onShow", PluginManager::NapiOnShow),
                    DECLARE_NAPI_FUNCTION("onHide", PluginManager::NapiOnHide),
                    DECLARE_NAPI_FUNCTION("onDestroy", PluginManager::NapiOnDestroy),
                };
                NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
            }

            break;
        case JS_PAGE_LIFECYCLE:
            {
                LOGE("kee cocos GetContext JS_PAGE_LIFECYCLE");
#ifdef _ARKUI_DECLARATIVE_
                /****************  声明式开发范式 JS Page 生命周期注册 ****************************/
                napi_property_descriptor desc[] = {
                    DECLARE_NAPI_FUNCTION("onPageShow", PluginManager::NapiOnPageShow),
                    DECLARE_NAPI_FUNCTION("onPageHide", PluginManager::NapiOnPageHide),
                };
                NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
#else
                /****************  类Web开发范式 JS Page 生命周期注册 ****************************/
                napi_property_descriptor desc[] ={
                    DECLARE_NAPI_FUNCTION("onInit", PluginManager::NapiOnInit),
                    DECLARE_NAPI_FUNCTION("onReady", PluginManager::NapiOnReady),
                    DECLARE_NAPI_FUNCTION("onShow", PluginManager::NapiOnShow),
                    DECLARE_NAPI_FUNCTION("onHide", PluginManager::NapiOnHide),
                    DECLARE_NAPI_FUNCTION("onDestroy", PluginManager::NapiOnDestroy),
                    DECLARE_NAPI_FUNCTION("onActive", PluginManager::NapiOnActive),
                    DECLARE_NAPI_FUNCTION("onInactive", PluginManager::NapiOnInactive),
                };
                NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
#endif
            }
            break;
        case XCOMPONENT_REGISTER_LIFECYCLE_CALLBACK:
            {
                LOGE("kee cocos GetContext XCOMPONENT_REGISTER_LIFECYCLE_CALLBACK");
            }
            break;
        case NATIVE_RENDER_API:
            {
                LOGE("kee cocos GetContext NATIVE_RENDER_API");
                napi_property_descriptor desc[] = {
                    DECLARE_NAPI_FUNCTION("changeColor", PluginRender::NapiChangeColorWorker),
                };
                NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
            }
            break;
        case WORKER_INIT:
            {
                LOGE("kee cocos GetContext WORKER_INIT");
                napi_property_descriptor desc[] = {
                    DECLARE_NAPI_FUNCTION("workerInit", PluginManager::NapiWorkerInit),
                };
                NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
            }
            break;
        case UV_ASYNC_SEND:
            {
                LOGE("kee cocos GetContext UV_ASYNC_SEND");
                napi_property_descriptor desc[] = {
                    DECLARE_NAPI_FUNCTION("send", PluginManager::NapiASend),
                };
                NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
            }
            break;
        default:
            LOGE("kee cocos unknown type");
    }
    return exports;
}

bool PluginManager::Export(napi_env env, napi_value exports)
{
    LOGE("kee cocos PluginManager::Export");
    napi_status status;
    napi_value exportInstance = nullptr;
    NativeXComponent *nativeXComponent = nullptr;
    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = { };
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;

    status = napi_get_named_property(env, exports, NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return false;
    }

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));
    if (status != napi_ok) {
        return false;
    }
    LOGE("kee cocos Export nativeXComponent = %p", nativeXComponent);
    ret = NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return false;
    }

    std::string id(idStr);
    auto context = PluginManager::GetInstance();
    if (context) {
        context->SetNativeXComponent(id, nativeXComponent);
        auto render = context->GetRender(id);
        render->SetNativeXComponent(nativeXComponent);
        render->Export(env, exports);
        return true;
    }

    return false;
}

void PluginManager::SetNativeXComponent(std::string& id, NativeXComponent* nativeXComponent)
{
    if (nativeXComponentMap_.find(id) == nativeXComponentMap_.end()) {
        nativeXComponentMap_[id] = nativeXComponent;
    } else {
        if (nativeXComponentMap_[id] != nativeXComponent) {
            nativeXComponentMap_[id] = nativeXComponent;
        }
    }
}

NativeXComponent* PluginManager::GetNativeXComponent(std::string& id)
{
    if (nativeXComponentMap_.find(id) == nativeXComponentMap_.end()) {
        return nullptr;
    } else {
        return nativeXComponentMap_[id];
    }
}

PluginRender* PluginManager::GetRender(std::string& id)
{
    if (pluginRenderMap_.find(id) == pluginRenderMap_.end()) {
        PluginRender* instance = PluginRender::GetInstance(id);
        pluginRenderMap_[id] = instance;
        return instance;
    } else {
        return pluginRenderMap_[id];
    }
}

void PluginManager::MainOnMessage(const uv_async_t* req)
{
    LOGE("kee cocos MainOnMessage Triggered");
}

void PluginManager::WorkerOnMessage(const uv_async_t* req)
{
    LOGE("kee cocos WorkerOnMessage Triggered");
    NativeXComponent* nativexcomponet = nullptr;
    void* window = nullptr;
    PluginManager::GetInstance()->workerMessageQ_.DeQueue(reinterpret_cast<MessageDataType*>(&nativexcomponet));
    LOGE("kee cocos WorkerOnMessage nativexcomponent = %p", nativexcomponet);
    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;
    ret = NativeXComponent_GetXComponentId(nativexcomponet, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    ret = NativeXComponent_GetNativeWindow(nativexcomponet, &window);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnSurfaceCreated(nativexcomponet, window);
}

void PluginManager::NapiChangeColorWorker(const uv_async_t* req)
{
    LOGE("kee cocos NapiChangeColorWorker Triggered");
    NativeXComponent* nativexcomponet = nullptr;
    PluginManager::GetInstance()->workerMessageQ_.DeQueue(reinterpret_cast<MessageDataType*>(&nativexcomponet));
    LOGE("kee cocos NapiChangeColorWorker nativexcomponent = %p", nativexcomponet);
    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;
    ret = NativeXComponent_GetXComponentId(nativexcomponet, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }
    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    if (render) {
        render->eglCore_->ChangeColor();
    }
}

napi_value PluginManager::NapiOnCreate(napi_env env, napi_callback_info info)
{
    uv_loop_t* loop = nullptr;
    uv_check_t* check = new uv_check_t;
    NAPI_CALL(env, napi_get_uv_event_loop(env, &loop));
    PluginManager::GetInstance()->OnCreateNative(env, loop);
    return nullptr;
}
napi_value PluginManager::NapiOnShow(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnShowNative();
    return nullptr;
}
napi_value PluginManager::NapiOnHide(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnHideNative();
    return nullptr;
}
napi_value PluginManager::NapiOnDestroy(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnDestroyNative();
    return nullptr;
}

napi_value PluginManager::NapiWorkerInit(napi_env env, napi_callback_info info)
{
    uv_loop_t* loop = nullptr;
    uv_check_t* check = new uv_check_t;
    NAPI_CALL(env, napi_get_uv_event_loop(env, &loop));
    PluginManager::GetInstance()->WorkerInit(env, loop);
    return nullptr;
}

napi_value PluginManager::NapiASend(napi_env env, napi_callback_info info)
{
    LOGE("kee cocos PluginManager::NapiASend");
    uv_async_send(&(PluginManager::GetInstance()->workerOnMessageSignal_));
    return nullptr;
}

void PluginManager::OnCreateNative(napi_env env, uv_loop_t* loop)
{
    LOGE("kee cocos PluginManager::OnCreateNative");
    mainEnv_ = env;
    mainLoop_ = loop;
    LOGE("kee cocos PluginManager::OnCreateNative mainEnv = %p, mainLoop = %p", mainEnv_, mainLoop_);
    if (mainLoop_) {
        LOGE("kee cocos OnCreateNative uv_async_init");
        uv_async_init(mainLoop_, &mainOnMessageSignal_, reinterpret_cast<uv_async_cb>(PluginManager::MainOnMessage));
    }
}
void PluginManager::OnShowNative()
{
    LOGE("kee cocos PluginManager::OnShowNative");
}
void PluginManager::OnHideNative()
{
    LOGE("kee cocos PluginManager::OnHideNative");
}
void PluginManager::OnDestroyNative()
{
    LOGE("kee cocos PluginManager::OnDestroyNative");
}

void PluginManager::WorkerInit(napi_env env, uv_loop_t* loop)
{
    LOGE("kee cocos PluginManager::WorkerInit");
    workerEnv_ = env;
    workerLoop_ = loop;
    LOGE("kee cocos PluginManager::WorkerInit workerEnv = %p, workerLoop = %p", workerEnv_, workerLoop_);
    if (workerLoop_) {
        LOGE("kee cocos WorkerInit uv_async_init");
        uv_async_init(workerLoop_, &workerOnMessageSignal_, reinterpret_cast<uv_async_cb>(PluginManager::WorkerOnMessage));
        uv_async_init(workerLoop_, &workerChangeColorSignal_, reinterpret_cast<uv_async_cb>(PluginManager::NapiChangeColorWorker));
    }
}

#ifdef _ARKUI_DECLARATIVE_
napi_value PluginManager::NapiOnPageShow(napi_env env, napi_callback_info info)
{
    LOGE("kee cocos PluginManager::NapiOnPageShow");
    return nullptr;
}

napi_value PluginManager::NapiOnPageHide(napi_env env, napi_callback_info info)
{
    LOGE("kee cocos PluginManager::NapiOnPageHide");
    return nullptr;
}

void PluginManager::OnPageShowNative()
{
    LOGE("kee cocos PluginManager::OnPageShowNative");
}

void PluginManager::OnPageHideNative()
{
    LOGE("kee cocos PluginManager::OnPageHideNative");
}
#else

void PluginManager::OnInitNative()
{
    LOGE("kee cocos PluginManager::OnInitNative");
    return;
}
void PluginManager::OnReadyNative()
{
    LOGE("kee cocos PluginManager::OnReadyNative");
    return;
}
void PluginManager::OnShowNative()
{
    LOGE("kee cocos PluginManager::OnShowNative");
    return;
}
void PluginManager::OnHideNative()
{
    LOGE("kee cocos PluginManager::OnHideNative");
    return;
}
void PluginManager::OnDestroyNative()
{
    LOGE("kee cocos PluginManager::OnDestroyNative");
    PluginManager::isPageInit = false;
    return;
}
void PluginManager::OnActiveNative()
{
    LOGE("kee cocos PluginManager::OnActiveNative:");
    return;
}
void PluginManager::OnInactiveNative()
{
    LOGE("kee cocos PluginManager::OnInactiveNative:");
    return;
}


napi_value PluginManager::NapiOnInit(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnInitNative();
    return nullptr;
}
napi_value PluginManager::NapiOnReady(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnReadyNative();
    return nullptr;
}
napi_value PluginManager::NapiOnShow(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnShowNative();
    return nullptr;
}
napi_value PluginManager::NapiOnHide(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnHideNative();
    return nullptr;
}
napi_value PluginManager::NapiOnDestroy(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnDestroyNative();
    return nullptr;
}
napi_value PluginManager::NapiOnActive(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnActiveNative();
    return nullptr;
}
napi_value PluginManager::NapiOnInactive(napi_env env, napi_callback_info info)
{
    PluginManager::GetInstance()->OnInactiveNative();
    return nullptr;
}
#endif