#include <string>
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

    EXPECT(v8_get_value_type(val) == v8_undefined);

    EXPECT(v8_is_undefined(val) == true);
    EXPECT(v8_is_boolean(val) == false);
    EXPECT(v8_is_null(val) == false);
    EXPECT(v8_is_number(val) == false);
    EXPECT(v8_is_double(val) == false);
    EXPECT(v8_is_int64(val) == false);
    EXPECT(v8_is_string(val) == false);
    EXPECT(v8_is_array(val) == false);
    EXPECT(v8_is_set(val) == false);
    EXPECT(v8_is_map(val) == false);

    v8_delete_value(&val);

    EXPECT(v8_get_value_type(val) == v8_undefined);
    EXPECT(v8_is_undefined(val));
}

SECTION("Test boolean")
{
    {
        v8_value val = v8_new_boolean(true);

        EXPECT(v8_get_value_type(val) == v8_boolean);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == true);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == false);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == false);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_bool(val) == true);

        v8_delete_value(&val);

        EXPECT(v8_get_value_type(val) == v8_undefined);
        EXPECT(v8_is_undefined(val));
    }

    {
        v8_value val = v8_new_boolean(false);

        EXPECT(v8_to_bool(val) == false);

        v8_delete_value(&val);
    }
}

SECTION("Test null")
{
    v8_value val = v8_new_null();

    EXPECT(v8_get_value_type(val) == v8_null);

    EXPECT(v8_is_undefined(val) == false);
    EXPECT(v8_is_boolean(val) == false);
    EXPECT(v8_is_null(val) == true);
    EXPECT(v8_is_number(val) == false);
    EXPECT(v8_is_double(val) == false);
    EXPECT(v8_is_int64(val) == false);
    EXPECT(v8_is_string(val) == false);
    EXPECT(v8_is_array(val) == false);
    EXPECT(v8_is_set(val) == false);
    EXPECT(v8_is_map(val) == false);

    v8_delete_value(&val);

    EXPECT(v8_get_value_type(val) == v8_undefined);
    EXPECT(v8_is_undefined(val));
}

SECTION("Test number")
{
    {
        v8_value val = v8_new_number(1.5);

        EXPECT(v8_get_value_type(val) == v8_number);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == true);
        EXPECT(v8_is_double(val) == true);
        EXPECT(v8_is_int64(val) == false);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_double(val) == 1.5);

        v8_delete_value(&val);

        EXPECT(v8_get_value_type(val) == v8_undefined);
        EXPECT(v8_is_undefined(val));
    }

    {
        v8_value val = v8_new_integer(0);

        EXPECT(v8_get_value_type(val) == v8_number);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == true);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == true);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_int64(val) == 0);

        v8_delete_value(&val);

        EXPECT(v8_get_value_type(val) == v8_undefined);
        EXPECT(v8_is_undefined(val));
    }

    {
        v8_value val = v8_new_integer(1);

        EXPECT(v8_get_value_type(val) == v8_number);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == true);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == true);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_int64(val) == 1);
    }

    {
        v8_value val = v8_new_integer(-1);

        EXPECT(v8_get_value_type(val) == v8_number);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == true);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == true);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_int64(val) == -1);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;

        v8_value val = v8_new_integer(x);

        EXPECT(v8_get_value_type(val) == v8_number);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == true);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == true);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_int64(val) == x);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;

        v8_value val = v8_new_integer(x);

        EXPECT(v8_get_value_type(val) == v8_number);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == true);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == true);
        EXPECT(v8_is_string(val) == false);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(v8_to_int64(val) == x);
    }
}

SECTION("Test to_double")
{
    {
        v8_value val = v8_new_number(1.5);
        EXPECT(v8_to_double(val) == 1.5);
    }

    {
        v8_value val = v8_new_integer(0);
        EXPECT((uint64_t)v8_to_double(val) == 0u);
    }

    {
        v8_value val = v8_new_integer(1);
        EXPECT((uint64_t)v8_to_double(val) == 1u);
    }

    {
        v8_value val = v8_new_integer(-1);
        EXPECT((int32_t)v8_to_double(val) == -1);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
        v8_value val = v8_new_integer(x);
        EXPECT((int64_t)v8_to_double(val) == x);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;
        v8_value val = v8_new_integer(x);
        EXPECT((int64_t)v8_to_double(val) == x);
    }
}
SECTION("Test string")
{
    {
        v8_value val = v8_new_string("", 0);

        EXPECT(v8_get_value_type(val) == v8_string);

        EXPECT(v8_is_undefined(val) == false);
        EXPECT(v8_is_boolean(val) == false);
        EXPECT(v8_is_null(val) == false);
        EXPECT(v8_is_number(val) == false);
        EXPECT(v8_is_double(val) == false);
        EXPECT(v8_is_int64(val) == false);
        EXPECT(v8_is_string(val) == true);
        EXPECT(v8_is_array(val) == false);
        EXPECT(v8_is_set(val) == false);
        EXPECT(v8_is_map(val) == false);

        EXPECT(*v8_to_string(&val) == '\0');

        v8_delete_value(&val);

        EXPECT(v8_get_value_type(val) == v8_undefined);
        EXPECT(v8_is_undefined(val));
    }

    {
        const std::string x = "7654321";

        v8_value val = v8_new_string(x.c_str(), static_cast<int32_t>(x.length()));

        EXPECT(v8_get_value_type(val) == v8_string);
        EXPECT(v8_is_string(val));
        EXPECT(v8_to_string(&val) == x);

        v8_delete_value(&val);

        EXPECT(v8_get_value_type(val) == v8_undefined);
        EXPECT(v8_is_undefined(val));
    }

    {
        const std::string x = "12345678";

        v8_value val = v8_new_string(x.c_str(), static_cast<int32_t>(x.length()));

        EXPECT(v8_get_value_type(val) == v8_string);
        EXPECT(v8_is_string(val));
        EXPECT(v8_to_string(&val) == x);

        v8_delete_value(&val);

        EXPECT(v8_get_value_type(val) == v8_undefined);
        EXPECT(v8_is_undefined(val));
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
