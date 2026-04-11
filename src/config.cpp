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
    size_t pos = group.find_first_of(U"0123456789");
    std::u32string prefix = group.substr(0, pos);
    std::cout << "addGroup " << group << "|" << group.substr(pos) << ' ';
    groups.push_back(group);
    if (group.substr(pos) != U"18") 
        groups.push_back(std::u32string(group[0], 1) + to_u32(stoi(to_utf8(group.substr(pos)))+2));
    else {
        groups.push_back(prefix + U"21");
        groups.push_back(prefix + U"21E");
    }
    std::cout << "end" << std::endl;
}

void readConfig(){
    std::ifstream file("config.json");
    
    json j;
    file >> j;

    typeResult = (j["type"] == "debug" ? TypeResult::debug : TypeResult::release);
    for (std::string group : j["groups"]){
        addGroup(to_u32(group));
    }
    std::sort(groups.begin(), groups.end());
    groups.resize(std::unique(groups.begin(), groups.end()) - groups.begin());
    
    for (std::string page : j["pages"]){
        pages.push_back(to_u32(page));
        cntCompetitions++;
    }

}