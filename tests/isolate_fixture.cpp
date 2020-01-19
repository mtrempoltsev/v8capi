#include "../include/v8capi.h"

#include "isolate_fixture.h"

void IsolateFixture::SetUp()
{
    vm = v8_new_isolate();
}

void IsolateFixture::TearDown()
{
    v8_delete_isolate(vm);
}

void SomeIsolatesFixture::SetUp()
{
    vm1 = v8_new_isolate();
    vm2 = v8_new_isolate();
    vm3 = v8_new_isolate();
    vm4 = v8_new_isolate();
}

void SomeIsolatesFixture::TearDown()
{
    v8_delete_isolate(vm1);
    v8_delete_isolate(vm2);
    v8_delete_isolate(vm3);
    v8_delete_isolate(vm4);
}
