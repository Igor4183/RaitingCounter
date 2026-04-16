#pragma once
#include "u32string.h"
#include <iostream>
#include <fstream>

std::u32string makeTime (int time);
std::u32string makeRes(int place, int time);
void myError(std::string m);
void td(std::ofstream& out, const std::string& text);
//void tdScore(std::ofstream& out, int score, const std::string& tooltip, bool in_sum);