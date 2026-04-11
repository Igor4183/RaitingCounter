#include "model.h"
#include "config.h"
#include "parser.h"
#include "u32string.h"
#include <iostream>
#include <string>
#include <vector>

int main(){
    readConfig();
    
    size_t nowid = 0;
    for (std::u32string link : pages){
        parse(link, nowid);
        nowid++;
    }

    std::cout << "base_size: " << base.size() << std::endl;
    for (auto [key, now] : base){
        std::cout << now.name << ' ' << now.surname << ' ' << now.group << ' ' << now.DOB << ' ' << now.sum << std::endl;
    }
}