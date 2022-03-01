#include "Utils.hpp"

std::wstring Utils::fix_path(const std::wstring& path)
{
    static const std::wstring invalid_chars{ L'<', L'>', L':', L'"', L'/', L'\\', L'|', L'?', L'*' };
    std::wstring new_path(path);
    for (wchar_t& wchar : new_path) {
        if (invalid_chars.contains(wchar)) {
            wchar = L'_';
        }
    }

    return new_path;
}
