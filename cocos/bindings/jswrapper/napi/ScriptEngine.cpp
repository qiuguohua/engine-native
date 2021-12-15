#include "ScriptEngine.h"
#include <sstream>
#include "../MappingUtils.h"
#include "Class.h"
#include "CommonHeader.h"

namespace se {
ScriptEngine *gSriptEngineInstance = nullptr;

ScriptEngine::ScriptEngine() = default;

ScriptEngine::~ScriptEngine() = default;

void ScriptEngine::setFileOperationDelegate(const FileOperationDelegate &delegate) {
    _fileOperationDelegate = delegate;
}

const ScriptEngine::FileOperationDelegate &ScriptEngine::getFileOperationDelegate() const {
    return _fileOperationDelegate;
}

ScriptEngine *ScriptEngine::getInstance() {
    if (gSriptEngineInstance == nullptr) {
        gSriptEngineInstance = new ScriptEngine();
    }

    return gSriptEngineInstance;
}

void ScriptEngine::destroyInstance() {
    if (gSriptEngineInstance) {
        gSriptEngineInstance->cleanup();
        delete gSriptEngineInstance;
        gSriptEngineInstance = nullptr;
    }
}

bool ScriptEngine::runScript(const std::string &path, Value *ret /* = nullptr */) {
    assert(!path.empty());
    assert(_fileOperationDelegate.isValid());

    return false;
}

bool ScriptEngine::evalString(const char *scriptStr, ssize_t length, Value *ret, const char *fileName) {
    napi_status status;
    napi_value  script;
    napi_value  result;
    length = length < 0 ? NAPI_AUTO_LENGTH : length;
    status = napi_create_string_utf8(ScriptEngine::getEnv(), scriptStr, length, &script);

    NODE_API_CALL(status, ScriptEngine::getEnv(), napi_run_script(ScriptEngine::getEnv(), script, &result));
    return true;
}

bool ScriptEngine::init() {
    napi_status status;
    napi_value  result;
    NativePtrToObjectMap::init();
    NonRefNativePtrCreatedByCtorMap::init();
    NODE_API_CALL(status, ScriptEngine::getEnv(), napi_get_global(ScriptEngine::getEnv(), &result));
    _globalObj = Object::_createJSObject(ScriptEngine::getEnv(), result, nullptr);
    _globalObj->root();
    return true;
}

Object *ScriptEngine::getGlobalObject() const {
    return _globalObj;
}

bool ScriptEngine::start() {
    bool ok = true;
    init();

    for (auto cb : _registerCallbackArray) {
        ok = cb(_globalObj);
        assert(ok);
        if (!ok) {
            break;
        }
    }

    // After ScriptEngine is started, _registerCallbackArray isn't needed. Therefore, clear it here.
    _registerCallbackArray.clear();

    return ok;
}

void ScriptEngine::cleanup() {
    return;
}

void ScriptEngine::addBeforeCleanupHook(const std::function<void()> &hook) {
    return;
}

void ScriptEngine::addAfterCleanupHook(const std::function<void()> &hook) {
    return;
}

void ScriptEngine::addRegisterCallback(RegisterCallback cb) {
    assert(std::find(_registerCallbackArray.begin(), _registerCallbackArray.end(), cb) == _registerCallbackArray.end());
    _registerCallbackArray.push_back(cb);
}

napi_env ScriptEngine::getEnv() {
    return getInstance()->_env;
}

void ScriptEngine::setEnv(napi_env env) {
    getInstance()->_env = env;
}

void ScriptEngine::addPermanentRegisterCallback(RegisterCallback cb) {
    //not impl
    return;
}

void ScriptEngine::setExceptionCallback(const ExceptionCallback &cb) {
    //not impl
    return;
}

const std::chrono::steady_clock::time_point &ScriptEngine::getStartTime() const {
    //not  impl
    return std::chrono::steady_clock::time_point();
}

bool ScriptEngine::isValid() const {
    //not impl
    return true;
}

void ScriptEngine::enableDebugger(const std::string &serverAddr, uint32_t port, bool isWait) {
    //not impl
    return;
}

bool ScriptEngine::saveByteCodeToFile(const std::string &path, const std::string &pathBc) {
    //not impl
    return true;
}

void ScriptEngine::clearException() {
    //not impl
    return;
}

void ScriptEngine::garbageCollect() {
    //not impl
    return;
}

void ScriptEngine::setJSExceptionCallback(const ExceptionCallback &cb) {
    //not impl
    return;
}

void ScriptEngine::addAfterInitHook(const std::function<void()> &hook) {
    //not impl
    return;
}

std::string ScriptEngine::getCurrentStackTrace() {
    //not impl
    return "";
}
}; // namespace se