#include "config.h"
#include "model.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using json = nlohmann::json;

void addGroup(std::string group){
    size_t pos = group.find_first_of("0123456789");
    std::string prefix = group.substr(0, pos);
    if (pos == std::string::npos) {
        std::cerr << "config.cpp::addGroup::wrong group" << std::endl;
        exit(0);
    }

    std::cout << "addGroup " << group << "|" << group.substr(pos) << ' ';
    groups.push_back(group);
    if (group.substr(pos) != "18") 
        groups.push_back(prefix + std::to_string(stoi(group.substr(pos))+2));
    else {
        groups.push_back(prefix + "21");
        groups.push_back(prefix + "21E");
    }
    std::cout << "end" << std::endl;
}

void readConfig(){
    std::ifstream file("./config.json");
    
    json j;
    file >> j;

    typeResult = (j["type"] == "debug" ? TypeResult::debug : TypeResult::release);
    for (std::string group : j["groups"]){
        addGroup(group);
    }
    std::sort(groups.begin(), groups.end());
    groups.resize(std::unique(groups.begin(), groups.end()) - groups.begin());
    
    for (std::string page : j["pages"]){
        pages.push_back(page);
        cntCompetitions++;
    }

}