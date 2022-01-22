#include "pch.h"

#include "RegistryPerformanceViewerLib/CounterNames.hpp"
#include "RegistryPerformanceViewerLib/CounterData.hpp"

using namespace performance_data;
TEST(TestCounterNames, Process)
{
    const CounterNames counter_names;
    EXPECT_EQ(counter_names.counter_name(L"230"), L"Process");
}

TEST(TestCounterNames, CounterNumber)
{
    const CounterNames counter_names;
    EXPECT_GT(counter_names.counter_ids().size(), 1000);
}

TEST(TestCounterData, Print)
{
    const CounterData counter_data(L"230");
    print(counter_data.instances());
}

TEST(TestCounterData, OneCounter)
{
    const std::wstring counter_name = L"Browser";
    const CounterNames counters;

    const CounterData counter(counters.counter_id(counter_name));

    EXPECT_TRUE(!counter.instances().empty());

}
