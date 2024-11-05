#include <Poco/JSON/Parser.h>
#include <Pson/Decoder.hpp>
#include <Pson/BinaryString.hpp>
#include <Pson/pson.h>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace Poco::JSON;
using Pson::BinaryString;
using Poco::Dynamic::Var;
using Poco::JSON::Array;
using Pson::Decoder;

pson_type pson_value_type(pson_value* value) {
    Var* var = (Var*)value;

    auto& type = var->type();
    if (type == typeid(void))         return PSON_NULL;
    if (type == typeid(bool))         return PSON_BOOL;
    if (type == typeid(Poco::Int32))  return PSON_INT64;
    if (type == typeid(Poco::Int64))  return PSON_INT64;
    if (type == typeid(float))        return PSON_FLOAT32;
    if (type == typeid(double))       return PSON_FLOAT64;
    if (type == typeid(std::string))  return PSON_STRING;
    if (type == typeid(BinaryString)) return PSON_BINARY;
    if (type == typeid(Array::Ptr))   return PSON_ARRAY;
    if (type == typeid(Object::Ptr))  return PSON_OBJECT;

    return PSON_INVALID;
}

int pson_parse(const char* buffer, size_t size, pson_value** result) {
    Decoder decoder;
    try {
        Var var = decoder.decode(std::string(buffer, size));
        *result = (pson_value*)(new Var(var));
        return true;
    } catch(...) {
        *result = NULL;
        return false;
    }
}

void pson_free_result(pson_value* result) {
    delete (Var*)result;
}

struct pson_object_iterator {
    pson_object_iterator(const Object::Ptr& obj) : obj(obj), started(false) {
        it = obj->begin();
    };
    Object::Ptr obj;
    Object::ConstIterator it;
    bool started;
};

int pson_open_object_iterator(pson_value* value, pson_object_iterator** iterator) {
    Var* var = (Var*)value;
    if (var->type() != typeid(Object::Ptr))
        return false;
    *iterator = new pson_object_iterator(var->extract<Object::Ptr>());
    return true;
}

int pson_object_iterator_next(pson_object_iterator* iterator, const char** key, pson_value** val) {
    if (iterator->it == iterator->obj->end())
        return false;
    if (!iterator->started) {
        iterator->started = true;
    } else {
        ++(iterator->it);
        if (iterator->it == iterator->obj->end())
            return false;
    }
    *key = iterator->it->first.c_str();
    *val = (pson_value*)&(iterator->it->second);
    return true;
}

void pson_close_object_iterator(pson_object_iterator* iterator) {
    delete iterator;
}

int pson_object_get_value_or_null_as_copy(pson_value* obj, const char* key, pson_value** result) {
    Var* var = (Var*)obj;
    if (var->type() != typeid(Object::Ptr))
        return false;
    *result = (pson_value*)new Var(var->extract<Object::Ptr>()->get(key));
    return true;
}

int pson_get_int32(pson_value* value, int32_t* result) {
    Var* var = (Var*)value;
    if (var->type() != typeid(Poco::Int32))
        return false;
    *result = var->convert<int32_t>();
    return true;
}

int pson_get_int64(pson_value* value, int64_t* result) {
    Var* var = (Var*)value;
    if (!var->isInteger())
        return false;
    *result = var->convert<int64_t>();
    return true;
}

int pson_get_float32(pson_value* value, float* result) {
    Var* var = (Var*)value;
    if (var->type() != typeid(float))
        return false;
    *result = var->convert<float>();
    return true;
}

int pson_get_float64(pson_value* value, double* result) {
    Var* var = (Var*)value;
    if (!var->isNumeric())
        return false;
    *result = var->convert<double>();
    return true;
}

int pson_is_null(pson_value* value) { 
    Var* var = (Var*)value; 
    return var->type() == typeid(void); 
}

int pson_get_bool(pson_value* value, int* result) {
    Var* var = (Var*)value;
    if (!var->isBoolean())
        return false;
    *result = var->convert<bool>();
    return true;
}

const char* pson_get_cstring(pson_value* value) {
    Var* var = (Var*)value;
    if (var->type() != typeid(std::string))
        return NULL;
    const std::string& str = var->extract<std::string>();
    return str.c_str();
}

int pson_inspect_binary(pson_value* value, const char** data, size_t* size) {
    Var* var = (Var*)value;
    if (var->type() == typeid(std::string)) {
        const std::string& str = var->extract<std::string>();
        *data = str.data();
        *size = str.size();
        return true;
    }
    if (var->type() == typeid(BinaryString)) {
        const BinaryString& bin = var->extract<BinaryString>();
        *data = bin.data();
        *size = bin.size();
        return true;
    }
    return false;
}

int pson_get_array_size(pson_value* array, size_t* size) {
    Var* var = (Var*)array;
    if (!var->isArray()) 
        return false;
    Array::Ptr arr = var->extract<Array::Ptr>();
    *size = arr->size();
    return true;
}

pson_value* pson_get_array_value(pson_value* array, size_t offset) {
    Var* var = (Var*)array;
    if (!var->isArray()) 
        return NULL;
    Array::Ptr arr = var->extract<Array::Ptr>();
    if (offset < 0 || offset >= arr->size())
        return NULL;
    // Hack:
    return (pson_value*)&(*(arr->begin() + offset));
}

pson_value* pson_new_null() {
    return (pson_value*)new Var();
}

pson_value* pson_new_bool(int val) {
    return (pson_value*)new Var((bool)val);
}

pson_value* pson_new_int32(int32_t val) {
    return (pson_value*)new Var(val);
}

pson_value* pson_new_int64(int64_t val) {
    return (pson_value*)new Var(val);
}

pson_value* pson_new_float32(float val) {
    return (pson_value*)new Var(val);
}

pson_value* pson_new_float64(double val) {
    return (pson_value*)new Var(val);
}

pson_value* pson_new_string(const char* val) {
    return (pson_value*)new Var(val);
}

pson_value* pson_new_binary(const char* data, size_t size) {
    BinaryString binary({data, size});
    return (pson_value*)new Var(binary);
}

pson_value* pson_new_array() {
    Array::Ptr array = new Array();
    return (pson_value*)new Var(array);
}

int pson_add_array_value(pson_value* array, pson_value* value) {
    Var* var = (Var*)array;
    if (!var->isArray()) 
        return false;
    Array::Ptr arr = var->extract<Array::Ptr>();
    Var* val = (Var*)value;
    arr->add(*val);
    return true;
}

pson_value* pson_new_object() {
    Object::Ptr object = new Object();
    return (pson_value*)new Var(object);
}

int pson_set_object_value(pson_value* object, const char* key, pson_value* value) {
    Var* var = (Var*)object;
    if (var->type() != typeid(Object::Ptr))
        return false;
    Object::Ptr obj = var->extract<Object::Ptr>();
    Var* val = (Var*)value;
    obj->set(key, *val);
    return true;
}

void pson_free_value(pson_value* value) {
    delete (Var*)value;
}
