#include "model.h"
#include "config.h"
#include "parser.h"
#include "debugHTML.h"
#include "releaseHTML.h"

int main(){
    readConfig();

    // for (auto page : pages){
    //     std::cout << page.id << ' ' << page.url << ' ' << page.title << ' ' << page.discipline << ' ' << page.date << ' ' << page.classComp << std::endl;
    // }
    
    for (auto& page : pages)
        parse(page);
    
    std::cout << "------------------" << "base_size: " << bigBase.size() << "------------------" << std::endl;

    // for (auto [key, a] : bigBase){
    //     std::cout << a.name << ' ' << a.surname << ' ' << a.group << ' ' << a.DOB << ' ' << a.sum << ": ";
    //     for (auto now : a.points) 
    //         std::cout << now.score << ' ';
    //     std::cout << std::endl;
    // }

    for (auto [key, athlete] : bigBase){
         while ((int)athlete.points.size()<cntCompetitions)
            athlete.points.push_back(Result(&pages[athlete.points.size()]));
        bestScore = std::max(bestScore, athlete.sum);
        groupBase[athlete.group].push_back(athlete);
    }
    
    for (auto& [group, vec] : groupBase){
        std::sort(vec.begin(), vec.end(), [](const Athlete& a, const Athlete& b){
            return a.sum>b.sum;
        });
    }

    for (std::u32string group : groupsInJSON)
        cntAthletes += groupBase[group].size();

    makeDebugHTML();
    makeReleaseHTML();
}