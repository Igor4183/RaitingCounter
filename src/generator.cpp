#include "model.h"
#include "u32string.h"
#include "generator.h"
#include "json.hpp"
#include <sstream>
#include <fstream>
#include <vector>

using json = nlohmann::json;

std::u32string makeTime(int time){
    if (time == -1) return U"--:--";
    std::u32string res = to_u32(time/60) + U":";
    if (time%60<10) res+=U"0";
    res+=to_u32(time%60);
    return res;
}

std::u32string getTable(std::u32string group){
    std::u32string res = U"<h2> " + group + U" </h2>\n";
    res+=U"<h4> Лучшие " + to_u32(getBest) + U" старта</h4>\n\n";
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
            if ((int)a[i-1].points.size()<=j) res+=U"  <td> <span style=\"opacity:0.4\"> 0 </span> </td>\n";
            else {
                //res+=U"  <td>" + to_u32(a[i-1].points[j].score) + U"</td>\n";
                res+=U"  <td title=\"⏱ " + makeTime(a[i-1].points[j].time) + U" | 🏁";
                if (a[i-1].points[j].place < 1) res+= U"нет результата\">\n";
                else res += to_u32(a[i-1].points[j].place) + U" место\">\n" ;

                bool inSum = (find(a[i-1].idxs.begin(), a[i-1].idxs.end(), j) != a[i-1].idxs.end());
                if (inSum) res += U"    <b>" + to_u32(a[i-1].points[j].score) + U"</b>\n";
                else res += U"    <span style=\"opacity:0.4\">" + to_u32(a[i-1].points[j].score) + U"</span>\n";
                res += U"  </td>\n";
            }
        }
        res+=U"</tr>\n";
    }

    res+=U"</table>\n\n";
    return res;
}

void makeDebugHTML(){
    if (typeResult==Type::debug) std::cout << "start makeDebugHTML" << std::endl;
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
         if (typeResult==Type::debug)  std::cout << group << " size: " << groupBase[to_u32(group)].size() << std::endl;
        std::u32string table = getTable(to_u32(group));
        res = res.substr(0, pos) + table + res.substr(pos);
        pos += table.size();
    }

    std::ofstream out("output/result.html");
    out << res;
}