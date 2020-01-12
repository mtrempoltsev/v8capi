#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct v8_instance;

// Creates an instance of V8. Must be called once
// and before all other calls
struct v8_instance* v8_new_instance(
    const char* exec_path);

void v8_delete_instance(
    struct v8_instance* instance);

struct v8_isolate;

// Creates a JS virual machine. Each VM has its 
// own context and heap. VM uses only one thread
struct v8_isolate* v8_new_isolate();

void v8_delete_isolate(
    struct v8_isolate* isolate);

// To get message like this:
//
// my.js:3: ReferenceError: y is not defined
// x = y
// ^
// stack trace :
//   test                  @ my.js:3
//   (anonymous function) @ my.js:5
//
// use next pattern:
//
// printf("%s:%d: %s\n%s\nstack trace:\n%s\n", 
//     err->location, 
//     err->line_number, 
//     err->message, 
//     err->wavy_underline, 
//     err->stack_trace)
//
// Some pointers may be NULL if information is missing
struct v8_error
{
    const char* message;
    const char* wavy_underline;
    const char* location;
    int32_t line_number;
    const char* stack_trace;
};

void v8_delete_error(
    struct v8_error* error);

struct v8_script;

// Compiles and binds a JS script to the specified VM. 
// Returns a script instance or NULL and populates 
// the error structure
struct v8_script* v8_compile_script(
    struct v8_isolate* isolate,
    const char* code,
    const char* location,
    struct v8_error* error);

struct v8_value;

struct v8_array
{
    int64_t size;
    void* data;
};

struct v8_pair
{
    v8_value* first;
    v8_value* second;
};

struct v8_value
{
    typedef enum 
    {
        // JS               C
        js_boolean,     // bool
        js_null,        // void* NULL
        js_undefined,   // void* NULL
        js_number,      // see specifiers
        js_string,      // const char*
        js_big_int,     // not implemented
        js_symbol,      // not implemented
        js_object       // see specifiers
    } js_types;

    typedef enum
    {
        // js number
        number,         // double
        int64,          // int64_t
        int32,          // int32_t
        uint32,         // uint32_t

        // js object
        object,         // v8_array* of v8_pair
        array,          // v8_array* of v8_value
        map,            // v8_array* of v8_pair
        set,            // v8_array* of v8_value
        function,       // not implemented
        date            // not implemented
    } type_specifiers;

    js_types type;
    type_specifiers specifier;
    void* value;
};

void v8_delete_value(
    struct v8_value* value);

// Runs a JS script and returns true if successfull, 
// the result of execution will be written to
// the value structure. 
// If an error occurs then false is returned and 
// the error structure is populated
bool v8_run_script(
    struct v8_script* script,
    struct v8_error* error);

// Terminates the currently running script
void v8_terminate_script(
    struct v8_script* script);

void v8_delete_script(
    struct v8_script* script);

#ifdef __cplusplus
}
#endif
