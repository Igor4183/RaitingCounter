#include "u32string.h"
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

std::string to_utf8(const std::u32string s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s);
}

std::u32string to_u32(const std::string s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.from_bytes(s);
}

std::u32string to_u32(const int val) {
    return to_u32(std::to_string(val));
}

std::ostream& operator<<(std::ostream& os, const std::u32string s) {
    return os << to_utf8(s);
}