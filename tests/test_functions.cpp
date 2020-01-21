#include <gtest/gtest.h>

#include "utils.h"

#include "../include/v8capi.h"

#include "isolate_fixture.h"

TEST_F(IsolateFixture, SimpleFunction)
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("sum.js").c_str(), "my.js", &err);

    ASSERT_NE(script, nullptr);

    v8_value res;

    bool ok = v8_run_script(script, &res, &err);

    v8_delete_value(&res);

    ASSERT_TRUE(ok);

    v8_callable* sum = v8_get_function(script, "sum");

    ASSERT_NE(sum, nullptr);

    v8_value args[] =
    {
        v8_new_integer(2),
        v8_new_integer(2)
    };

    ok = v8_call_function(sum, 2, args, &res, &err);

    ASSERT_TRUE(ok);

    EXPECT_EQ(v8_is_integer(res), true);
    EXPECT_EQ(v8_to_int32(res), 4);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_delete_value(&res);
    v8_delete_error(&err);
    v8_delete_function(sum);
    v8_delete_script(script);
}
