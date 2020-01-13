#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct v8_value;

struct v8_array
{
    int64_t size;
    void* data;
};

struct v8_pair
{
    struct v8_value* first;
    struct v8_value* second;
};

struct v8_value
{
    enum js_types
    {
        // JS               C
        js_undefined,   // void* NULL
        js_boolean,     // bool
        js_null,        // void* NULL
        js_number,      // see specifiers
        js_string,      // const char*
        js_big_int,     // not implemented
        js_symbol,      // not implemented
        js_object       // see specifiers
    } type;

    enum type_specifiers
    {
        not_special,

        // js number
        number,         // double
        int64,          // int64_t
        int32,          // int32_t
        uint32,         // uint32_t

        // js object
        object,         // not implemented
        array,          // v8_array* of v8_value
        map,            // v8_array* of v8_pair
        set,            // v8_array* of v8_value
        function,       // not implemented
        date            // not implemented
    } specifier;

    void* value;
};

struct v8_value v8_new_undefined();
struct v8_value v8_new_boolean(bool value);
struct v8_value v8_new_null();
struct v8_value v8_new_number(double value);
struct v8_value v8_new_integer(int64_t value);
struct v8_value v8_new_string(const char* value, int64_t length);
struct v8_value v8_new_array(int64_t size);
struct v8_value v8_new_set(int64_t size);
struct v8_value v8_new_map(int64_t size);

void v8_delete_value(struct v8_value* value);

double v8_to_double(void* value);

#ifdef __cplusplus
}
#endif
