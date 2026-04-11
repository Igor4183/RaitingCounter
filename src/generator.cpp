#include "model.h"
#include "u32string.h"
#include "generator.h"
#include "json.hpp"
#include <sstream>
#include <fstream>
#include <vector>

using json = nlohmann::json;

std::u32string getTable(std::u32string group){
    std::u32string res = U"<h2> " + group + U" </h2>\n\n";
    res+=U"<table border=\"1\">\n";
    res+=U"<tr>\n";
    res+=U"  <th>#</th>\n";
    res+=U"  <th>Name</th>\n";
    res+=U"  <th>Sum</th>\n";
    for (int i = 1; i<=cntCompetitions; i++) 
        res+=U"  <th>" + to_u32(i) + U"</th>\n";
    res+=U"</tr>\n";

    std::vector<Athlete> a = groupBase[group];
    for (size_t i = 1; i<=a.size(); i++){
        res+=U"<tr>\n";
        res+=U"  <td>" + to_u32(i) + U"</td>\n";
        res+=U"  <td>" + a[i-1].name + U" " + a[i-1].surname + U"</td>\n";
        res+=U"  <td>" + to_u32(a[i-1].sum) + U"</td>\n";
        for (int j = 0; j<cntCompetitions; j++){
            res+=U"  <td>" + to_u32(a[i-1].points[j].score) + U"</td>\n";
        }
        res+=U"<tr>\n";
    }

    res+=U"</table>\n\n";
    return res;
}

void makeHTML(){
    if (typeResult==TypeResult::debug) std::cout << "start makeHTML" << std::endl;
    std::ifstream in("template.html");
    std::u32string res = U"";

    std::ostringstream ss;
    ss << in.rdbuf();
    res = to_u32(ss.str());

    int pos = res.find(U"<!--TEMPLATE-->");
    if (pos == -1){
        std::cerr << "generator.cpp::NOT FOUNDED <!--TEMPLATE--> in template.html" << std::endl;
        exit(0);
    }
    res.erase(pos, 15);

    std::ifstream file("config.json");
    json j;
    file >> j;
    
    for (std::string group : j["groups"]){
         if (typeResult==TypeResult::debug)  std::cout << group << " size: " << groupBase[to_u32(group)].size() << std::endl;
        std::u32string table = getTable(to_u32(group));
        res = res.substr(0, pos) + table + res.substr(pos);
        pos += table.size();
    }

    std::ofstream out("output/result.html");
    out << res;
}