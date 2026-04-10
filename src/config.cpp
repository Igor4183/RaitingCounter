#include "config.h"

void readConfig(){
    std::ifstream file("./config.json");
    
    json j;
    file >> j;

    typeResult = (j["type"] == "debug" ? TypeResult::debug : TypeResult::release);
    for (std::string group : j["groups"]){
        groups.push_back(group);
    }
    for (std::string page : j["pages"]){
        pages.push_back(page);
        cntCompetitions++;
    }

}