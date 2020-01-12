# v8capi
### C API for V8 JavaScript engine

```C
struct v8_instance* v8 = v8_new_instance(argv[0]);

struct v8_isolate* vm = v8_new_isolate();

struct v8_error error;

struct v8_script* script =
    v8_compile_script(vm, read_file("my_script.js"), "my_script.js", &error);

if (!script)
{
    printf("%s:%d: %s\n%s\n",
        error.location,
        error.line_number,
        error.message,
        error.wavy_underline)
    v8_delete_error(&error);
}
else
{
    if (!v8_run_script(script, &error)
    {
        printf("%s:%d: %s\n%s\nstack trace:\n%s\n",
            error.location,
            error.line_number,
            error.message,
            error.wavy_underline,
            error.stack_trace)
        v8_delete_error(&error);
    }

    v8_delete_script(script);
}

v8_delete_isolate(vm);

v8_delete_instance(v8);
```
