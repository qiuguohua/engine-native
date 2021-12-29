#pragma once
#include <napi/js_native_api.h>
#include <napi/node_api.h>

// Empty value so that macros here are able to return NULL or void
#define NODE_API_RETVAL_NOTHING // Intentionally blank #define

#define GET_AND_THROW_LAST_ERROR(env)                                                                                          \
    do {                                                                                                                       \
        const napi_extended_error_info* error_info;                                                                            \
        napi_get_last_error_info((env), &error_info);                                                                          \
        bool is_pending;                                                                                                       \
        napi_is_exception_pending((env), &is_pending);                                                                         \
        /* If an exception is already pending, don't rethrow it */                                                             \
        if (!is_pending) {                                                                                                     \
            const char* error_message = error_info->error_message != NULL ? error_info->error_message : "empty error message"; \
            napi_throw_error((env), NULL, error_message);                                                                      \
        }                                                                                                                      \
    } while (0)

#define NODE_API_ASSERT_BASE(env, assertion, message, ret_val)  \
    do {                                                        \
        if (!(assertion)) {                                     \
            napi_throw_error(                                   \
                (env),                                          \
                NULL,                                           \
                "assertion (" #assertion ") failed: " message); \
            return ret_val;                                     \
        }                                                       \
    } while (0)

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define NODE_API_ASSERT(env, assertion, message) \
    NODE_API_ASSERT_BASE(env, assertion, message, NULL)

// Returns empty on failed assertion.
// This is meant to be used inside functions with void return type.
#define NODE_API_ASSERT_RETURN_VOID(env, assertion, message) \
    NODE_API_ASSERT_BASE(env, assertion, message, NODE_API_RETVAL_NOTHING)

#define NODE_API_CALL_BASE(env, the_call, ret_val) \
    do {                                           \
        if ((the_call) != napi_ok) {               \
            assert(false);                         \
        }                                          \
    } while (0)

// Returns nullptr if the_call doesn't return napi_ok.
#define NODE_API_CALL(status, env, the_call) \
    status = the_call;                       \
    LOGI("error:%d", status);                \
    NODE_API_CALL_BASE(env, status, nullptr)

// Returns empty if the_call doesn't return napi_ok.
#define NODE_API_CALL_RETURN_VOID(env, the_call) \
    NODE_API_CALL_BASE(env, the_call, NODE_API_RETVAL_NOTHING)

#define DECLARE_NODE_API_PROPERTY(name, func) \
    { (name), nullptr, (func), nullptr, nullptr, nullptr, napi_default, nullptr }

#define DECLARE_NODE_API_GETTER(name, func) \
    { (name), nullptr, nullptr, (func), nullptr, nullptr, napi_default, nullptr }

void add_returned_status(napi_env    env,
                         const char* key,
                         napi_value  object,
                         char*       expected_message,
                         napi_status expected_status,
                         napi_status actual_status);

void add_last_status(napi_env env, const char* key, napi_value return_value);
