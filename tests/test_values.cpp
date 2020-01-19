#include <string>
#include <limits>

#include <gtest/gtest.h>

#include "../include/v8capi_values.h"

#include "isolate_fixture.h"

TEST_F(IsolateFixture, UndefinedValue)
{
    v8_value val = v8_new_undefined();

    EXPECT_EQ(v8_get_value_type(val), v8_undefined);

    EXPECT_EQ(v8_is_undefined(val), true);
    EXPECT_EQ(v8_is_boolean(val), false);
    EXPECT_EQ(v8_is_null(val), false);
    EXPECT_EQ(v8_is_number(val), false);
    EXPECT_EQ(v8_is_double(val), false);
    EXPECT_EQ(v8_is_int64(val), false);
    EXPECT_EQ(v8_is_string(val), false);
    EXPECT_EQ(v8_is_array(val), false);
    EXPECT_EQ(v8_is_set(val), false);
    EXPECT_EQ(v8_is_map(val), false);

    v8_delete_value(&val);

    EXPECT_EQ(v8_get_value_type(val), v8_undefined);
    EXPECT_TRUE(v8_is_undefined(val));
}

TEST_F(IsolateFixture, BooleanValue)
{
    {
        v8_value val = v8_new_boolean(true);

        EXPECT_EQ(v8_get_value_type(val), v8_boolean);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), true);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), false);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), false);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_bool(val), true);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

    {
        v8_value val = v8_new_boolean(false);

        EXPECT_EQ(v8_to_bool(val), false);

        v8_delete_value(&val);
    }
}

TEST_F(IsolateFixture, NullValue)
{
    v8_value val = v8_new_null();

    EXPECT_EQ(v8_get_value_type(val), v8_null);

    EXPECT_EQ(v8_is_undefined(val), false);
    EXPECT_EQ(v8_is_boolean(val), false);
    EXPECT_EQ(v8_is_null(val), true);
    EXPECT_EQ(v8_is_number(val), false);
    EXPECT_EQ(v8_is_double(val), false);
    EXPECT_EQ(v8_is_int64(val), false);
    EXPECT_EQ(v8_is_string(val), false);
    EXPECT_EQ(v8_is_array(val), false);
    EXPECT_EQ(v8_is_set(val), false);
    EXPECT_EQ(v8_is_map(val), false);

    v8_delete_value(&val);

    EXPECT_EQ(v8_get_value_type(val), v8_undefined);
    EXPECT_TRUE(v8_is_undefined(val));
}

TEST_F(IsolateFixture, NumberValue)
{
    {
        v8_value val = v8_new_number(1.5);

        EXPECT_EQ(v8_get_value_type(val), v8_number);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), true);
        EXPECT_EQ(v8_is_double(val), true);
        EXPECT_EQ(v8_is_int64(val), false);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_double(val), 1.5);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

    {
        v8_value val = v8_new_integer(0);

        EXPECT_EQ(v8_get_value_type(val), v8_number);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), true);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), true);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_int64(val), 0);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

    {
        v8_value val = v8_new_integer(1);

        EXPECT_EQ(v8_get_value_type(val), v8_number);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), true);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), true);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_int64(val), 1);
    }

    {
        v8_value val = v8_new_integer(-1);

        EXPECT_EQ(v8_get_value_type(val), v8_number);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), true);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), true);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_int64(val), -1);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;

        v8_value val = v8_new_integer(x);

        EXPECT_EQ(v8_get_value_type(val), v8_number);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), true);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), true);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_int64(val), x);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;

        v8_value val = v8_new_integer(x);

        EXPECT_EQ(v8_get_value_type(val), v8_number);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), true);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), true);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        EXPECT_EQ(v8_to_int64(val), x);
    }
}

TEST_F(IsolateFixture, to_double)
{
    {
        v8_value val = v8_new_number(1.5);
        EXPECT_EQ(v8_to_double(val), 1.5);
    }

    {
        v8_value val = v8_new_integer(0);
        EXPECT_EQ(static_cast<uint64_t>(v8_to_double(val)), 0u);
    }

    {
        v8_value val = v8_new_integer(1);
        EXPECT_EQ(static_cast<uint64_t>(v8_to_double(val)), 1u);
    }

    {
        v8_value val = v8_new_integer(-1);
        EXPECT_EQ(static_cast<int32_t>(v8_to_double(val)), -1);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
        v8_value val = v8_new_integer(x);
        EXPECT_EQ(static_cast<int64_t>(v8_to_double(val)), x);
    }

    {
        const auto x = static_cast<int64_t>(std::numeric_limits<int32_t>::min()) - 1;
        v8_value val = v8_new_integer(x);
        EXPECT_EQ(static_cast<int64_t>(v8_to_double(val)), x);
    }
}

TEST_F(IsolateFixture, StringValue)
{
    {
        v8_value val = v8_new_string("", 0);

        EXPECT_EQ(v8_get_value_type(val), v8_string);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), false);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), false);
        EXPECT_EQ(v8_is_string(val), true);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        v8_string_value str = v8_to_string(&val);

        EXPECT_EQ(str.size, 0);
        EXPECT_EQ(*str.data, '\0');

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

    {
        const std::string x = "7654321";

        v8_value val = v8_new_string(x.c_str(), static_cast<int32_t>(x.length()));

        EXPECT_EQ(v8_get_value_type(val), v8_string);
        EXPECT_TRUE(v8_is_string(val));

        v8_string_value str = v8_to_string(&val);

        EXPECT_EQ(static_cast<size_t>(str.size), x.length());
        EXPECT_EQ(str.data, x);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

    {
        const std::string x = "12345678";

        v8_value val = v8_new_string(x.c_str(), static_cast<int32_t>(x.length()));

        EXPECT_EQ(v8_get_value_type(val), v8_string);
        EXPECT_TRUE(v8_is_string(val));

        v8_string_value str = v8_to_string(&val);

        EXPECT_EQ(static_cast<size_t>(str.size), x.length());
        EXPECT_EQ(str.data, x);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

#ifdef NDEBUG
    {
        v8_value val = v8_new_undefined();

        v8_string_value str = v8_to_string(&val);

        EXPECT_EQ(str.size, 0);
        EXPECT_EQ(str.data, nullptr);
    }

    {
        v8_value val = v8_new_string("", -1);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_EQ(v8_is_undefined(val), true);
        EXPECT_EQ(v8_is_string(val), false);
    }
#endif
}

TEST_F(IsolateFixture, ArrayValue)
{
    {
        v8_value val = v8_new_array(1);

        EXPECT_EQ(v8_get_value_type(val), v8_array);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), false);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), false);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), true);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), false);

        v8_array_value arr = v8_to_array(val);

        EXPECT_EQ(arr.size, 1);
        EXPECT_NE(arr.data, nullptr);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

#ifdef NDEBUG
    {
        v8_value val = v8_new_undefined();

        v8_array_value arr = v8_to_array(val);

        EXPECT_EQ(arr.size, 0);
        EXPECT_EQ(arr.data, nullptr);
    }

    {
        v8_value val = v8_new_array(0);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_EQ(v8_is_undefined(val), true);
        EXPECT_EQ(v8_is_array(val), false);
    }
#endif
}

TEST_F(IsolateFixture, SetValue)
{
    {
        v8_value val = v8_new_set(1);

        EXPECT_EQ(v8_get_value_type(val), v8_set);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), false);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), false);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), true);
        EXPECT_EQ(v8_is_map(val), false);

        v8_set_value arr = v8_to_set(val);

        EXPECT_EQ(arr.size, 1);
        EXPECT_NE(arr.data, nullptr);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

#ifdef NDEBUG
    {
        v8_value val = v8_new_undefined();

        v8_set_value arr = v8_to_set(val);

        EXPECT_EQ(arr.size, 0);
        EXPECT_EQ(arr.data, nullptr);
    }

    {
        v8_value val = v8_new_array(0);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_EQ(v8_is_undefined(val), true);
        EXPECT_EQ(v8_is_set(val), false);
    }
#endif
}

TEST_F(IsolateFixture, MapValue)
{
    {
        v8_value val = v8_new_map(1);

        EXPECT_EQ(v8_get_value_type(val), v8_map);

        EXPECT_EQ(v8_is_undefined(val), false);
        EXPECT_EQ(v8_is_boolean(val), false);
        EXPECT_EQ(v8_is_null(val), false);
        EXPECT_EQ(v8_is_number(val), false);
        EXPECT_EQ(v8_is_double(val), false);
        EXPECT_EQ(v8_is_int64(val), false);
        EXPECT_EQ(v8_is_string(val), false);
        EXPECT_EQ(v8_is_array(val), false);
        EXPECT_EQ(v8_is_set(val), false);
        EXPECT_EQ(v8_is_map(val), true);

        v8_map_value arr = v8_to_map(val);

        EXPECT_EQ(arr.size, 1);
        EXPECT_NE(arr.data, nullptr);

        v8_delete_value(&val);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_TRUE(v8_is_undefined(val));
    }

#ifdef NDEBUG
    {
        v8_value val = v8_new_undefined();

        v8_map_value arr = v8_to_map(val);

        EXPECT_EQ(arr.size, 0);
        EXPECT_EQ(arr.data, nullptr);
    }

    {
        v8_value val = v8_new_map(0);

        EXPECT_EQ(v8_get_value_type(val), v8_undefined);
        EXPECT_EQ(v8_is_undefined(val), true);
        EXPECT_EQ(v8_is_map(val), false);
    }
#endif
}
