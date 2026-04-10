#pragma once
#include <iostream>

void downloadPage(std::string link);
int getTime (const std::string hmmss);
bool isNumber(const std::string& s);
std::string getGroup (char gender, int date);
void parse(std::string link, int nowid);