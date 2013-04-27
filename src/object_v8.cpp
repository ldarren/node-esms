#include <v8.h>
#include <string>
#include "./object_v8.h"

using namespace v8;

ObjectV8::ObjectV8(){
    _obj = Persistent<Object>::New(Object::New());
}
ObjectV8::ObjectV8(Handle<Object> obj){
    _obj = Persistent<Object>::New(obj);
}
ObjectV8::~ObjectV8(){
    _obj.Dispose();
}

bool ObjectV8::get(const char *param, bool def) const{
    HandleScope scope;
    Handle<String> p = String::NewSymbol(param);
    if (_obj->Has(p)) {
        Handle<Value> v = _obj->Get(p);
        if (v->IsBoolean()){
            return v->BooleanValue();
        }
    }
    return def;
}
int ObjectV8::get(const char *param, int def) const{
    HandleScope scope;
    Handle<String> p = String::NewSymbol(param);
    if (_obj->Has(p)) {
        Handle<Value> v = _obj->Get(p);
        if (v->IsInt32()){
            return v->Int32Value();
        }
    }
    return def;
}
unsigned int ObjectV8::get(const char *param, unsigned int def) const{
    HandleScope scope;
    Handle<String> p = String::NewSymbol(param);
    if (_obj->Has(p)) {
        Handle<Value> v = _obj->Get(p);
        if (v->IsUint32()){
            return v->Uint32Value();
        }
    }
    return def;
}
double ObjectV8::get(const char *param, double def) const{
    HandleScope scope;
    Handle<String> p = String::NewSymbol(param);
    if (_obj->Has(p)) {
        Handle<Value> v = _obj->Get(p);
        if (v->IsNumber()){
            return v->NumberValue();
        }
    }
    return def;
}
/*
 * How to get const char from Handle<String>
 *  Handle<String> testStr = objV8->get("TEST_STR", "NA");
 *  String::AsciiValue ascii(testStr); // String::Utf8Value utf(testStr);
 *  roster->Set(7, String::New(*ascii));
 */
Handle<String> ObjectV8::get(const char *param, const char *def) const{
    HandleScope scope;
    Handle<String> p = String::NewSymbol(param);
    if (_obj->Has(p)) {
        Handle<Value> v = _obj->Get(p);
        if (v->IsString()){
            return scope.Close(v->ToString());
        }
    }
    return String::New(def);
}

void ObjectV8::set(const char *param, bool value){
    _obj->Set(String::NewSymbol(param), Boolean::New(value));
}
void ObjectV8::set(const char *param, int value){
    _obj->Set(String::NewSymbol(param), Integer::New(value));
}
void ObjectV8::set(const char *param, unsigned int value){
    _obj->Set(String::NewSymbol(param), Integer::New(value));
}
void ObjectV8::set(const char *param, double value){
    _obj->Set(String::NewSymbol(param), Number::New(value));
}
void ObjectV8::set(const char *param, const std::string &value){
    set(param, value.c_str());
}
void ObjectV8::set(const char *param, const char *value){
    _obj->Set(String::NewSymbol(param), String::New(value));
}

Handle<Object> ObjectV8::toJSObject() const{
    return _obj;
}
