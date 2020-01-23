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
  or if you want to build V8 library (it will take about 20-60 minutes)
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
struct v8_instance* v8 = v8_new_instance(argv[0]);

struct v8_isolate* vm = v8_new_isolate();

struct v8_error error;

struct v8_script* script =
    v8_compile_script(vm, "2 + 2", "my_script.js", &error);

if (!script)
{
    printf("%s:%d: %s\n%s\n",
        error.location,
        error.line_number,
        error.message,
        error.wavy_underline);
    v8_delete_error(&error);
}
else
{
    struct v8_value result;
    if (!v8_run_script(script, &result, &error))
    {
        printf("%s:%d: %s\n%s\nstack trace:\n%s\n",
            error.location,
            error.line_number,
            error.message,
            error.wavy_underline,
            error.stack_trace);
        v8_delete_error(&error);
    }
    else
    {
        printf("result = %ld\n", v8_to_int64(result));
        v8_delete_value(&result);
    }

    v8_delete_script(script);
}

v8_delete_isolate(vm);

v8_delete_instance(v8);
```
