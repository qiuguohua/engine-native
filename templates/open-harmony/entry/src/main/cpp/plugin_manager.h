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

#ifndef _PLUGIN_MANAGER_H_
#define _PLUGIN_MANAGER_H_

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/js_native_api.h>
#include <napi/node_api.h>
#include <string>
#include <unordered_map>
#include <uv.h>

#include "plugin_render.h"
#include "message_queue.h"

#define _ARKUI_DECLARATIVE_ 1

class PluginManager {
public:
    ~PluginManager() {}

    static PluginManager* GetInstance()
    {
        return &PluginManager::manager_;
    }

    static napi_value GetContext(napi_env env, napi_callback_info info);

    /******************************APP Lifecycle******************************/
    static napi_value NapiOnCreate(napi_env env, napi_callback_info info);
    static napi_value NapiOnShow(napi_env env, napi_callback_info info);
    static napi_value NapiOnHide(napi_env env, napi_callback_info info);
    static napi_value NapiOnDestroy(napi_env env, napi_callback_info info);

    void OnCreateNative(napi_env env, uv_loop_t* loop);
    void OnShowNative();
    void OnHideNative();
    void OnDestroyNative();
    /*********************************************************************/

#ifdef _ARKUI_DECLARATIVE_
    /******************************声明式范式******************************/
    /**                      JS Page : Lifecycle                        **/
    static napi_value NapiOnPageShow(napi_env env, napi_callback_info info);
    static napi_value NapiOnPageHide(napi_env env, napi_callback_info info);
    void OnPageShowNative();
    void OnPageHideNative();
    /*************************************************************************/

#else
    /******************************类 Web范式******************************/
    /**                      JS Page : Lifecycle                        **/
    static napi_value NapiOnInit(napi_env env, napi_callback_info info);
    static napi_value NapiOnReady(napi_env env, napi_callback_info info);
    static napi_value NapiOnShow(napi_env env, napi_callback_info info);
    static napi_value NapiOnHide(napi_env env, napi_callback_info info);
    static napi_value NapiOnDestroy(napi_env env, napi_callback_info info);
    static napi_value NapiOnActive(napi_env env, napi_callback_info info);
    static napi_value NapiOnInactive(napi_env env, napi_callback_info info);

    void OnInitNative();
    void OnReadyNative();
    void OnShowNative();
    void OnHideNative();
    void OnDestroyNative();
    void OnActiveNative();
    void OnInactiveNative();
    /*************************************************************************/
#endif

    /*************************Worker Func*************************************/
    static napi_value NapiWorkerInit(napi_env env, napi_callback_info info);
    static napi_value NapiASend(napi_env env, napi_callback_info info);

    void WorkerInit(napi_env env, uv_loop_t* loop);
    /*************************************************************************/

    NativeXComponent* GetNativeXComponent(std::string& id);
    void SetNativeXComponent(std::string& id, NativeXComponent* nativeXComponent);
    PluginRender* GetRender(std::string& id);

public:
    // Napi export
    bool Export(napi_env env, napi_value exports);
private:
    static void MainOnMessage(const uv_async_t* req);
    static void WorkerOnMessage(const uv_async_t* req);
    static void NapiChangeColorWorker(const uv_async_t* req);

    static PluginManager manager_;

    std::string id_;
    std::unordered_map<std::string, NativeXComponent*> nativeXComponentMap_;
    std::unordered_map<std::string, PluginRender*> pluginRenderMap_;

public:
    napi_env mainEnv_ = nullptr;
    uv_loop_t* mainLoop_ = nullptr;
    uv_async_t mainOnMessageSignal_ {};

    napi_env workerEnv_ = nullptr;
    uv_loop_t* workerLoop_ = nullptr;
    uv_async_t workerOnMessageSignal_ {};
    uv_async_t workerChangeColorSignal_ {};

    MessageQueue workerMessageQ_;
};

#endif // _PLUGIN_MANAGER_H_