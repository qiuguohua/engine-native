#include "Class.h"
#include "CommonHeader.h"
#include "ScriptEngine.h"

namespace se {

napi_value *Class::_exports = nullptr;

Class::Class() : _parent(nullptr),
                 _proto(nullptr),
                 _parentProto(nullptr),
                 _ctorFunc(Class::_defaultCtor){};

Class::~Class() {
}

/* static */
Class *Class::create(const std::string &clsName, se::Object *parent, Object *parentProto, napi_callback ctor) {
    Class *cls = new Class();
    if (cls != nullptr && !cls->init(clsName, parent, parentProto, ctor)) {
        delete cls;
        cls = nullptr;
    }
    return cls;
}

bool Class::init(const std::string &clsName, Object *parent, Object *parentProto, napi_callback ctor) {
    _name   = clsName;
    _parent = parent;
    if (_parent != nullptr)
        _parent->incRef();
    _parentProto = parentProto;

    if (_parentProto != nullptr)
        _parentProto->incRef();
    if (ctor) {
        _ctorFunc = ctor;
    }

    return true;
}

napi_value Class::_defaultCtor(napi_env env, napi_callback_info info) {
    return nullptr;
}

void Class::defineProperty(const std::string &name, napi_callback g, napi_callback s) {
    _properties.push_back({name.c_str(), nullptr, nullptr, g, s, 0, napi_default, 0});
}

void Class::defineStaticProperty(const std::string &name, napi_callback g, napi_callback s) {
    _properties.push_back({name.c_str(), nullptr, nullptr, g, s, 0, napi_static, 0});
}

void Class::defineFunction(const std::string &name, napi_callback func) {
    _properties.push_back({name.c_str(), nullptr, func, nullptr, nullptr, nullptr, napi_default_method, nullptr});
}

void Class::defineStaticFunction(const std::string &name, napi_callback func) {
    _properties.push_back({name.c_str(), nullptr, func, nullptr, nullptr, 0, napi_static, 0});
}

void Class::defineFinalizeFunction(napi_finalize func) {
    assert(func != nullptr);
    _finalizeFunc = func;
}

napi_finalize Class::_getFinalizeFunction() const {
    return _finalizeFunc;
}

void Class::install() {
    napi_value  cons;
    napi_status status;
    NODE_API_CALL(status, ScriptEngine::getEnv(), napi_define_class(ScriptEngine::getEnv(), _name.c_str(), -1, _ctorFunc, nullptr, _properties.size(), _properties.data(), &cons));
    if (_parentProto) {
        inherit(ScriptEngine::getEnv(), cons, _parentProto->_getJSObject());
    }
    NODE_API_CALL(status, ScriptEngine::getEnv(),
                  napi_create_reference(ScriptEngine::getEnv(), cons, 1, &_constructor));

    NODE_API_CALL(status, ScriptEngine::getEnv(),
                  napi_set_named_property(ScriptEngine::getEnv(), _parent->_getJSObject(), _name.c_str(), cons));

    napi_value proto;
    NODE_API_CALL(status, ScriptEngine::getEnv(), napi_get_named_property(ScriptEngine::getEnv(), cons, "prototype", &proto));

    if (status == napi_ok) {
        _proto = Object::_createJSObject(ScriptEngine::getEnv(), proto, nullptr);
        _proto->root();
    }
}

    napi_status Class::inherit(napi_env env, napi_value subclass, napi_value superProto)
    {
        napi_value global, objectClass, setProto;
        napi_value argv[2];
        napi_value callbackResult = nullptr;

        LOGI("napi_inherit begin");
        napi_get_global(env, &global);
        napi_status status = napi_get_named_property(env, global, "Object", &objectClass);
        if (status != napi_ok) {
            LOGE("ace zbclog napi_get_named_property Object %{public}d", status);
            return napi_ok;
        }
        status = napi_get_named_property(env, objectClass, "setPrototypeOf", &setProto);
        if (status != napi_ok) {
            LOGE("ace zbclog napi_get_named_property setPrototypeOf %{public}d", status);
            return napi_ok;
        }

        status = napi_get_named_property(env, subclass, "prototype", &argv[0]);
        if (status != napi_ok) {
            LOGE("ace zbclog napi_get_named_property prototype arg0 %{public}d", status);
            return napi_ok;
        }
        argv[1] = superProto;
        status = napi_call_function(env, objectClass, setProto, 2, argv, &callbackResult);
        if (status != napi_ok) {
            LOGE("ace zbclog napi_call_function setProto 1 %{public}d", status);
            return napi_ok;
        }
        LOGI("napi_inherit end");

        return napi_ok;
    }

napi_value Class::_createJSObjectWithClass(Class *cls) {
    napi_value obj;
    napi_status status;
    assert(cls);
    napi_value clsCtor = cls->_getCtorFunc();
    if (!clsCtor) {
        LOGE("get ctor func err");
        return nullptr;
    }
    NODE_API_CALL(status, ScriptEngine::getEnv(), napi_new_instance( ScriptEngine::getEnv(), clsCtor, 0, nullptr, &obj));
    return obj;
}

Object *Class::getProto() const {
    //not impl
    return _proto;
}

napi_ref Class::_getCtorRef() const {
    return _constructor;
}

napi_value Class::_getCtorFunc() const {
    assert(_constructor);
    napi_value  result = nullptr;
    napi_status status;
    NODE_API_CALL(status, ScriptEngine::getEnv(), napi_get_reference_value(ScriptEngine::getEnv(), _constructor, &result));
    return result;
}
}; // namespace se