#include <jni.h>
#include <Hilog/log.h>
#include <GLES3/gl3.h>
#include "napi/js_native_api.h"
#include "napi/native_common.h"
#include "napi/node_api.h"


static napi_value Add(napi_env env, napi_callback_info info)
{
    size_t requireArgc = 2;
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args , nullptr, nullptr));

    napi_valuetype valuetype0;
    NAPI_CALL(env, napi_typeof(env, args[0], &valuetype0));

    napi_valuetype valuetype1;
    NAPI_CALL(env, napi_typeof(env, args[1], &valuetype1));

    double value0;
    NAPI_CALL(env, napi_get_value_double(env, args[0], &value0));

    double value1;
    NAPI_CALL(env, napi_get_value_double(env, args[1], &value1));

    napi_value sum;
    NAPI_CALL(env, napi_create_double(env, value0+value1, &sum));

    return sum;

}

static napi_value testNapiRun(napi_env env, napi_callback_info info){
    napi_value script,result;
    size_t argc =1;

    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, &script, NULL, NULL));

    NAPI_CALL(env, napi_run_script(env, script, &result));

    return result;
}

static napi_value getContext(napi_env env, napi_callback_info info){
    napi_value script,result;
    size_t argc =1;

    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, &script, NULL, NULL));

    NAPI_CALL(env, napi_run_script(env, script, &result));

    return result;
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports)
{

    napi_property_descriptor desc[] = {
      DECLARE_NAPI_FUNCTION("add",Add),
      DECLARE_NAPI_FUNCTION("testNapiRun",testNapiRun),
      DECLARE_NAPI_FUNCTION("getContext", getContext),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));

    return exports;
}
EXTERN_C_END


static napi_module demoModule = {
.nm_version =1,
.nm_flags = 0,
.nm_filename = nullptr,
.nm_register_func = Init,
.nm_modname = "hello",
.nm_priv = ((void*)0),
.reserved = { 0 },
};

extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&demoModule);
}