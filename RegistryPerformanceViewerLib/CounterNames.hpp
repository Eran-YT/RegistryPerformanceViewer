#include <map>
#include <string>
#include <vector>
#include <set>

namespace performance_data
{

class CounterNames final
{
public:
    CounterNames();
    ~CounterNames() = default;

    [[nodiscard]] std::wstring counter_name(const std::wstring& counter_id) const;
    [[nodiscard]] std::wstring counter_id(const std::wstring& counter_name) const;

    [[nodiscard]] std::set<std::wstring> counter_ids() const;
    [[nodiscard]] std::set<std::wstring> counter_names() const;

    CounterNames(const CounterNames&) = default;
    CounterNames(CounterNames&&) = default;
    CounterNames& operator=(const CounterNames&) = default;
    CounterNames& operator=(CounterNames&&) = default;

private:

    [[nodiscard]] static std::map<std::wstring, std::wstring> get_counter_names();
    [[nodiscard]] static std::vector<wchar_t> get_counter_data(const std::wstring& source);

    std::map<std::wstring, std::wstring> m_counter_names;
};

}
