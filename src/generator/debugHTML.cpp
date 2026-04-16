#include "debugHTML.h"

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
        res+=U"  <td>" + a[i-1].surname + U" " + a[i-1].name + U"</td>\n";
        res+=U"  <td>" + to_u32(a[i-1].sum) + U"</td>\n";
        for (int j = 0; j<cntCompetitions; j++){
            //res+=U"  <td>" + to_u32(a[i-1].points[j].score) + U"</td>\n";
            res+=U"  <td title=\"⏱ " + makeRes(a[i-1].points[j].place, a[i-1].points[j].time) + U"\">\n";

            bool inSum = (find(a[i-1].idxs.begin(), a[i-1].idxs.end(), j) != a[i-1].idxs.end());
            if (inSum) res += U"    <b>" + to_u32(a[i-1].points[j].score) + U"</b>\n";
            else res += U"    <span style=\"opacity:0.4\">" + to_u32(a[i-1].points[j].score) + U"</span>\n";
            res += U"  </td>\n";
        }
        res+=U"</tr>\n";
    }

    res+=U"</table>\n\n";
    return res;
}

void makeDebugHTML(){
    if (typeResult==Type::debug) std::cout << "start makeDebugHTML" << std::endl;
    std::ifstream in("template/debugTemplate.html");
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

    for (std::u32string group : groupsInJSON){
        if (typeResult==Type::debug)  std::cout << group << " size: " << groupBase[group].size() << std::endl;
        std::u32string table = getTable(group);
        res = res.substr(0, pos) + table + res.substr(pos);
        pos += table.size();
    }

    std::ofstream out("output/debug result.html");
    out << res;
}