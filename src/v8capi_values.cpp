#include <cstring>
#include <limits>

#include "../include/v8capi_values.h"

v8_value v8_new_undefined()
{
    return
    {
        v8_value::js_undefined,
        v8_value::not_special,
        nullptr
    };
}

v8_value v8_new_boolean(bool value)
{
    return
    {
        v8_value::js_boolean,
        v8_value::not_special,
        reinterpret_cast<void*>(value)
    };
}

v8_value v8_new_null()
{
    return
    {
        v8_value::js_null,
        v8_value::not_special,
        nullptr
    };
}

v8_value v8_new_number(double value)
{
    v8_value val;
    val.type = v8_value::js_number;
    val.specifier = v8_value::number;
    std::memcpy(&val.value, &value, sizeof(void*));
    return val;
}

v8_value v8_new_integer(int64_t value)
{
    v8_value val;
    val.type = v8_value::js_number;
    if (value >= 0)
    {
        val.specifier = 
            value <= std::numeric_limits<int32_t>::max()
                ? v8_value::uint32
                : v8_value::int64;
    }
    else
    {
        val.specifier = 
            value < std::numeric_limits<int32_t>::min()
                ? v8_value::int64
                : v8_value::int32;
    }
    val.value = reinterpret_cast<void*>(value);
    return val;
}

/*
v8_value v8_new_string(const char* value, int64_t length);
v8_value v8_new_array(int64_t size);
v8_value v8_new_set(int64_t size);
v8_value v8_new_map(int64_t size);
*/

void set_undefined(v8_value* value)
{
    value->type = v8_value::js_undefined;
    value->specifier = v8_value::not_special;
    value->value = nullptr;
}

void v8_delete_value(v8_value* value)
{
    if (!value)
    {
        return;
    }

    switch (value->type)
    {
    case v8_value::js_undefined:
        return;
    case v8_value::js_boolean:
    case v8_value::js_null:
    case v8_value::js_number:
        set_undefined(value);
        return;
    }
}

double v8_to_double(void* value)
{
    double val;
    std::memcpy(&val, &value, sizeof(double));
    return val;
}
