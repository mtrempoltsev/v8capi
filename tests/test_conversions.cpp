﻿#include <gtest/gtest.h>

#include "utils.h"

#include "../include/v8capi.h"

#include "../src/v8capi_value_helpers.h"

#include "isolate_fixture.h"

TEST_F(IsolateFixture, PrimitivesConversion)
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("primitives.js").c_str(), "my.js", &err);

    ASSERT_NE(script, nullptr);

    v8_value res;

    const bool ok = v8_run_script(script, &res, &err);

    ASSERT_TRUE(ok);

    ASSERT_TRUE(v8_is_object(res));

    v8_object_value obj = v8_to_object(res);

    ASSERT_EQ(obj.size, 9);

    EXPECT_TRUE(v8_is_string(obj.data[0].first));
    EXPECT_STREQ(v8_to_string(&obj.data[0].first).data, "u");
    EXPECT_TRUE(v8_is_undefined(obj.data[0].second));

    EXPECT_TRUE(v8_is_string(obj.data[1].first));
    EXPECT_STREQ(v8_to_string(&obj.data[1].first).data, "b");
    EXPECT_TRUE(v8_is_boolean(obj.data[1].second));
    EXPECT_EQ(v8_to_bool(obj.data[1].second), true);

    EXPECT_TRUE(v8_is_string(obj.data[2].first));
    EXPECT_STREQ(v8_to_string(&obj.data[2].first).data, "n");
    EXPECT_TRUE(v8_is_null(obj.data[2].second));

    EXPECT_TRUE(v8_is_string(obj.data[3].first));
    EXPECT_STREQ(v8_to_string(&obj.data[3].first).data, "d1");
    EXPECT_TRUE(v8_is_double(obj.data[3].second));
    EXPECT_EQ(v8_to_double(obj.data[3].second), 1.5);

    EXPECT_TRUE(v8_is_string(obj.data[4].first));
    EXPECT_STREQ(v8_to_string(&obj.data[4].first).data, "d2");
    EXPECT_TRUE(v8_is_integer(obj.data[4].second));
    EXPECT_EQ(v8_to_int32(obj.data[4].second), -1);

    EXPECT_TRUE(v8_is_string(obj.data[5].first));
    EXPECT_STREQ(v8_to_string(&obj.data[5].first).data, "d3");
    EXPECT_TRUE(v8_is_integer(obj.data[5].second));
    EXPECT_EQ(v8_to_uint32(obj.data[5].second), 1u);

    EXPECT_TRUE(v8_is_string(obj.data[6].first));
    EXPECT_STREQ(v8_to_string(&obj.data[6].first).data, "d4");
    EXPECT_TRUE(v8_is_double(obj.data[6].second));
    EXPECT_EQ(v8_to_int64(obj.data[6].second), 9007199254740992);

    EXPECT_TRUE(v8_is_string(obj.data[7].first));
    EXPECT_STREQ(v8_to_string(&obj.data[7].first).data, "s1");
    EXPECT_TRUE(v8_is_string(obj.data[7].second));
    EXPECT_STREQ(v8_to_string(&obj.data[7].second).data, "short");

    EXPECT_TRUE(v8_is_string(obj.data[8].first));
    EXPECT_STREQ(v8_to_string(&obj.data[8].first).data, "s2");
    EXPECT_TRUE(v8_is_string(obj.data[8].second));
    EXPECT_STREQ(v8_to_string(&obj.data[8].second).data, "long long long long long long long long long long long string");

    v8_delete_value(&res);

    v8_delete_value(&res);
    v8_delete_error(&err);
    v8_delete_script(script);
}