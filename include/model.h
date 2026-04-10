#pragma once
#include <iostream>
#include <string>
#include <vector>

enum class TypeResult{
    debug,
    release
};

extern TypeResult typeResult;
extern int cntCompetitions;
extern std::vector<std::string> groups;
extern std::vector<std::string> pages;

struct Result{
    std::string link = "None";
    int time = -1, place = -1;
    int score = 0;
};  

struct Sportsmen{
    std::string name, surname, group, team;
    std::vector<Result> points;
};