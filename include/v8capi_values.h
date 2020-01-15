#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

            // JS                   C
#define v8_undefined    0   // (void*) NULL
#define v8_boolean      1   // bool
#define v8_null         2   // (void*) NULL
#define v8_number       3   // double, int64_t
#define v8_string       4   // const char*
#define v8_big_int      5   // not implemented
#define v8_symbol       6   // not implemented
#define v8_object       7   // not implemented
#define v8_array        8   // v8_array_value
#define v8_set          9   // v8_set_value
#define v8_map          10  // v8_map_value
#define v8_function     11  // not implemented
#define v8_date         12  // not implemented

// You should not use data from this structure 
// directly, use a helper function instead
struct v8_value
{
    void* data;
    int64_t specifiers;
};

struct v8_array_value
{
    int32_t size;
    struct v8_value* data;
};

struct v8_set_value
{
    int32_t size;
    struct v8_value* data;
};

struct v8_pair_value
{
    struct v8_value first;
    struct v8_value second;
};

struct v8_map_value
{
    int32_t size;
    struct v8_pair_value* data;
};

struct v8_value v8_new_undefined();
struct v8_value v8_new_boolean(bool value);
struct v8_value v8_new_null();
struct v8_value v8_new_number(double value);
struct v8_value v8_new_integer(int64_t value);
struct v8_value v8_new_string(const char* value, int32_t length);
struct v8_value v8_new_array(int32_t size);
struct v8_value v8_new_set(int32_t size);
struct v8_value v8_new_map(int32_t size);

bool v8_is_undefined(struct v8_value value);
bool v8_is_boolean(struct v8_value value);
bool v8_is_null(struct v8_value value);
bool v8_is_number(struct v8_value value);
bool v8_is_double(struct v8_value value);
bool v8_is_int64(struct v8_value value);
bool v8_is_string(struct v8_value value);
bool v8_is_array(struct v8_value value);
bool v8_is_set(struct v8_value value);
bool v8_is_map(struct v8_value value);

int v8_get_value_type(struct v8_value value);

bool v8_to_bool(struct v8_value value);
double v8_to_double(struct v8_value value);
int64_t v8_to_int64(struct v8_value value);
const char* v8_to_string(struct v8_value* value);
struct v8_array_value v8_to_array(struct v8_value value);
struct v8_set_value v8_to_set(struct v8_value value);
struct v8_map_value v8_to_map(struct v8_value value);

void v8_delete_value(struct v8_value* value);

#ifdef __cplusplus
}
#endif
