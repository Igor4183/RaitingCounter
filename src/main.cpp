#include "model.h"
#include "config.h"
#include "parser.h"
#include "u32string.h"
#include "generator.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main(){
    readConfig();
    
    size_t nowid = 0;
    for (std::u32string link : pages){
        parse(link, nowid);
        nowid++;
    }
    
    if (typeResult==TypeResult::debug){
        std::cout << "--------------------make base---------------------" << std::endl;
        std::cout << "base_size: " << allBase.size() << std::endl;
        std::cout <<  "-------------------------------------------------" << std::endl;
    }

    // for (auto [key, a] : allBase){
    //     std::cout << a.name << ' ' << a.surname << ' ' << a.group << ' ' << a.DOB << ' ' << a.sum << std::endl;
    // }

    for (auto [key, athlete] : allBase)
        groupBase[athlete.group].push_back(athlete);
    
    for (auto& [group, vec] : groupBase){
        std::sort(vec.begin(), vec.end(), [](const Athlete& a, const Athlete& b){
            return a.sum>b.sum;
        });
    }

    makeHTML();
}