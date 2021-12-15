#include "Class.h"
#include "CommonHeader.h"
#include "ScriptEngine.h"

namespace se {

    napi_value* Class::_exports = nullptr;

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
        _properties.push_back(DECLARE_NODE_API_PROPERTY(name.c_str(), func));
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
        napi_value cons;
        napi_status status;
        NODE_API_CALL(status, ScriptEngine::getEnv(), napi_define_class(
                ScriptEngine::getEnv(), _name.c_str(), -1, _ctorFunc, nullptr,
                _properties.size(),
                _properties.data(), &cons));

        NODE_API_CALL(status, ScriptEngine::getEnv(),
                napi_create_reference(ScriptEngine::getEnv(), cons, 1, &_constructor));

        NODE_API_CALL(status, ScriptEngine::getEnv(),
                napi_set_named_property(ScriptEngine::getEnv(), _parent->_getJSObject(), _name.c_str(), cons));

        napi_valuetype valType;
        NODE_API_CALL(status, ScriptEngine::getEnv()(), napi_typeof(ScriptEngine::getEnv(), cons, &valType));

        napi_value result;
        status = napi_coerce_to_object(ScriptEngine::getEnv(), cons, &result);
        NODE_API_CALL(status, ScriptEngine::getEnv()(), napi_typeof(ScriptEngine::getEnv(), result, &valType));


        napi_value proto;
        NODE_API_CALL(status, ScriptEngine::getEnv(),
                napi_get_prototype(ScriptEngine::getEnv(), cons, &proto));

        NODE_API_CALL(status, ScriptEngine::getEnv()(), napi_typeof(ScriptEngine::getEnv(), proto, &valType));

        if (status == napi_ok){
            _proto = Object::_createJSObject(ScriptEngine::getEnv(), proto, nullptr);
            _proto->root();
        }
    }

    napi_value Class::_createJSObjectWithClass(Class *cls) {
        //not impl
        return nullptr;
    }

    Object *Class::getProto() const {
        //not impl
        return _proto;
    }

    napi_ref Class::_getCtorRef() const {
        return _constructor;
    }

}; // namespace se