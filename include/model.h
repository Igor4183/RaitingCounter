#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

const int nowYear = 2026;
const std::map<std::u32string, std::vector<std::u32string>> aboveGroups = {
    {U"М10", {U"М12"}},
    {U"М12", {U"М14"}},
    {U"М14", {U"М16"}},
    {U"М16", {U"М18"}},
    {U"М18", {U"М20", U"М20", U"М21"}},
    {U"М20", {U"М21", U"М21Е"}},
    {U"М21", {U"М21Е"}},
    {U"М21Е", {U"Ж10"}},
    {U"Ж10", {U"Ж12"}},
    {U"Ж12", {U"Ж14"}},
    {U"Ж14", {U"Ж16"}},
    {U"Ж16", {U"Ж18"}},
    {U"Ж18", {U"Ж20", U"Ж21", U"Ж21Е"}},
    {U"Ж20", {U"Ж21", U"Ж21Е"}},
    {U"Ж21", {U"Ж21Е"}},
};

enum class Type{
    debug,
    release
};

extern Type typeResult;
extern int cntCompetitions;
extern std::vector<std::u32string> groups;

struct Competition{
    int id = -1;
    std::u32string url, title, discipline, date;
    int classComp;
};

extern std::vector<Competition> pages;

struct Result{
    Competition* page = nullptr;
    int time = -1, place = -1;
    int score = 0;

    int getScore(int time, int place, Result& leader, Competition* page){
        this->page = page;
        if (time == -1 or place == -1 or page == nullptr) {
            std::cerr << "model.h::getScore error: " << time << ' ' << place << " page==nullptr -> " << !page << std::endl;
            exit(0);
        }

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
    int sum = 0;

    void makeAthlete(std::u32string name, std::u32string surname, std::u32string group, int DOB){
        if (this->name==U"") this->name = name;
        if (this->surname==U"") this->surname = surname;
        if (this->group==U"") this->group = group;
        if (this->DOB == -1) this->DOB = DOB;
    }

    void add_points(Competition& page, int time, int place, Result& leader) {
        while ((int)points.size()<page.id)
            points.emplace_back(Result());
        sum += points.back().getScore(time, place, leader, &page);
    }
};

extern std::map<std::u32string, Athlete> bigBase;
extern std::map<std::u32string, std::vector<Athlete>> groupBase;