#pragma once
#include <cassert>
#include "../RefCounter.h"
#include "../Value.h"
#include "../config.h"
#include "CommonHeader.h"
#include "HelperMacros.h"

namespace se {
class Class;

class ObjectRef {
private:
    napi_ref   _ref       = nullptr;
    int        _refCounts = 0;
    napi_env   _env       = nullptr;
    napi_value _obj       = nullptr;

public:
    ~ObjectRef() {
        deleteRef();
    }
    napi_value getValue(napi_env env) const {
        if (!_ref) {
            return _obj;
        }
        napi_value  result;
        napi_status status;
        NODE_API_CALL(status, env, napi_get_reference_value(env, _ref, &result));
        assert(status == napi_ok);
        return result;
    }
    void initWeakref(napi_env env, napi_value obj) {
        assert(_ref == nullptr);
        _obj = obj;
        _env = env;
        return;
        napi_create_reference(env, obj, 0, &_ref);
    }
    void setWeakref(napi_env env, napi_ref ref) {
        assert(_ref == nullptr);
        _ref = ref;
    }
    void initStrongRef(napi_env env, napi_value obj) {
        assert(_ref == nullptr);
        _refCounts = 1;
        _obj       = obj;
        napi_create_reference(env, obj, _refCounts, &_ref);
        _env = env;
    }
    void incRef(napi_env env) {
        assert(_ref == nullptr);
        assert(_refCounts == 0);
        _refCounts = 1;
        napi_create_reference(env, _obj, _refCounts, &_ref);
        return;
        if (_refCounts == 0) {
            uint32_t result = 0;
            _refCounts      = 1;
            napi_reference_ref(env, _ref, &result);
        }
    }
    void decRef(napi_env env) {
        assert(_refCounts == 1);
        deleteRef();
        return;
        uint32_t result = 0;
        if (_refCounts > 0) {
            _refCounts--;
            napi_reference_unref(env, _ref, &result);
        }
    }
    void deleteRef() {
        if (!_ref) {
            return;
        }
        _refCounts = 0;
        napi_delete_reference(_env, _ref);
        _ref = nullptr;
    }
};

class Object;

class Object : public RefCounter {
public:
    enum class TypedArrayType {
        NONE,
        INT8,
        INT16,
        INT32,
        UINT8,
        UINT8_CLAMPED,
        UINT16,
        UINT32,
        FLOAT32,
        FLOAT64
    };

    Object();
    ~Object();
    /**
         *  @brief Sets a property to an object.
         *  @param[in] name A utf-8 string containing the property's name.
         *  @param[in] value A value to be used as the property's value.
         *  @return true if the property is set successfully, otherwise false.
         */
    bool setProperty(const char *name, const Value &data);

    inline bool setProperty(const std::string &name, const Value &value) {
        return setProperty(name.c_str(), value);
    }

    /**
         *  @brief Gets a property from an object.
         *  @param[in] name A utf-8 string containing the property's name.
         *  @param[out] value The property's value if object has the property, otherwise the undefined value.
         *  @return true if object has the property, otherwise false.
         */
    bool getProperty(const char *name, Value *data);

    inline bool getProperty(const std::string &name, Value *value) {
        return getProperty(name.c_str(), value);
    }

    /**
         *  @brief Sets a pointer to private data on an object.
         *  @param[in] data A void* to set as the object's private data.
         *  @note This method will associate private data with se::Object by std::unordered_map::emplace.
         *        It's used for search a se::Object via a void* private data.
         */
    void setPrivateData(void *data);

    /**
         *  @brief Gets an object's private data.
         *  @return A void* that is the object's private data, if the object has private data, otherwise nullptr.
         */
    void *getPrivateData() const;

    /**
         *  @brief Clears private data of an object.
         *  @param clearMapping Whether to clear the mapping of native object & se::Object.
         */
    void clearPrivateData(bool clearMapping = true);

    /**
         *  @brief Tests whether an object is an array.
         *  @return true if object is an array, otherwise false.
         */
    bool isArray() const;

    /**
         *  @brief Gets array length of an array object.
         *  @param[out] length The array length to be stored. It's set to 0 if there is an error.
         *  @return true if succeed, otherwise false.
         */
    bool getArrayLength(uint32_t *length) const;

    /**
         *  @brief Gets an element from an array object by numeric index.
         *  @param[in] index An integer value for index.
         *  @param[out] data The se::Value to be stored for the element in certain index.
         *  @return true if succeed, otherwise false.
         */
    bool getArrayElement(uint32_t index, Value *data) const;

    /**
         *  @brief Sets an element to an array object by numeric index.
         *  @param[in] index An integer value for index.
         *  @param[in] data The se::Value to be set to array with certain index.
         *  @return true if succeed, otherwise false.
         */
    bool setArrayElement(uint32_t index, const Value &data);

    /** @brief Tests whether an object is a typed array.
         *  @return true if object is a typed array, otherwise false.
         */
    bool isTypedArray() const;

    /**
         *  @brief Gets the type of a typed array object.
         *  @return The type of a typed array object.
         */
    TypedArrayType getTypedArrayType() const;

    /**
         *  @brief Gets backing store of a typed array object.
         *  @param[out] ptr A temporary pointer to the backing store of a JavaScript Typed Array object.
         *  @param[out] length The byte length of a JavaScript Typed Array object.
         *  @return true if succeed, otherwise false.
         */
    bool getTypedArrayData(uint8_t **ptr, size_t *length) const;
    /**
         *  @brief Tests whether an object is an array buffer object.
         *  @return true if object is an array buffer object, otherwise false.
         */
    bool isArrayBuffer() const;

    /**
         *  @brief Gets buffer data of an array buffer object.
         *  @param[out] ptr A pointer to the data buffer that serves as the backing store for a JavaScript Typed Array object.
         *  @param[out] length The number of bytes in a JavaScript data object.
         *  @return true if succeed, otherwise false.
         */
    bool getArrayBufferData(uint8_t **ptr, size_t *length) const;

    /**
         *  @brief Creates a JavaScript Typed Array Object with specified format from an existing pointer,
                   if provide a null pointer,then will create a empty JavaScript Typed Array Object.
         *  @param[in] type The format of typed array.
         *  @param[in] data A pointer to the byte buffer to be used as the backing store of the Typed Array object.
         *  @param[in] byteLength The number of bytes pointed to by the parameter bytes.
         *  @return A JavaScript Typed Array Object whose backing store is the same as the one pointed data, or nullptr if there is an error.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *createTypedArray(TypedArrayType type, const void *data, size_t byteLength);

    /**
         *  @brief Tests whether an object can be called as a function.
         *  @return true if object can be called as a function, otherwise false.
         */
    bool isFunction() const;

    /**
         *  @brief Defines a function with a native callback for an object.
         *  @param[in] funcName A utf-8 string containing the function name.
         *  @param[in] func The native callback triggered by JavaScript code.
         *  @return true if succeed, otherwise false.
         */
    bool defineFunction(const char *funcName, napi_callback func);

    /**
         *  @brief Defines a property with native accessor callbacks for an object.
         *  @param[in] name A utf-8 string containing the property's name.
         *  @param[in] getter The native callback for getter.
         *  @param[in] setter The native callback for setter.
         *  @return true if succeed, otherwise false.
         */
    bool defineProperty(const char *name, napi_callback getter, napi_callback setter);

    bool attachObject(Object *obj);

    /**
         *  @brief Detaches an object from current object.
         *  @param[in] obj The object to be detached.
         *  @return true if succeed, otherwise false.
         *  @note The attached object will not be released if current object is not garbage collected.
         */
    bool detachObject(Object *obj);

    /**
         *  @brief Calls an object as a function.
         *  @param[in] args A se::Value array of arguments to pass to the function. Pass se::EmptyValueArray if argumentCount is 0.
         *  @param[in] thisObject The object to use as "this," or NULL to use the global object as "this."
         *  @param[out] rval The se::Value that results from calling object as a function, passing nullptr if return value is ignored.
         *  @return true if object is a function and there isn't any errors, otherwise false.
         */
    bool call(const ValueArray &args, Object *thisObject, Value *rval = nullptr);

    /**
         *  @brief Creates a JavaScript Native Binding Object from an existing se::Class instance.
         *  @param[in] cls The se::Class instance which stores native callback informations.
         *  @return A JavaScript Native Binding Object, or nullptr if there is an error.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *createObjectWithClass(Class *cls);

    static Object *_createJSObject(napi_env env, napi_value js_object, Class *cls);

    /**
         *  @brief Creates a JavaScript Object like `{} or new Object()`.
         *  @return A JavaScript Object, or nullptr if there is an error.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *createPlainObject();

    /**
         *  @brief Creates a JavaScript Array Object like `[] or new Array()`.
         *  @param[in] length The initical length of array.
         *  @return A JavaScript Array Object, or nullptr if there is an error.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *createArrayObject(size_t length);

    /**
         *  @brief Creates a JavaScript Array Buffer object from an existing pointer.
         *  @param[in] bytes A pointer to the byte buffer to be used as the backing store of the Typed Array object.
         *  @param[in] byteLength The number of bytes pointed to by the parameter bytes.
         *  @return A Array Buffer Object whose backing store is the same as the one pointed to data, or nullptr if there is an error.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *createArrayBufferObject(void *data, size_t byteLength);

    /**
         *  @brief Roots an object from garbage collection.
         *  @note Use this method when you want to store an object in a global or on the heap, where the garbage collector will not be able to discover your reference to it.
         *        An object may be rooted multiple times and must be unrooted an equal number of times before becoming eligible for garbage collection.
         */
    void root();

    /**
         *  @brief Unroots an object from garbage collection.
         *  @note An object may be rooted multiple times and must be unrooted an equal number of times before becoming eligible for garbage collection.
         */
    void unroot();

    /**
         *  @brief Tests whether an object is rooted.
         *  @return true if it has been already rooted, otherwise false.
         */
    bool isRooted() const;

    /**
         *  @brief Creates a JavaScript Object from a JSON formatted string.
         *  @param[in] jsonStr The utf-8 string containing the JSON string to be parsed.
         *  @return A JavaScript Object containing the parsed value, or nullptr if the input is invalid.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *createJSONObject(const std::string &jsonStr);

    /**
         *  @brief Gets all property names of an object.
         *  @param[out] allKeys A string vector to store all property names.
         *  @return true if succeed, otherwise false.
         */
    bool getAllKeys(std::vector<std::string> *allKeys) const;

    /**
         *  @brief Gets a se::Object from an existing native object pointer.
         *  @param[in] ptr The native object pointer associated with the se::Object
         *  @return A JavaScript Native Binding Object, or nullptr if there is an error.
         *  @note The return value (non-null) has to be released manually.
         */
    static Object *getObjectWithPtr(void *ptr);

    Class *    _getClass() const; // NOLINT(readability-identifier-naming)
    napi_value _getJSObject() const;
    void       _setFinalizeCallback(napi_finalize finalizeCb); // NOLINT(readability-identifier-naming)
    /**
         *  @brief Returns the string for describing current object.
         *  @return The string for describing current object.
         */
    std::string toString() const;

    bool init(napi_env env, napi_value js_object, Class *cls);

private:
    static void weakCallback(napi_env env, void *nativeObject, void * /*finalize_hint*/);
    static void setup();
    static void cleanup();

private:
    ObjectRef     _objRef;
    napi_finalize _finalizeCb  = nullptr;
    void *        _privateData = nullptr;
    napi_env      _env         = nullptr;
    Class *       _cls         = nullptr;
    uint32_t      _rootCount   = 0;

    friend class ScriptEngine;
};
}; // namespace se