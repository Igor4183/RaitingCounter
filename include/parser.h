#pragma once
#include "model.h"

void downloadPage(std::u32string link);
int getTime (const std::u32string hmmss);
bool isNumber(const std::u32string& s);
void parse(Competition& page, int colNumb);