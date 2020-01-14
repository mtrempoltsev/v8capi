﻿#include <string>
#include <limits>

#include "lest.hpp"

#include "../include/v8capi_values.h"

const lest::test specification[] =
{
CASE("Test values")
{
SETUP("Empty setup")
{
SECTION("Test undefined")
{
    v8_value val = v8_new_undefined();

    EXPECT(val.type == v8_value::js_undefined);
    EXPECT(val.specifier == v8_value::not_special);
    EXPECT(val.value == nullptr);

    v8_delete_value(&val);

    EXPECT(val.type == v8_value::js_undefined);
    EXPECT(val.specifier == v8_value::not_special);
    EXPECT(val.value == nullptr);
}

SECTION("Test boolean")
{
    {
        v8_value val = v8_new_boolean(true);

        EXPECT(val.type == v8_value::js_boolean);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT((bool)val.value == true);

        v8_delete_value(&val);

        EXPECT(val.type == v8_value::js_undefined);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT(val.value == nullptr);
    }

    {
        v8_value val = v8_new_boolean(false);

        EXPECT((bool)val.value == false);

        v8_delete_value(&val);
    }
}

SECTION("Test null")
{
    v8_value val = v8_new_null();

    EXPECT(val.type == v8_value::js_null);
    EXPECT(val.specifier == v8_value::not_special);
    EXPECT(val.value == nullptr);

    v8_delete_value(&val);

    EXPECT(val.type == v8_value::js_undefined);
    EXPECT(val.specifier == v8_value::not_special);
    EXPECT(val.value == nullptr);
}

SECTION("Test number")
{
    {
        v8_value val = v8_new_number(1.5);

        EXPECT(val.type == v8_value::js_number);
        EXPECT(val.specifier == v8_value::number);
        EXPECT(v8_to_double(&val) == 1.5);

        v8_delete_value(&val);

        EXPECT(val.type == v8_value::js_undefined);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT(val.value == nullptr);
    }

    {
        v8_value val = v8_new_integer(0);

        EXPECT(val.type == v8_value::js_number);
        EXPECT(val.specifier == v8_value::uint32);
        EXPECT((uint32_t)(int64_t)val.value == 0u);

        v8_delete_value(&val);

        EXPECT(val.type == v8_value::js_undefined);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT(val.value == nullptr);
    }

    {
        v8_value val = v8_new_integer(1);

        EXPECT(val.type == v8_value::js_number);
        EXPECT(val.specifier == v8_value::uint32);
        EXPECT((uint32_t)(int64_t)val.value == 1u);
    }

    {
        v8_value val = v8_new_integer(-1);

        EXPECT(val.type == v8_value::js_number);
        EXPECT(val.specifier == v8_value::int32);
        EXPECT((int32_t)(int64_t)val.value == -1);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;

        v8_value val = v8_new_integer(x);

        EXPECT(val.type == v8_value::js_number);
        EXPECT(val.specifier == v8_value::int64);
        EXPECT((int64_t)val.value == x);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;

        v8_value val = v8_new_integer(x);

        EXPECT(val.type == v8_value::js_number);
        EXPECT(val.specifier == v8_value::int64);
        EXPECT((int64_t)val.value == x);
    }
}

SECTION("Test to_number")
{
    {
        v8_value val = v8_new_number(1.5);
        EXPECT(v8_to_double(&val) == 1.5);
    }

    {
        v8_value val = v8_new_integer(0);
        EXPECT((uint64_t)v8_to_double(&val) == 0u);
    }

    {
        v8_value val = v8_new_integer(1);
        EXPECT((uint64_t)v8_to_double(&val) == 1u);
    }

    {
        v8_value val = v8_new_integer(-1);
        EXPECT((int32_t)v8_to_double(&val) == -1);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
        v8_value val = v8_new_integer(x);
        EXPECT((int64_t)v8_to_double(&val) == x);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;
        v8_value val = v8_new_integer(x);
        EXPECT((int64_t)v8_to_double(&val) == x);
    }
}

SECTION("Test string")
{
    {
        v8_value val = v8_new_string("", 0);

        EXPECT(val.type == v8_value::js_string);
        EXPECT(val.specifier == v8_value::small_string);
        EXPECT(*v8_to_string(&val) == '\0');

        v8_delete_value(&val);

        EXPECT(val.type == v8_value::js_undefined);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT(val.value == nullptr);
    }

    {
        const std::string x = "7654321";

        v8_value val = v8_new_string(x.c_str(), x.length());

        EXPECT(val.type == v8_value::js_string);
        EXPECT(val.specifier == v8_value::small_string);
        EXPECT(v8_to_string(&val) == x);

        v8_delete_value(&val);

        EXPECT(val.type == v8_value::js_undefined);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT(val.value == nullptr);
    }

    {
        const std::string x = "12345678";

        v8_value val = v8_new_string(x.c_str(), x.length());

        EXPECT(val.type == v8_value::js_string);
        EXPECT(val.specifier == v8_value::string);
        EXPECT(v8_to_string(&val) == x);

        v8_delete_value(&val);

        EXPECT(val.type == v8_value::js_undefined);
        EXPECT(val.specifier == v8_value::not_special);
        EXPECT(val.value == nullptr);
    }
}
}
}
};

int main(int argc, char* argv[])
{
    const int result = lest::run(specification, argc, argv);
    return result;
}
