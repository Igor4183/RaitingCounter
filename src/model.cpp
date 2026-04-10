#include "model.h"
#include <iostream>

TypeResult typeResult = TypeResult::debug;
int cntCompetitions = 0;
std::vector<std::string> groups;
std::vector<std::string> pages;
std::map<std::string, Athlete> base;