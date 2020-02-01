#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>

#include <v8.h>

#include "v8capi_value_helpers.h"

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

template <class T>
v8_value make_sequense(int32_t size, js_types type)
{
    assert(size >= 0);

    if (size < 0)
    {
        return v8_new_undefined();
    }

    v8_value_impl val_impl =
    {
        size == 0
            ? nullptr
            : new T[size],
        type,
        type_specifiers::not_special,
        size
    };

    return to_value(val_impl);
}

v8_value v8_new_object(int32_t size)
{
    return make_sequense<v8_pair_value>(size, js_types::object);
}

v8_value v8_new_array(int32_t size)
{
    return make_sequense<v8_value>(size, js_types::array);
}

v8_value v8_new_set(int32_t size)
{
    return make_sequense<v8_value>(size, js_types::set);
}

v8_value v8_new_map(int32_t size)
{
    return make_sequense<v8_pair_value>(size, js_types::map);
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

bool v8_is_integer(v8_value value)
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

bool v8_is_object(v8_value value)
{
    const auto val_impl = to_value_impl(value);
    return val_impl.type == js_types::object;
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

double get_double(void* data)
{
    double val;
    std::memcpy(&val, &data, sizeof(double));
    return val;
}

double v8_to_double(v8_value value)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == js_types::number);

    if (val_impl.type != js_types::number)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (val_impl.specifier == type_specifiers::number)
    {
        return get_double(val_impl.data);
    }

    return static_cast<double>(reinterpret_cast<int64_t>(val_impl.data));
}

int32_t v8_to_int32(v8_value value)
{
    return static_cast<int32_t>(v8_to_int64(value));
}

uint32_t v8_to_uint32(v8_value value)
{
    return static_cast<uint32_t>(v8_to_int64(value));
}

int64_t v8_to_int64(v8_value value)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == js_types::number);

    if (val_impl.type != js_types::number)
    {
        return 0;
    }

    if (val_impl.specifier != type_specifiers::number)
    {
        return reinterpret_cast<int64_t>(value.data);
    }

    return static_cast<int64_t>(get_double(val_impl.data));
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

template <class T>
T to_sequense(v8_value value, js_types type)
{
    const auto val_impl = to_value_impl(value);

    assert(val_impl.type == type);

    if (val_impl.type != type)
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
        static_cast<decltype(T().data)>(val_impl.data)
    };
}

v8_object_value v8_to_object(v8_value value)
{
    return to_sequense<v8_object_value>(value, js_types::object);
}

v8_array_value v8_to_array(v8_value value)
{
    return to_sequense<v8_array_value>(value, js_types::array);
}

v8_set_value v8_to_set(v8_value value)
{
    return to_sequense<v8_set_value>(value, js_types::set);
}

v8_map_value v8_to_map(v8_value value)
{
    return to_sequense<v8_map_value>(value, js_types::map);
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
    case js_types::boolean: // fallthrough
    case js_types::null:    // fallthrough
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
        for (int i = 0; i < val_impl.size; ++i)
        {
            auto pair = static_cast<v8_pair_value*>(val_impl.data)[i];
            v8_delete_value(&pair.first);
            v8_delete_value(&pair.second);
        }
        delete[] static_cast<v8_pair_value*>(val_impl.data);
        set_undefined(value);
        return;
    case js_types::array:  // fallthrough
    case js_types::set:
        for (int i = 0; i < val_impl.size; ++i)
        {
            v8_delete_value(&static_cast<v8_value*>(val_impl.data)[i]);
        }
        delete[] static_cast<v8_value*>(val_impl.data);
        set_undefined(value);
        return;
    case js_types::map:
        for (int i = 0; i < val_impl.size; ++i)
        {
            auto pair = &static_cast<v8_pair_value*>(val_impl.data)[i];
            v8_delete_value(&pair->first);
            v8_delete_value(&pair->second);
        }
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

v8_value from_v8_value(v8::Local<v8::Context> context, v8::Local<v8::Value> value)
{
    if (value->IsNullOrUndefined())
    {
        return value->IsUndefined()
            ? v8_new_undefined()
            : v8_new_null();
    }

    if (value->IsNumber())
    {
        if (value->IsUint32() || value->IsInt32())
        {
            v8::Integer* integer = v8::Integer::Cast(*value);
            return v8_new_integer(integer->Value());
        }

        double number;
        if (value->NumberValue(context).To(&number))
        {
            return v8_new_number(number);
        }

        assert(!"invalid conversion");
        return v8_new_undefined();
    }

    if (value->IsBoolean())
    {
        return v8_new_boolean(value->BooleanValue(context->GetIsolate()));
    }

    if (value->IsString())
    {
        v8::String::Utf8Value utf8(context->GetIsolate(), value);
        return v8_new_string(*utf8, utf8.length());
    }

    if (value->IsArray())
    {
        v8::Array* arr = v8::Array::Cast(*value);
        const int length = arr->Length();

        v8_value res = v8_new_array(length);
        auto data = static_cast<v8_value*>(res.data);

        for (int i = 0; i < length; ++i)
        {
            v8::Local<v8::Value> elem;

            if (!arr->Get(context, i).ToLocal(&elem))
            {
                v8_delete_value(&res);
                assert(!"invalid conversion");
                return v8_new_undefined();
            }

            data[i] = from_v8_value(context, elem);
        }

        return res;
    }

    if (value->IsSet())
    {
        v8::Set* set = v8::Set::Cast(*value);

        v8::Local<v8::Array> arr = set->AsArray();
        const int length = arr->Length();

        v8_value res = v8_new_set(length);
        auto data = static_cast<v8_value*>(res.data);

        for (int i = 0; i < length; ++i)
        {
            v8::Local<v8::Value> elem;

            if (!arr->Get(context, i).ToLocal(&elem))
            {
                v8_delete_value(&res);
                assert(!"invalid conversion");
                return v8_new_undefined();
            }

            data[i] = from_v8_value(context, elem);
        }

        return res;
    }

    if (value->IsMap())
    {
        v8::Map* map = v8::Map::Cast(*value);

        v8::Local<v8::Array> arr = map->AsArray();
        const int length = arr->Length();

        v8_value res = v8_new_map(length / 2);
        auto data = static_cast<v8_pair_value*>(res.data);

        for (int i = 0; i < length; i += 2)
        {
            v8::Local<v8::Value> k;
            if (!arr->Get(context, i).ToLocal(&k))
            {
                v8_delete_value(&res);
                assert(!"invalid conversion");
                return v8_new_undefined();
            }

            v8::Local<v8::Value> v;
            if (!arr->Get(context, i + 1).ToLocal(&v))
            {
                v8_delete_value(&res);
                assert(!"invalid conversion");
                return v8_new_undefined();
            }

            const auto index = i / 2;

            data[index].first = from_v8_value(context, k);
            data[index].second = from_v8_value(context, v);
        }

        return res;
    }

    if (value->IsObject())
    {
        v8::Object* obj = v8::Object::Cast(*value);

        v8::Local<v8::Array> names;
        if (!obj->GetOwnPropertyNames(context).ToLocal(&names))
        {
            assert(!"invalid conversion");
            return v8_new_undefined();
        }

        const auto length = names->Length();

        v8_value res = v8_new_object(length);
        auto data = static_cast<v8_pair_value*>(res.data);

        for (uint32_t i = 0; i < length; ++i)
        {
            v8::Local<v8::Value> k;
            if (!names->Get(context, i).ToLocal(&k))
            {
                v8_delete_value(&res);
                assert(!"invalid conversion");
                return v8_new_undefined();
            }

            v8::Local<v8::Value> v;
            if (!obj->Get(context, k).ToLocal(&v))
            {
                v8_delete_value(&res);
                assert(!"invalid conversion");
                return v8_new_undefined();
            }

            data[i].first = from_v8_value(context, k);
            data[i].second = from_v8_value(context, v);
        }

        return res;
    }

    assert(!"Not implemented type");
    return v8_new_undefined();
}

v8::Local<v8::Value> to_v8_value(v8::Local<v8::Context> context, v8_value value)
{
    v8::Isolate* isolate = context->GetIsolate();

    v8::EscapableHandleScope handle_scope(isolate);

    v8::Local<v8::Value> result;

    const auto val_impl = to_value_impl(value);

    switch (val_impl.type)
    {
    case js_types::undefined:
        return handle_scope.Escape(v8::Undefined(isolate));
    case js_types::boolean:
        return v8_to_bool(value)
            ? handle_scope.Escape(v8::True(isolate))
            : handle_scope.Escape(v8::False(isolate));
    case js_types::null:
        return handle_scope.Escape(v8::Null(isolate));
    case js_types::number:
        switch (val_impl.specifier)
        {
        case type_specifiers::not_special:
            assert(!"Invalid value");
            return handle_scope.Escape(v8::Undefined(isolate));
        case type_specifiers::number:
            return handle_scope.Escape(v8::Number::New(isolate, v8_to_double(value)));
        case type_specifiers::int64:
            // TODO maybe BigInt?
            return handle_scope.Escape(v8::Number::New(isolate, v8_to_double(value)));
        case type_specifiers::int32:
            return handle_scope.Escape(v8::Integer::New(isolate, v8_to_int32(value)));
        case type_specifiers::uint32:
            return handle_scope.Escape(v8::Integer::NewFromUnsigned(isolate, v8_to_uint32(value)));
        }
        break;
    case js_types::string:
    {
        const auto str = v8_to_string(&value);
        v8::Local<v8::String> val;
        if (!v8::String::NewFromUtf8(
            isolate, str.data, v8::NewStringType::kNormal, str.size).ToLocal(&val))
        {
            assert(!"Invalid string conversion");
            return handle_scope.Escape(v8::Undefined(isolate));
        }
        return handle_scope.Escape(val);
    }
    case js_types::big_int:
        assert(!"not implemented");
        break;
    case js_types::symbol:
        assert(!"not implemented");
        break;
    case js_types::object:
    {
        const auto obj = v8_to_object(value);
        v8::Local<v8::Object> val = v8::Object::New(isolate);
        for (int i = 0; i < obj.size; ++i)
        {
            const auto str = v8_to_string(&obj.data[i].first);

            v8::Local<v8::String> name;
            if (!v8::String::NewFromUtf8(
                isolate, str.data, v8::NewStringType::kNormal, str.size).ToLocal(&name))
            {
                assert(!"Invalid string conversion");
                return handle_scope.Escape(v8::Undefined(isolate));
            }

            if (!val->CreateDataProperty(context,
                name,
                to_v8_value(context, obj.data[i].second)).ToChecked())
            {
                assert(!"Failed to create object property");
                return handle_scope.Escape(v8::Undefined(isolate));
            }
        }
        return handle_scope.Escape(val);
    }
    case js_types::array:
    {
        const auto arr = v8_to_array(value);
        v8::Local<v8::Array> val = v8::Array::New(isolate, arr.size);
        for (int i = 0; i < arr.size; ++i)
        {
            if (!val->Set(context, i, to_v8_value(context, arr.data[i])).ToChecked())
            {
                assert(!"Failed to add array element");
                return handle_scope.Escape(v8::Undefined(isolate));
            }
        }
        return handle_scope.Escape(val);
    }
    case js_types::set:
    {
        const auto set = v8_to_set(value);
        v8::Local<v8::Set> val = v8::Set::New(isolate);
        for (int i = 0; i < set.size; ++i)
        {
            v8::MaybeLocal<v8::Set> tmp =
                val->Add(context, to_v8_value(context, set.data[i]));
            if (!tmp.ToLocal(&val))
            {
                assert(!"Failed to insert into set");
                return handle_scope.Escape(v8::Undefined(isolate));
            }
        }
        return handle_scope.Escape(val);
    }
    case js_types::map:
    {
        const auto map = v8_to_map(value);
        v8::Local<v8::Map> val = v8::Map::New(isolate);
        for (int i = 0; i < map.size; ++i)
        {
            v8::MaybeLocal<v8::Map> tmp = val->Set(context,
                to_v8_value(context, map.data[i].first),
                to_v8_value(context, map.data[i].second));
            if (!tmp.ToLocal(&val))
            {
                assert(!"Failed to insert into map");
                return handle_scope.Escape(v8::Undefined(isolate));
            }
        }
        return handle_scope.Escape(val);
    }
    case js_types::function:
        assert(!"not implemented");
        break;
    case js_types::date:
        assert(!"not implemented");
        break;
    }

    return handle_scope.Escape(v8::Undefined(isolate));
}
