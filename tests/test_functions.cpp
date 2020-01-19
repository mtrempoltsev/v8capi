#include <thread>

#include "lest.hpp"

#include "utils.h"

#include "../include/v8capi.h"

const lest::test specification[] =
{
CASE("Test multi isolates")
{
SETUP("With one isolate")
{
v8_isolate* vm = v8_new_isolate();

EXPECT(vm != nullptr);

SECTION("Sum")
{
    v8_error err;

    v8_script* script =
        v8_compile_script(vm, read_file("sum.js").c_str(), "my.js", &err);

    EXPECT(script != nullptr);

    v8_value res;

    bool ok = v8_run_script(script, &res, &err);

    v8_delete_value(&res);

    EXPECT(ok == true);

    v8_callable* sum = v8_get_function(script, "sum");

    EXPECT(sum != nullptr);

    v8_value args[] =
    {
        v8_new_integer(2),
        v8_new_integer(2)
    };

    ok = v8_call_function(sum, 2, args, &res, &err);

    EXPECT(ok == true);

    EXPECT(v8_is_int64(res) == true);
    EXPECT(v8_to_int64(res) == 4);

    EXPECT(err.line_number == 0);
    EXPECT(err.location == nullptr);
    EXPECT(err.message == nullptr);
    EXPECT(err.stack_trace == nullptr);
    EXPECT(err.wavy_underline == nullptr);

    v8_delete_value(&res);
    v8_delete_error(&err);
    v8_delete_function(sum);
    v8_delete_script(script);
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
