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

#include <stdint.h>

#include "plugin_render.h"
#include "plugin_common.h"
#include "plugin_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

std::unordered_map<std::string, PluginRender*> PluginRender::instance_;

NativeXComponentCallback PluginRender::callback_;

void OnSurfaceCreatedCB(NativeXComponent* component, void* window)
{
//    int32_t ret;
//    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
//    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;
//    ret = NativeXComponent_GetXComponentId(component, idStr, &idSize);
//    if (ret != XCOMPONENT_RESULT_SUCCESS) {
//        return;
//    }
//
//    std::string id(idStr);
//    auto render = PluginRender::GetInstance(id);
//    render->OnSurfaceCreated(component, window);
    LOGE("kee cocos OnSurfaceCreatedCB");
    PluginManager::GetInstance()->workerMessageQ_.EnQueue(component);
    uv_async_send(&(PluginManager::GetInstance()->workerOnMessageSignal_));
}

void OnSurfaceChangedCB(NativeXComponent* component, void* window)
{
    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;
    ret = NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnSurfaceChanged(component, window);
}

void OnSurfaceDestroyedCB(NativeXComponent* component, void* window)
{
    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;
    ret = NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->OnSurfaceDestroyed(component, window);
}

void DispatchTouchEventCB(NativeXComponent* component, void* window)
{
    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;
    ret = NativeXComponent_GetXComponentId(component, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return;
    }

    std::string id(idStr);
    auto render = PluginRender::GetInstance(id);
    render->DispatchTouchEvent(component, window);

}

PluginRender::PluginRender(std::string& id) : id_(id), component_(nullptr)
{
    eglCore_ = new EGLCore(id);
    auto renderCallback = PluginRender::GetNXComponentCallback();
    renderCallback->OnSurfcaeCreated = OnSurfaceCreatedCB;
    renderCallback->OnSurfaceChanged = OnSurfaceChangedCB;
    renderCallback->OnSurfaceDestroyed = OnSurfaceDestroyedCB;
    renderCallback->DispatchTouchEvent = DispatchTouchEventCB;
}

PluginRender* PluginRender::GetInstance(std::string& id)
{
    if (instance_.find(id) == instance_.end()) {
        PluginRender*  instance = new PluginRender(id);
        instance_[id] = instance;
        return instance;
    } else {
        return instance_[id];
    }
}

NativeXComponentCallback* PluginRender::GetNXComponentCallback()
{
    return &PluginRender::callback_;
}

void PluginRender::SetNativeXComponent(NativeXComponent* component)
{
    component_ = component;
    NativeXComponent_RegisterCallback(component_, &PluginRender::callback_);
}

void PluginRender::OnSurfaceCreated(NativeXComponent* component, void* window)
{
    int32_t ret=NativeXComponent_GetXComponentSize(component, window, &width_, &height_);
    if (ret == XCOMPONENT_RESULT_SUCCESS) {
        eglCore_->GLContextInit(window, width_, height_);
    }
}

void PluginRender::OnSurfaceChanged(NativeXComponent* component, void* window)
{
}

void PluginRender::OnSurfaceDestroyed(NativeXComponent* component, void* window)
{
}

void PluginRender::DispatchTouchEvent(NativeXComponent* component, void* window)
{
    int32_t ret1 = NativeXComponent_GetXComponentOffset(component, window, &x_, &y_);

    int32_t ret = NativeXComponent_GetTouchInfo(component, window, &touchInfo_);

    if (ret == XCOMPONENT_RESULT_SUCCESS) {
        // LOGE("kee cocos x = %f, y = %f", touchInfo_.x, touchInfo_.y);
    }

}

napi_value PluginRender::Export(napi_env env, napi_value exports)
{
    // Register JS API
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("changeShape", PluginRender::NapiChangeShape),
        DECLARE_NAPI_FUNCTION("drawTriangle", PluginRender::NapiDrawTriangle),
        DECLARE_NAPI_FUNCTION("changeColor", PluginRender::NapiChangeColor),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}

napi_value PluginRender::NapiChangeShape(napi_env env, napi_callback_info info)
{
    LOGE("kee cocos NapiChangeShape");
    napi_value exportInstance;
    napi_value thisArg;
    napi_status status;
    NativeXComponent *nativeXComponent = nullptr;

    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;

    // napi_value thisArg;
    NAPI_CALL(env, napi_get_cb_info(env, info, NULL, NULL, &thisArg, NULL));

    status = napi_get_named_property(env, thisArg, NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return nullptr;
    };

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));
    if (status != napi_ok) {
        return nullptr;
    }

    ret = NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return nullptr;
    }

    std::string id(idStr);
    PluginRender* instance = PluginRender::GetInstance(id);
    if (instance) {
        instance->eglCore_->ChangeShape();
    }
    return nullptr;
}

napi_value PluginRender::NapiDrawTriangle(napi_env env, napi_callback_info info)
{
    napi_value exportInstance;
    napi_value thisArg;
    napi_status status;
    NativeXComponent *nativeXComponent = nullptr;

    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;

    // napi_value thisArg;
    NAPI_CALL(env, napi_get_cb_info(env, info, NULL, NULL, &thisArg, NULL));

    status = napi_get_named_property(env, thisArg, NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return nullptr;
    };

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));
    if (status != napi_ok) {
        return nullptr;
    }

    ret = NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
    if (ret != XCOMPONENT_RESULT_SUCCESS) {
        return nullptr;
    }

    std::string id(idStr);
    PluginRender* instance = PluginRender::GetInstance(id);
    if (instance) {
        instance->eglCore_->DrawTriangle();
    }
    return nullptr;
}

napi_value PluginRender::NapiChangeColor(napi_env env, napi_callback_info info)
{
    LOGE("kee cocos NapiChangeColor");
    napi_value exportInstance;
    napi_value thisArg;
    napi_status status;
    NativeXComponent *nativeXComponent = nullptr;

    int32_t ret;
    char idStr[XCOMPONENT_ID_LEN_MAX + 1] = {};
    uint64_t idSize = XCOMPONENT_ID_LEN_MAX + 1;

    // napi_value thisArg;
    NAPI_CALL(env, napi_get_cb_info(env, info, NULL, NULL, &thisArg, NULL));

    status = napi_get_named_property(env, thisArg, NATIVE_XCOMPONENT_OBJ, &exportInstance);
    if (status != napi_ok) {
        return nullptr;
    }

    status = napi_unwrap(env, exportInstance, reinterpret_cast<void**>(&nativeXComponent));
    if (status != napi_ok) {
        return nullptr;
    }

    PluginManager::GetInstance()->workerMessageQ_.EnQueue(nativeXComponent);
    uv_async_send(&(PluginManager::GetInstance()->workerChangeColorSignal_));

//    ret = NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize);
//    if (ret != XCOMPONENT_RESULT_SUCCESS) {
//        return nullptr;
//    }
//
//    std::string id(idStr);
//    PluginRender* instance = PluginRender::GetInstance(id);
//    if (instance) {
//        instance->eglCore_->ChangeColor();
//    }
    return nullptr;
}

napi_value PluginRender::NapiChangeColorWorker(napi_env env, napi_callback_info info)
{
    LOGE("kee cocos NapiChangeColorWorker");
//    PluginManager::GetInstance()->workerMessageQ_.EnQueue(component);
//    uv_async_send(&(PluginManager::GetInstance()->workerChangeColorSignal_));
    return nullptr;
}
#ifdef __cplusplus
}
#endif