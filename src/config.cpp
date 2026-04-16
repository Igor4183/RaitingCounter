#include "config.h"
#include "model.h"
#include "json.hpp"
#include "u32string.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using json = nlohmann::json;

void addGroup(std::u32string group){
    groups.emplace_back(group);
    for (auto& now : aboveGroups.at(group)) 
        groups.emplace_back(now);
}

void readConfig(){
    std::ifstream file("config.json");
    
    json j;
    file >> j;

    typeResult = (j["type"] == "debug" ? Type::debug : Type::release);
    for (std::string group : j["groups"]){
        groupsInJSON.push_back(to_u32(group));
        addGroup(to_u32(group));
    }
    
    std::sort(groups.begin(), groups.end());
    groups.resize(std::unique(groups.begin(), groups.end()) - groups.begin());
    
    for (auto page : j["pages"]){
        cntCompetitions++;
        int id = page["id"];
        std::string url = page["url"];
        std::string title = page["title"];
        std::string discipline = page["discipline"];
        std::string date = page["date"];
        int classComp = page["class"];

        pages.push_back(Competition{id, to_u32(url), to_u32(title), to_u32(discipline), to_u32(date), classComp});
    }

}