#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

#include "../v8capi.h"

std::string read_file(const char* name)
{
    std::ifstream file(name);
    if (!file)
    {
        throw std::runtime_error(std::string("can't read ") + name);
    }

    std::stringstream reader;
    reader << file.rdbuf();

    return reader.str();
}

#define ASSERT(expr) \
    if (!(expr)) \
    { \
        std::string msg; \
        msg += #expr; \
        msg += " at line "; \
        msg += std::to_string(__LINE__); \
        throw std::runtime_error(msg); \
    } \

void check_compile_errors(v8_isolate* vm)
{
    v8_error* err = nullptr;

    v8_script* script = 
        v8_compile_script(vm, read_file("syntax_error.js").c_str(), "my.js", &err);

    ASSERT(script == nullptr);

    ASSERT(err != nullptr);

    ASSERT(err->line_number == 1);
    ASSERT(err->location == std::string("my.js"));
    ASSERT(err->message == std::string("SyntaxError: Invalid or unexpected token"));
    ASSERT(err->stack_trace == nullptr);
    ASSERT(err->wavy_underline == std::string("﻿x = 3'\n      ^"));

    v8_delete_error(err);
}

void check_runtime_errors(v8_isolate* vm)
{
    v8_error* err = nullptr;

    v8_script* script =
        v8_compile_script(vm, read_file("throw.js").c_str(), "my.js", &err);

    ASSERT(script != nullptr);
    ASSERT(err == nullptr);

    bool ok = v8_run_script(script, &err);

    ASSERT(ok == false);
    ASSERT(err != nullptr);

    ASSERT(err->line_number == 2);
    ASSERT(err->location == std::string("my.js"));
    ASSERT(err->message == std::string("my_err"));
    ASSERT(err->stack_trace == std::string("    test                 @ my.js:2\n    (anonymous function) @ my.js:5"));
    ASSERT(err->wavy_underline == std::string("    throw 'my_err'\n    ^"));

    v8_delete_error(err);

    v8_delete_script(script);
}

void check_normal_execution(v8_isolate* vm)
{
    v8_error* err = nullptr;

    v8_script* script =
        v8_compile_script(vm, read_file("good_script.js").c_str(), "my.js", &err);

    ASSERT(script != nullptr);
    ASSERT(err == nullptr);

    bool ok = v8_run_script(script, &err);

    ASSERT(ok == true);
    ASSERT(err == nullptr);

    v8_delete_script(script);
}

void check_script_termination(v8_isolate* vm)
{
    v8_error* err = nullptr;

    v8_script* s1 =
        v8_compile_script(vm, read_file("infinite_loop.js").c_str(), "my.js", &err);

    ASSERT(s1 != nullptr);
    ASSERT(err == nullptr);

    v8_script* s2 =
        v8_compile_script(vm, read_file("good_script.js").c_str(), "my.js", &err);

    ASSERT(s2 != nullptr);
    ASSERT(err == nullptr);

    std::thread killer(
        [s1]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            v8_terminate_script(s1);
        });

    bool ok = v8_run_script(s1, &err);

    killer.join();

    ASSERT(ok == false);
    ASSERT(err != nullptr);

    ASSERT(err->line_number == 0);
    ASSERT(err->location == nullptr);
    ASSERT(err->message == std::string("Script execution terminated"));
    ASSERT(err->stack_trace == nullptr);
    ASSERT(err->wavy_underline == nullptr);

    v8_delete_error(err);

    err = nullptr;

    ok = v8_run_script(s2, &err);

    ASSERT(ok == true);
    ASSERT(err == nullptr);

    v8_delete_script(s1);
    v8_delete_script(s2);
}

void run_tests()
{
    v8_isolate* vm1 = v8_new_isolate();

    check_compile_errors(vm1);
    check_runtime_errors(vm1);
    check_normal_execution(vm1);
    check_script_termination(vm1);

    v8_delete_isolate(vm1);
}

int main(int argc, char* argv[])
{
    try
    {
        v8_instance* v8 = v8_new_instance(argv[0]);
        run_tests();
        v8_delete_instance(v8);
    }
    catch (const std::exception & err)
    {
        std::cerr << err.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "unexpected error\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
