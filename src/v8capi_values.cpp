#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>

#include "../include/v8capi_values.h"

enum class js_types : int16_t
{
    undefined   = v8_undefined,
    boolean     = v8_boolean,
    null        = v8_null,
    number      = v8_number,
    string      = v8_string,
    big_int     = v8_big_int,
    symbol      = v8_symbol,
    object      = v8_object,
    array       = v8_array,
    set         = v8_set,
    map         = v8_map,
    function    = v8_function,
    date        = v8_date
};

enum class type_specifiers : int16_t
{
    not_special     = 0,

    // js number
    number,         // double
    int64,          // int64_t
    int32,          // int32_t
    uint32,         // uint32_t
};

struct v8_value_impl
{
    void* data;
    js_types type;
    type_specifiers specifier;
    int32_t size;
};

static_assert(sizeof(v8_value_impl) == 16);
static_assert(sizeof(v8_value) == sizeof(v8_value_impl));

static_assert(offsetof(v8_value_impl, data) == 0);
static_assert(offsetof(v8_value_impl, type) == 8);
static_assert(offsetof(v8_value_impl, specifier) == 10);
static_assert(offsetof(v8_value_impl, size) == 12);

v8_value to_value(v8_value_impl val_impl)
{
    v8_value val;
    std::memcpy(&val, &val_impl, sizeof(v8_value));
    return val;
}

v8_value_impl to_value_impl(v8_value val)
{
    v8_value_impl val_impl;
    std::memcpy(&val_impl, &val, sizeof(v8_value));
    return val_impl;
}

v8_value v8_new_undefined()
{
    v8_value_impl val_impl = 
    {
        nullptr,
        js_types::undefined,
        type_specifiers::not_special,
        0
    };
    return to_value(val_impl);
}

v8_value v8_new_boolean(bool value)
{
    v8_value_impl val_impl =
    {
        reinterpret_cast<void*>(value),
        js_types::boolean,
        type_specifiers::not_special,
        0
    };
    return to_value(val_impl);
}

v8_value v8_new_null()
{
    v8_value_impl val_impl =
    {
        nullptr,
        js_types::null,
        type_specifiers::not_special,
        0
    };
    return to_value(val_impl);
}

v8_value v8_new_number(double value)
{
    v8_value_impl val_impl =
    {
        nullptr,
        js_types::number,
        type_specifiers::number,
        0
    };
    std::memcpy(&val_impl.data, &value, sizeof(void*));
    return to_value(val_impl);
}

v8_value v8_new_integer(int64_t value)
{
    v8_value_impl val_impl =
    {
        reinterpret_cast<void*>(value),
        js_types::number,
        value >= 0
            ? value <= std::numeric_limits<int32_t>::max()
                ? type_specifiers::uint32
                : type_specifiers::int64
            : value < std::numeric_limits<int32_t>::min()
                ? type_specifiers::int64
                : type_specifiers::int32,
        0
    };
    return to_value(val_impl);
}

v8_value v8_new_string(const char* value, int32_t length)
{
    assert(value);
    assert(length >= 0);

    if (!value || length < 0)
    {
        return v8_new_undefined();
    }

    v8_value_impl val_impl =
    {
        nullptr,
        js_types::string,
        type_specifiers::not_special,
        length
    };

    const size_t size = static_cast<size_t>(length) + 1;

    if (size <= sizeof(void*))
    {
        std::memcpy(&val_impl.data, value, size);
    }
    else
    {
        val_impl.data = new char[size];
        std::memcpy(val_impl.data, value, size);
    }

    return to_value(val_impl);
}

v8_value v8_new_array(int32_t size)
{
    assert(size > 0);

    if (size <= 0)
    {
        return v8_new_undefined();
    }

    v8_value_impl val_impl =
    {
        new v8_value[size],
        js_types::array,
        type_specifiers::not_special,
        size
    };

    return to_value(val_impl);
}

v8_value v8_new_set(int32_t size)
{
    assert(size > 0);

    if (size <= 0)
    {
        return v8_new_undefined();
    }

    v8_value_impl val_impl =
    {
        new v8_value[size],
        js_types::set,
        type_specifiers::not_special,
        size
    };

    return to_value(val_impl);
}

v8_value v8_new_map(int32_t size)
{
    assert(size > 0);

    if (size <= 0)
    {
        return v8_new_undefined();
    }

    v8_value_impl val_impl =
    {
        new v8_pair_value[size],
        js_types::map,
        type_specifiers::not_special,
        size
    };

    return to_value(val_impl);
}

bool v8_is_undefined(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::undefined;
}

bool v8_is_boolean(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::boolean;
}

bool v8_is_null(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::null;
}

bool v8_is_number(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::number;
}

bool v8_is_double(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.specifier == type_specifiers::number;
}

bool v8_is_int64(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::number
        && val_impl.specifier != type_specifiers::number;
}

bool v8_is_string(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::string;
}

bool v8_is_array(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::array;
}

bool v8_is_set(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::set;
}

bool v8_is_map(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::map;
}

int v8_get_value_type(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return static_cast<int>(val_impl.type);
}

bool v8_to_bool(v8_value value)
{
    return value.data;
}

double v8_to_double(v8_value value)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == js_types::number);

    if (val_impl.type == js_types::number)
    {
        std::numeric_limits<double>::quiet_NaN();
    }

    if (val_impl.specifier == type_specifiers::number)
    {
        double val;
        std::memcpy(&val, &val_impl.data, sizeof(double));
        return val;
    }

    return static_cast<double>(reinterpret_cast<int64_t>(val_impl.data));
}

int64_t v8_to_int64(v8_value value)
{
    return reinterpret_cast<int64_t>(value.data);
}

v8_string_value v8_to_string(v8_value* value)
{
    assert(value);

    if (!value)
    {
        return
        {
            0,
            nullptr
        };
    }

    const auto val_impl = to_value_impl(*value);

    assert(val_impl.type == js_types::string);

    if (val_impl.type != js_types::string)
    {
        return
        {
            0,
            nullptr
        };
    }

    return
    {
        val_impl.size,
        static_cast<size_t>(val_impl.size) < sizeof(void*)
            ? reinterpret_cast<const char*>(&value->data)
            : static_cast<const char*>(value->data)
    };
}

v8_array_value v8_to_array(struct v8_value value)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == js_types::array);

    if (val_impl.type != js_types::array)
    {
        return
        {
            0,
            nullptr
        };
    }

    return
    {
        val_impl.size,
        static_cast<v8_value*>(val_impl.data)
    };
}

v8_set_value v8_to_set(struct v8_value value)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == js_types::set);

    if (val_impl.type != js_types::set)
    {
        return
        {
            0,
            nullptr
        };
    }

    return
    {
        val_impl.size,
        static_cast<v8_value*>(val_impl.data)
    };
}

v8_map_value v8_to_map(struct v8_value value)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == js_types::map);

    if (val_impl.type != js_types::map)
    {
        return
        {
            0,
            nullptr
        };
    }

    return
    {
        val_impl.size,
        static_cast<v8_pair_value*>(val_impl.data)
    };
}

void set_undefined(v8_value* value)
{
    std::memset(value, 0, sizeof(v8_value));
}

void v8_delete_value(v8_value* value)
{
    assert(value);

    if (!value)
    {
        return;
    }

    const auto val_impl = to_value_impl(*value);

    switch (val_impl.type)
    {
    case js_types::undefined:
        return;
    case js_types::boolean:
    case js_types::null:
    case js_types::number:
        set_undefined(value);
        return;
    case js_types::string:
        if (static_cast<size_t>(val_impl.size) >= sizeof(void*))
        {
            delete[] static_cast<char*>(val_impl.data);
        }
        set_undefined(value);
        return;
    case js_types::big_int:
        assert(!"not implemented");
        return;
    case js_types::symbol:
        assert(!"not implemented");
        return;
    case js_types::object:
        assert(!"not implemented");
        return;
    case js_types::array:
    case js_types::set:
        delete[] static_cast<v8_value*>(val_impl.data);
        set_undefined(value);
        return;
    case js_types::map:
        delete[] static_cast<v8_pair_value*>(val_impl.data);
        set_undefined(value);
        return;
    case js_types::function:
        assert(!"not implemented");
        return;
    case js_types::date:
        assert(!"not implemented");
        return;
    }
}
