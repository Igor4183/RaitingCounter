#pragma once
#include "u32string.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>


const int nowYear = 2026;
extern size_t validStarts;
const std::map<std::u32string, std::vector<std::u32string>> aboveGroups = {
    {U"М10", {U"М12", U"М12А"}},
    {U"М12", {U"М14", U"М14А"}},
    {U"М14", {U"М16", U"М16А"}},
    {U"М16", {U"М18", U"М18А"}},
    {U"М18", {U"М20", U"М20", U"М21"}},
    {U"М20", {U"М21", U"М21Е"}},
    {U"М21", {U"М21Е"}},
    {U"М21Е", {U"Ж10"}},
    {U"Ж10", {U"Ж12", U"Ж12А"}},
    {U"Ж12", {U"Ж14", U"Ж14А"}},
    {U"Ж14", {U"Ж16", U"Ж16А"}},
    {U"Ж16", {U"Ж18", U"Ж18А"}},
    {U"Ж18", {U"Ж20", U"Ж21", U"Ж21Е"}},
    {U"Ж20", {U"Ж21", U"Ж21Е"}},
    {U"Ж21", {U"Ж21Е"}},
};

struct Competition{
    int id = -1;
    std::u32string url, title, discipline, date;
    int classComp;
};

enum class TypeResult{
    valid,
    outOfCompetition,
    removed,
    undefinded
};

struct Result{
    Competition* page = nullptr;
    int time = -1, place = -1;
    int score = 0;
    TypeResult type = TypeResult::undefinded;

    Result() = default;
    Result(Competition* page) : page(page) {} 
    Result(TypeResult type) : type(type) {}

    int getScore(int time, int place, Result& leader, Competition* page){
        if (time == -1 or place == -1 or page == nullptr) {
            std::cerr << "model.h::getScore error: " << time << ' ' << place << " page==nullptr -> " << !page << std::endl;
            exit(0);
        }

        type = TypeResult::valid;
        this->page = page;
        this->time = time;
        this->place = place;
        // std::cout << leader.time << ' ' << time << ' ' << place << ' ';
        if (place == 1) score = page->classComp;
        else score = std::max<int>(round((long double)(page->classComp) * (2.0 * (leader.time) / (long double)time - 1)), 0);
        // std::cout << score << std::endl;
        return score;
    }
};  

struct Athlete{
    std::u32string name = U"", surname = U"", group = U"";
    int DOB = -1;
    std::vector<Result> points;
    std::vector<int> idxs;
    int sum = 0;

    void updateSum(){
        idxs.assign(points.size(), 0);
        std::iota(idxs.begin(), idxs.end(), 0);
        sort(idxs.begin(), idxs.end(), [&](const int a, const int b){
            return points[a].score > points[b].score;
        });

        if (idxs.size()>validStarts) idxs.resize(validStarts);
        while (!idxs.empty() and points[idxs.back()].score<=0) idxs.pop_back();
        sum = 0;
        for (auto idx : idxs) 
            sum+=points[idx].score;
    }

    std::u32string getGroup(char32_t gender, int date){
        int groupNum = nowYear - date;
        groupNum += (groupNum & 1);
        if (groupNum>18) groupNum = 21;
        return std::u32string(1, gender) + to_u32(groupNum);
    }

    void makeAthlete(std::u32string name, std::u32string surname, std::u32string group, int DOB){
        if (this->name==U"") this->name = name;
        if (this->surname==U"") this->surname = surname;
        if (this->group==U"") this->group = getGroup(group[0], DOB);
        if (this->DOB == -1) this->DOB = DOB;
    }

    void add_points(Competition& page, int time, int place, Result& leader, int colNumb, TypeResult type = TypeResult::valid) {
        while ((int)points.size()<colNumb)
            points.emplace_back(Result());
        if (type == TypeResult::valid) points.back().getScore(time, place, leader, &page);
        else points.back().type = type;
        updateSum();
    }
};

extern int cntCompetitions;
extern int cntAthletes;
extern int bestScore;
extern std::vector<std::u32string> groupsInJSON;
extern std::vector<std::u32string> groups;
extern std::vector<Competition> pages;
extern std::map<std::u32string, Athlete> bigBase;
extern std::map<std::u32string, std::vector<Athlete>> groupBase;