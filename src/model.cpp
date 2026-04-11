#include "model.h"
#include <iostream>

TypeResult typeResult = TypeResult::debug;
int cntCompetitions = 0;
std::vector<std::u32string> groups;
std::vector<std::u32string> pages;
std::map<std::u32string, Athlete> base;