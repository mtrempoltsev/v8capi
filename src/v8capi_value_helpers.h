#pragma once

#include <v8.h>

#include "../include/v8capi_values.h"

v8_value from_v8_value(v8::Local<v8::Context> context, v8::Local<v8::Value> val);
v8::Local<v8::Value> to_v8_value(v8::Local<v8::Context> context, v8_value val);
