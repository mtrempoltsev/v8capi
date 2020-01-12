#include <chrono>
#include <string>
#include <thread>

#include "lest.hpp"

#include "utils.h"

#include "../include/v8capi.h"

const lest::test specification[] =
{
CASE("Test common features")
{
SETUP("With one isolate")
{
v8_isolate* vm = v8_new_isolate();

EXPECT(vm != nullptr);

SECTION("Test compilation with errors")
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("syntax_error.js").c_str(), "my.js", &err);

    EXPECT(script == nullptr);

    EXPECT(err.line_number == 1);
    EXPECT(err.location == std::string("my.js"));
    EXPECT(err.message == std::string("SyntaxError: Invalid or unexpected token"));
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == std::string("﻿x = 3'\n      ^"));

    v8_delete_error(&err);
}

SECTION("Test runtime errors")
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("throw.js").c_str(), "my.js", &err);

    EXPECT(script != nullptr);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    const bool ok = v8_run_script(script, &err);

    EXPECT(ok == false);

    EXPECT(err.line_number == 2);
    EXPECT(err.location == std::string("my.js"));
    EXPECT(err.message == std::string("my_err"));
    EXPECT(err.stack_trace == std::string("    test                 @ my.js:2\n    (anonymous function) @ my.js:5"));
    EXPECT(err.wavy_underline == std::string("    throw 'my_err'\n    ^"));

    v8_delete_error(&err);

    v8_delete_script(script);
}

SECTION("Test normal execution")
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("good_script.js").c_str(), "my.js", &err);

    EXPECT(script != nullptr);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    bool ok = v8_run_script(script, &err);

    EXPECT(ok == true);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    v8_delete_script(script);
}

SECTION("Test script termination")
{
    v8_error err;

    v8_script* s1 =
        v8_compile_script(vm, read_file("infinite_loop.js").c_str(), "my.js", &err);

    EXPECT(s1 != nullptr);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    v8_script* s2 =
        v8_compile_script(vm, read_file("good_script.js").c_str(), "my.js", &err);

    v8_run_script(s2, &err);

    EXPECT(s2 != nullptr);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    std::thread killer(
        [s1]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            v8_terminate_script(s1);
        });

    bool ok = v8_run_script(s1, &err);

    killer.join();

    EXPECT(ok == false);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == std::string("Script execution terminated"));
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    v8_delete_error(&err);

    ok = v8_run_script(s2, &err);

    EXPECT(ok == true);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    v8_delete_script(s1);
    v8_delete_script(s2);
}

v8_delete_isolate(vm);
}
}
};

int main(int argc, char* argv[])
{
    v8_instance* v8 = v8_new_instance(argv[0]);

    const int result = lest::run(specification, argc, argv);

    v8_delete_instance(v8);

    return result;
}
