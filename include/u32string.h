#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

std::string to_utf8(const std::u32string s);
std::u32string to_u32(const std::string s);
std::u32string to_u32(const int val);
std::ostream& operator<<(std::ostream& os, const std::u32string s);