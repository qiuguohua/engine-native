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

#pragma once

#include "platform/UniversalPlatform.h"
#include "platform/openharmony/render/PluginRender.h"

#include <ace/xcomponent/native_interface_xcomponent.h>
#include <napi/js_native_api.h>
#include <napi/node_api.h>
#include <string>
#include <unordered_map>

namespace cc {
class OpenharmonyPlatform : public UniversalPlatform {
public:
    OpenharmonyPlatform();

    static OpenharmonyPlatform* getInstance();
    // JS API : Lifecycle
    static napi_value NapiOnInit(napi_env env, napi_callback_info info);
    static napi_value NapiOnReady(napi_env env, napi_callback_info info);
    static napi_value NapiOnShow(napi_env env, napi_callback_info info);
    static napi_value NapiOnHide(napi_env env, napi_callback_info info);
    static napi_value NapiOnDestroy(napi_env env, napi_callback_info info);
    static napi_value NapiOnActive(napi_env env, napi_callback_info info);
    static napi_value NapiOnInactive(napi_env env, napi_callback_info info);

    // Native logic : Lifecycle
    int32_t OnInitNative();
    void OnReadyNative();
    void OnShowNative();
    void OnHideNative();
    void OnDestroyNative();
    void OnActiveNative();
    void OnInactiveNative();

    // Napi export
    bool Export(napi_env env, napi_value exports);
    void SetNativeXComponent(NativeXComponent* component);

    void    pollEvent() override;
    int32_t run(int argc, const char** argv) override;
    int     getSdkVersion() const override;
    int32_t loop() override;

private:
    static bool isInit;
    bool isRunning{false};
    void waitWindowInitialized();
    std::string id_;
};
} // namespace cc