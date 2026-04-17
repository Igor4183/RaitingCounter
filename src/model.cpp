#include "model.h"

size_t validStarts;
int cntCompetitions = 0;
int cntAthletes = 0;
int bestScore = -1;
std::vector<std::u32string> groupsInJSON;
std::vector<std::u32string> groups;
std::vector<Competition> pages;
std::map<std::u32string, Athlete> bigBase;
std::map<std::u32string, std::vector<Athlete>> groupBase;