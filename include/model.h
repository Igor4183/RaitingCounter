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
extern std::vector<std::string> groups;
extern std::vector<std::string> pages;

struct Result{
    // std::string link = "None";
    // int time = -1, place = -1;
    int score = 0;
};  

struct Athlete{
    std::string name, surname, group;
    int DOB;
    std::vector<Result> points;
    int sum = 0;

    void add_points(std::string name, std::string surname, std::string group, int DOB, int addPoints, size_t nowid)
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

extern std::map<std::string, Athlete> base;