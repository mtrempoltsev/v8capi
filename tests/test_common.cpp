#include <chrono>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "utils.h"

#include "../include/v8capi.h"

#include "isolate_fixture.h"

TEST_F(IsolateFixture, CompilationWithErrors)
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("syntax_error.js").c_str(), "my.js", &err);

    ASSERT_EQ(script, nullptr);

    EXPECT_EQ(err.line_number, 1);
    EXPECT_STREQ(err.location, "my.js");
    EXPECT_STREQ(err.message, "SyntaxError: Invalid or unexpected token");
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_STREQ(err.wavy_underline, "﻿x = 3'\n      ^");

    v8_delete_error(&err);
}

TEST_F(IsolateFixture, RuntimeErrors)
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("throw.js").c_str(), "my.js", &err);

    ASSERT_NE(script, nullptr);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_value res;

    const bool ok = v8_run_script(script, &res, &err);

    EXPECT_FALSE(ok);

    EXPECT_EQ(err.line_number, 2);
    EXPECT_STREQ(err.location, "my.js");
    EXPECT_STREQ(err.message, "my_err");
    EXPECT_STREQ(err.stack_trace, "    test                 @ my.js:2\n    (anonymous function) @ my.js:5");
    EXPECT_STREQ(err.wavy_underline, "    throw 'my_err'\n    ^");

    v8_delete_error(&err);
    v8_delete_script(script);
}

TEST_F(IsolateFixture, NormalExecution)
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("good_script.js").c_str(), "my.js", &err);

    ASSERT_NE(script, nullptr);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_value res;

    const bool ok = v8_run_script(script, &res, &err);

    EXPECT_TRUE(ok);

    EXPECT_EQ(v8_is_int64(res), true);
    EXPECT_EQ(v8_to_int64(res), 4);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_delete_value(&res);
    v8_delete_error(&err);
    v8_delete_script(script);
}

TEST_F(IsolateFixture, ScriptTermination)
{
    v8_error err;

    v8_script* s1 =
        v8_compile_script(vm, read_file("infinite_loop.js").c_str(), "my.js", &err);

    ASSERT_NE(s1, nullptr);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_script* s2 =
        v8_compile_script(vm, read_file("good_script.js").c_str(), "my.js", &err);

    v8_value res;

    v8_run_script(s2, &res, &err);

    ASSERT_NE(s2, nullptr);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_delete_value(&res);
    v8_delete_error(&err);

    std::thread killer(
        [s1]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            v8_terminate_script(s1);
        });

    bool ok = v8_run_script(s1, &res, &err);

    killer.join();

    EXPECT_FALSE(ok);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_STREQ(err.message, "Script execution terminated");
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_delete_value(&res);
    v8_delete_error(&err);

    ok = v8_run_script(s2, &res, &err);

    EXPECT_TRUE(ok);

    EXPECT_EQ(err.line_number, 0);
    EXPECT_EQ(err.location, nullptr);
    EXPECT_EQ(err.message, nullptr);
    EXPECT_EQ(err.stack_trace, nullptr);
    EXPECT_EQ(err.wavy_underline, nullptr);

    v8_delete_value(&res);
    v8_delete_error(&err);

    v8_delete_script(s1);
    v8_delete_script(s2);
}
