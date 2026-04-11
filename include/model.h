#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

const int nowYear = 2026;
const int pointsClass = 1000;
enum class TypeResult{
    debug,
    release
};

extern TypeResult typeResult;
extern int cntCompetitions;
extern std::vector<std::u32string> groups;
extern std::vector<std::u32string> pages;

struct Result{
    // std::u32string link = "None";
    // int time = -1, place = -1;
    int score = 0;
};  

struct Athlete{
    std::u32string name, surname, group;
    int DOB;
    std::vector<Result> points;
    int sum = 0;

    void add_points(std::u32string name, std::u32string surname, std::u32string group, int DOB, int addPoints, size_t nowid)
        {
            this->name = name;
            this->surname = surname;
            this->group = group;
            this->DOB = DOB;

            sum+=addPoints;
            while (points.size()<=nowid) points.emplace_back(0);
            points.back().score = addPoints;
    }
};

struct AthleteCmp{
    bool operator()(Athlete a, Athlete b){
        if (a.group==b.group) return a.sum>b.sum;
        return a.group<b.group;
    }
};

extern std::map<std::u32string, Athlete> allBase;
extern std::map<std::u32string, std::vector<Athlete>> groupBase;