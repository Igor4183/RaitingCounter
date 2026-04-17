#pragma once
#include "model.h"

void downloadPage(std::u32string link);
int getTime (const std::u32string hmmss);
bool isNumber(const std::u32string& s);
std::string getGroup (char gender, int date);
void parse(Competition& page);