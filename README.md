# V8 C API

C API for V8 JavaScript engine

### Supported operating systems

- Linux x64
- MacOS X

### How to build

#### Build Requirements

- Git to clone this repository
- A C++17-standard-compliant compiler
- Ninja build system (https://ninja-build.org/)
- CMake (https://cmake.org/)

#### Step by step guide

From root of V8 C API repository:

```
- mkdir build
- cd build
- cmake .. -DCMAKE_BUILD_TYPE=Release
  or if you want to build V8 library (it will take about 20-60 minutes,
  also on Linux you may need a root password. See https://v8.dev/docs/build)
  cmake .. -DCMAKE_BUILD_TYPE=Release -DV8CAPI_BUILD_V8=ON
- make
```

After that you can run the tests:

```
- ctest
  or
  ./test_v8capi
```

### Example

```C
#include <stdio.h>

#include <v8capi.h>

void print_error(struct v8_error* error)
{
    printf("%s:%d: %s\n%s\nstack trace:\n%s\n",
        error->location,
        error->line_number,
        error->message,
        error->wavy_underline,
        error->stack_trace);
}

int main(int argc, char* argv[])
{
    struct v8_instance* v8 = v8_new_instance(argv[0]);

    struct v8_isolate* vm = v8_new_isolate();

    struct v8_error error;

    const char* code = "\
        function sum(x, y) { \
            return x + y \
        } \
        'ok'";

    struct v8_script* script =
        v8_compile_script(vm, code, "my_script.js", &error);

    if (!script)
    {
        print_error(&error);
        v8_delete_error(&error);
        return 1;
    }

    struct v8_value result;
    if (!v8_run_script(script, &result, &error))
    {
        print_error(&error);
        v8_delete_error(&error);
        return 1;
    }

    printf("Result: %s\n", v8_to_string(&result).data); // ok

    v8_delete_value(&result);

    struct v8_callable* sum = v8_get_function(script, "sum");

    if (!sum)
    {
        printf("The 'sum' function is missing");
        return 1;
    }

    struct v8_value args[] =
    {
        v8_new_integer(2),
        v8_new_integer(2)
    };

    if (!v8_call_function(sum, 2, args, &result, &error))
    {
        print_error(&error);
        v8_delete_error(&error);
        return 1;
    }

    printf("Result: %ld\n", v8_to_int64(result)); // 4

    v8_delete_value(&result);

    v8_delete_script(script);

    v8_delete_isolate(vm);

    v8_delete_instance(v8);

    return 0;
}
```
