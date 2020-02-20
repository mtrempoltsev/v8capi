#pragma once

#include <stdint.h>

#include "v8capi_values.h"

#ifdef __cplusplus
// Only x64 is supported
static_assert(sizeof(void*) == sizeof(double));

extern "C" {
#endif

struct v8_instance;

// Creates an instance of V8. Must be called once
// and before all other calls. thread_pool_size
// is the number of worker threads to allocate for
// background jobs. If a value of zero is passed,
// a suitable default based on the current number of
// processors online will be chosen
struct v8_instance* v8_new_instance(
    unsigned thread_pool_size,
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

// Runs a JS script and returns true if successfull, 
// the result of execution will be written to the result 
// structure.
// If an error occurs then false is returned and 
// the error structure is populated
bool v8_run_script(
    struct v8_script* script,
    struct v8_value* result,
    struct v8_error* error);

// Terminates the currently running script
void v8_terminate_script(
    struct v8_script* script);

void v8_delete_script(
    struct v8_script* script);

struct v8_callable;

// Get JS function from compiled script. Returns NULL 
// if there is no function with specified name
struct v8_callable* v8_get_function(
    struct v8_script* script,
    const char* name);

// Calls a JS function and returns true if successfull, 
// the result of execution will be written to the result 
// structure.
// If an error occurs then false is returned and 
// the error structure is populated
bool v8_call_function(
    struct v8_callable* func,
    int argc,
    struct v8_value* argv,
    struct v8_value* result,
    struct v8_error* error);

void v8_delete_function(
    struct v8_callable* func);

#ifdef __cplusplus
}
#endif
