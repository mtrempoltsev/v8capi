#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include <libplatform/libplatform.h>
#include <v8.h>

#include "../include/v8capi.h"

struct v8_instance
{
    std::unique_ptr<v8::Platform> platform_;
};

static const int ONE_THREAD = 1;

v8_instance* v8_new_instance(
    const char* exec_path)
{
    v8::V8::InitializeICUDefaultLocation(exec_path);
    v8::V8::InitializeExternalStartupData(exec_path);

    auto instance = std::make_unique<v8_instance>();

    instance->platform_ = v8::platform::NewDefaultPlatform(ONE_THREAD);

    v8::V8::InitializePlatform(instance->platform_.get());
    v8::V8::Initialize();

    return instance.release();
}

void v8_delete_instance(
    v8_instance* instance)
{
    assert(instance);

    if (!instance)
    {
        return;
    }

    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();

    delete instance;
}

struct v8_isolate
{
    std::unique_ptr<v8::ArrayBuffer::Allocator> allocator_;
    v8::Isolate* isolate_;
};

v8_isolate* v8_new_isolate()
{
    auto instance = std::make_unique<v8_isolate>();

    instance->allocator_.reset(v8::ArrayBuffer::Allocator::NewDefaultAllocator());

    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = instance->allocator_.get();
    create_params.only_terminate_in_safe_scope = true;

    instance->isolate_ = v8::Isolate::New(create_params);

    instance->isolate_->SetCaptureStackTraceForUncaughtExceptions(true);

    return instance.release();
}

void v8_delete_isolate(
    v8_isolate* isolate)
{
    assert(isolate);

    if (!isolate)
    {
        return;
    }

    isolate->isolate_->Dispose();

    delete isolate;
}

void v8_delete_error(
    v8_error* error)
{
    assert(error);

    if (!error)
    {
        return;
    }

    delete[] error->message;
    delete[] error->wavy_underline;
    delete[] error->location;
    delete[] error->stack_trace;
}

char* duplicate_string(
    const char* string)
{
    const size_t length = std::strlen(string);
    auto result = std::make_unique<char[]>(length + 1);
    std::memcpy(result.get(), string, length + 1);
    return result.release();
}

char* make_string(
    v8::Isolate* isolate, 
    v8::Local<v8::String> source)
{
    if (source.IsEmpty())
    {
        return duplicate_string("<invalid string>");
    }

    v8::String::Utf8Value utf8string(isolate, source);

    const char* string = *utf8string;

    return string
        ? duplicate_string(string)
        : duplicate_string("<string conversion failed>");
}

void clean_error(
    v8_error& error)
{
    error.message = nullptr;
    error.wavy_underline = nullptr;
    error.location = nullptr;
    error.line_number = 0;
    error.stack_trace = nullptr;
}

void make_error(
    v8::Isolate* isolate,
    v8::TryCatch& try_catch,
    v8_error* error)
{
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    v8::Local<v8::String> message;

    if (try_catch.Exception().IsEmpty()
        || !try_catch.Exception()->ToString(context).ToLocal(&message))
    {
        if (isolate->IsExecutionTerminating())
        {
            error->message = duplicate_string("Script execution terminated");
        }
        else
        {
            error->message = duplicate_string("<V8 didn't provide any information about error>");
        }
        return;
    }

    error->message = make_string(isolate, message);

    v8::Local<v8::Message> info = try_catch.Message();

    if (info.IsEmpty())
    {
        return;
    }

    v8::Local<v8::String> code_line;
    if (info->GetSourceLine(context).ToLocal(&code_line))
    {
        std::string text(*v8::String::Utf8Value(isolate, code_line));

        text += '\n';

        const auto start = info->GetStartColumn(context).FromMaybe(0);
        const auto end = info->GetEndColumn(context).FromMaybe(0);

        for (int i = 0; i < start; i++) {
            text += ' ';
        }

        text += '^';

        for (int i = start; i < end - 1; i++) {
            text += '~';
        }

        error->wavy_underline = duplicate_string(text.c_str());
    }

    v8::Local<v8::String> location;
    if (info->GetScriptResourceName()->ToString(context).ToLocal(&location))
    {
        error->location = make_string(isolate, location);
    }

    if (!info->GetLineNumber(context).To(&error->line_number))
    {
        error->line_number = 0;
    }

    v8::Local<v8::StackTrace> stack_trace = info->GetStackTrace();
    if (!stack_trace.IsEmpty() && stack_trace->GetFrameCount() > 0)
    {
        const std::string anonymous_function = "(anonymous function)";

        int max_function_length = 0;

        std::vector<std::string> functions;

        for (int i = 0; i < stack_trace->GetFrameCount(); ++i)
        {
            v8::Local<v8::StackFrame> frame = stack_trace->GetFrame(isolate, i);

            if (frame->GetFunctionName().IsEmpty())
            {
                functions.push_back(anonymous_function);
                max_function_length = std::max(
                    max_function_length, 
                    static_cast<int>(anonymous_function.length()));
            }
            else
            {
                functions.push_back(*v8::String::Utf8Value(isolate, frame->GetFunctionName()));
                max_function_length = std::max(
                    max_function_length, 
                    frame->GetFunctionName()->Length());
            }
        }

        std::string trace;

        for (int i = 0; i < stack_trace->GetFrameCount(); ++i)
        {
            if (i > 0)
            {
                trace += '\n';
            }

            v8::Local<v8::StackFrame> frame = stack_trace->GetFrame(isolate, i);

            trace += "    ";

            trace += functions[i];
            trace += std::string(max_function_length - functions[i].length(), ' ');

            trace += " @ ";

            trace += *v8::String::Utf8Value(isolate, frame->GetScriptName());
            trace += ':';
            trace += std::to_string(frame->GetLineNumber());
        }

        error->stack_trace = duplicate_string(trace.c_str());
    }
}

struct v8_script
{
    v8::Isolate* isolate_;
    v8::Persistent<v8::Context> context_;
    v8::Persistent<v8::Script> script_;
};

v8_script* v8_compile_script(
    v8_isolate* isolate,
    const char* code,
    const char* location,
    v8_error* error)
{
    assert(isolate);
    assert(code);
    assert(location);
    assert(error);

    if (!isolate || !code || !location || !error)
    {
        return nullptr;
    }

    clean_error(*error);

    v8::Isolate::Scope isolate_scope(isolate->isolate_);

    v8::HandleScope handle_scope(isolate->isolate_);

    v8::Local<v8::String> code_str;
    if (!v8::String::NewFromUtf8(
        isolate->isolate_, code, v8::NewStringType::kInternalized).
        ToLocal(&code_str))
    {
        return nullptr;
    }

    v8::Local<v8::String> location_str;
    if (!v8::String::NewFromUtf8(
        isolate->isolate_, location, v8::NewStringType::kInternalized).
        ToLocal(&location_str))
    {
        return nullptr;
    }

    v8::ScriptOrigin origin(location_str);

    v8::Local<v8::Context> context = v8::Context::New(isolate->isolate_);

    context->AllowCodeGenerationFromStrings(false);

    v8::Context::Scope context_scope(context);

    v8::TryCatch try_catch(isolate->isolate_);

    v8::Local<v8::Script> script;
    if (!v8::Script::Compile(context, code_str, &origin).ToLocal(&script))
    {
        make_error(isolate->isolate_, try_catch, error);
        return nullptr;
    }

    auto instance = std::make_unique<v8_script>();

    instance->isolate_ = isolate->isolate_;
    instance->context_.Reset(isolate->isolate_, context);
    instance->script_.Reset(isolate->isolate_, script);

    return instance.release();
}

bool v8_run_script(
    v8_script* script,
    v8_error* error)
{
    assert(script);
    assert(error);

    if (!script || !error)
    {
        return false;
    }

    clean_error(*error);

    v8::Isolate* isolate = script->isolate_;

    v8::Isolate::Scope isolate_scope(isolate);

    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::Context> context =
        v8::Local<v8::Context>::New(isolate, script->context_);

    v8::Context::Scope context_scope(context);

    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Script> compiled_script =
        v8::Local<v8::Script>::New(isolate, script->script_);

    v8::Local<v8::Value> result;

    {
        v8::Isolate::SafeForTerminationScope isolate_scope(isolate);

        if (!compiled_script->Run(context).ToLocal(&result))
        {
            make_error(isolate, try_catch, error);
            return false;
        }
    }

    isolate->ClearKeptObjects();

    return true;
}

void v8_terminate_script(
    v8_script* script)
{
    assert(script);

    if (!script)
    {
        return;
    }

    script->isolate_->TerminateExecution();
}

void v8_delete_script(
    v8_script* script)
{
    assert(script);

    if (!script)
    {
        return;
    }

    script->context_.Reset();
    script->script_.Reset();

    delete script;
}
