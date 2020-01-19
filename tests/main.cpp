#include <gtest/gtest.h>

#include "../include/v8capi.h"

int main(int argc, char* argv[])
{
    v8_instance* v8 = v8_new_instance(argv[0]);

    ::testing::InitGoogleTest(&argc, argv);
    const auto result = RUN_ALL_TESTS();

    v8_delete_instance(v8);

    return result;
}
