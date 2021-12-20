#include "pch.h"

#include "..\RegistryPerformanceViewerLib\CounterNames.hpp"

using namespace performance_data;

TEST(TestCounterNames, Process)
{
    CounterNames counter_names;
    EXPECT_EQ(counter_names.counter_name(L"230"), L"Process");
}

TEST(TestCounterNames, CounterNumber)
{
    CounterNames counter_names;
    EXPECT_GT(counter_names.counter_ids().size(), 1000);

    for (const std::wstring& id : counter_names.counter_ids()) {
        std::wcout << id << L" - " << counter_names.counter_name(id) << std::endl;
    }
}
