#include <thread>

#include "lest.hpp"

#include "utils.h"

#include "../include/v8capi.h"

const lest::test specification[] =
{
CASE("Test multi isolates")
{
SETUP("With two isolates")
{
v8_isolate* vm1 = v8_new_isolate();
v8_isolate* vm2 = v8_new_isolate();
v8_isolate* vm3 = v8_new_isolate();
v8_isolate* vm4 = v8_new_isolate();

EXPECT(vm1 != nullptr);
EXPECT(vm2 != nullptr);
EXPECT(vm3 != nullptr);
EXPECT(vm4 != nullptr);

SECTION("Two threads")
{
    const int N = 100;

    int64_t res1 = 0;
    int64_t res2 = 0;
    int64_t res3 = 0;
    int64_t res4 = 0;

    std::string code = read_file("good_script.js");

    std::thread t1(
        [vm1, code, &res1]()
        {
            v8_error err;
            v8_value res;

            for (int i = 0; i < N; ++i)
            {
                v8_script* script =
                    v8_compile_script(vm1, code.c_str(), "my.js", &err);
                v8_run_script(script, &res, &err);
                v8_delete_script(script);
            }

            res1 = v8_to_int64(res);

            v8_delete_value(&res);
            v8_delete_error(&err);
        });

    std::thread t2(
        [vm2, code, &res2]()
        {
            v8_error err;
            v8_value res;

            for (int i = 0; i < N; ++i)
            {
                v8_script* script =
                    v8_compile_script(vm2, code.c_str(), "my.js", &err);
                v8_run_script(script, &res, &err);
                v8_delete_script(script);
            }

            res2 = v8_to_int64(res);

            v8_delete_value(&res);
            v8_delete_error(&err);
        });

    std::thread t3(
        [vm3, code, &res3]()
        {
            v8_error err;
            v8_value res;

            for (int i = 0; i < N; ++i)
            {
                v8_script* script =
                    v8_compile_script(vm3, code.c_str(), "my.js", &err);
                v8_run_script(script, &res, &err);
                v8_delete_script(script);
            }

            res3 = v8_to_int64(res);

            v8_delete_value(&res);
            v8_delete_error(&err);
        });

    std::thread t4(
        [vm4, code, &res4]()
        {
            v8_error err;
            v8_value res;

            for (int i = 0; i < N; ++i)
            {
                v8_script* script =
                    v8_compile_script(vm4, code.c_str(), "my.js", &err);
                v8_run_script(script, &res, &err);
                v8_delete_script(script);
            }

            res4 = v8_to_int64(res);

            v8_delete_value(&res);
            v8_delete_error(&err);
        });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    EXPECT(res1 == 4);
    EXPECT(res2 == 4);
    EXPECT(res3 == 4);
    EXPECT(res4 == 4);
}

v8_delete_isolate(vm1);
v8_delete_isolate(vm2);
v8_delete_isolate(vm3);
v8_delete_isolate(vm4);
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
