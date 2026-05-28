#include "model.h"
#include "initialization.h"
#include "config.h"
#include "parser.h"
#include "database_reader.h"
#include "data_importer.h"
#include "debugHTML.h"
#include "releaseHTML.h"

int main(){
    init();
    readConfig();

    // for (auto page : pages){
    //     std::cout << page.id << ' ' << page.url << ' ' << page.title << ' ' << page.discipline << ' ' << page.date << ' ' << page.classComp << std::endl;
    // }
    
    for (size_t i = 0; i<pages.size(); i++){
        auto& page = pages[i];
        importData(page);             //
        readDatabase(page, i + 1);    //
        // parse(page, i+1);          <- рабочая версия, которая не создаёт БД, а работает самостоятельно
    } 
    
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

    std::filesystem::create_directories("output");
    makeDebugHTML();
    makeReleaseHTML();
}