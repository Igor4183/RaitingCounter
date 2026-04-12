#include "model.h"
#include <iostream>

Type typeResult = Type::debug;
int cntCompetitions = 0;
std::vector<std::u32string> groups;
std::vector<Competition> pages;
std::map<std::u32string, Athlete> bigBase;
std::map<std::u32string, std::vector<Athlete>> groupBase;