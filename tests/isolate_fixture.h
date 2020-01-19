#pragma once

#include <gtest/gtest.h>

struct v8_isolate;

class IsolateFixture
    : public ::testing::Test
{
public:
    v8_isolate* vm = nullptr;

protected:
    void SetUp() override;
    void TearDown() override;
};

class SomeIsolatesFixture
    : public ::testing::Test
{
public:
    v8_isolate* vm1 = nullptr;
    v8_isolate* vm2 = nullptr;
    v8_isolate* vm3 = nullptr;
    v8_isolate* vm4 = nullptr;

protected:
    void SetUp() override;
    void TearDown() override;
};
