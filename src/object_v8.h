#ifndef _OBJECT_V8_H_
#define _OBJECT_V8_H_

class ObjectV8{
public:
    ObjectV8();
    ObjectV8(v8::Handle<v8::Object> obj);
    ~ObjectV8();

    bool get(const char *param, bool def) const;
    int get(const char *param, int def) const;
    unsigned int get(const char *param, unsigned int def) const;
    double get(const char *param, double def) const;
    v8::Handle<v8::String> get(const char *param, const char *def) const;

    void set(const char *param, bool value);
    void set(const char *param, int value);
    void set(const char *param, unsigned int value);
    void set(const char *param, double value);
    void set(const char *param, const std::string &value);
    void set(const char *param, const char *value);

    v8::Handle<v8::Object> toJSObject() const;

private:
    v8::Persistent<v8::Object> _obj;
};

const char* toAscii(v8::Handle<v8::String> jsStr, char *buffer);

#endif // _OBJECT_V8_H_
