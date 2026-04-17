#include "parser.h"
#include "model.h"

void downloadPage(std::u32string u32link){
    std::string link = to_utf8(u32link);
    std::string path = "build/page.html";
    std::string cmd = "curl " + link + " | iconv -f WINDOWS-1251 -t UTF-8 > " + path; 

    system("mkdir -p build"); // лишнее
    system(cmd.c_str());
}

int getTime (const std::string hmmss){ // h:mm:ss
    // std::string hmmss = to_utf8(u32hmmss);
    return stoi(hmmss.substr(0,1))*3600 + stoi(hmmss.substr(2,2))*60 + stoi(hmmss.substr(5,2));
} 

bool isNumber(const std::string s){
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::u32string getGroup (char32_t gender, int date){
    int groupNum = nowYear - date;
    groupNum += (groupNum & 1);
    if (groupNum>18) groupNum = 21;
    return std::u32string(1, gender) + to_u32(groupNum);
}

void parse(Competition& page){
    downloadPage(page.url);
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Downloaded " << page.url << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (std::u32string group : groups){
        std::ifstream fin("build/page.html");
        std::string line;
        std::u32string u32line;

        bool findGroup = false;
        while (std::getline(fin, line) and !findGroup) { 
            u32line = to_u32(line);
            if (line.substr(0,4) == "<h2>") {

                if (u32line.find(group) != std::u32string::npos) {
                    // std::cout << "find group: " << group << std::endl;
                    findGroup = true;
                    std::getline(fin, line); //<pre>
                    std::getline(fin, line); //Параметры дистанции:
                    std::getline(fin, line); //-------------------------------------------------------------------------------------------
                    std::getline(fin, line);
                    u32line = to_u32(line);
                    int nameptr = u32line.find(U"Фамилия");
                    int datePtr = u32line.find(U"Г.");
                    int timePtr = u32line.find(U"Результат");
                    int placePtr = u32line.find(U"Место");
                    Result leader;

                    if (nameptr == -1 or datePtr == -1 or timePtr == -1 or placePtr == -1){
                        std::cerr << "Parser::ptr error: " << group << ' ' << page.url << std::endl;
                        std::cerr << "nameptr: " << nameptr << std::endl; 
                        std::cerr << "datePtr: " << datePtr << std::endl; 
                        std::cerr << "timePtr: " << timePtr << std::endl; 
                        std::cerr << "placePtr: " << placePtr << std::endl; 
                        std::cerr << "   line: " << line << std::endl;
                        std::cerr << "u32line: " << u32line << std::endl;
                        exit(0);
                    }
                    
                    std::getline(fin, line); //-------------------------------------------------------------------------------------------

                    while (std::getline(fin, line)){
                        u32line = to_u32(line);
                        if (u32line[0]==U'-') break;
                        std::istringstream iss(line.substr(nameptr));
                        std::string name, surname;
                        iss >> surname >> name;
                        std::u32string u32name = to_u32(name), u32surname = to_u32(surname);

                        iss.clear();
                        iss.str(to_utf8(u32line.substr(datePtr)));
                        std::string dateStr;
                        iss >> dateStr;
                        // std::cout << "dateStr: " << dateStr <<  " | "; 
                        int date = stoi(dateStr);

                        iss.clear();
                        iss.str(to_utf8(u32line.substr(placePtr)));
                        std::string placeStr;
                        iss >> placeStr;
                        // std::cout << "placeStr: " << placeStr << " | "; 
                        if (!isNumber(placeStr)) { // "в/к, -"
                            std::u32string key = u32name + U" " + u32surname + U" " + to_u32(date);
                            if (bigBase.find(key) == bigBase.end()) bigBase[key].makeAthlete(u32name, u32surname, group, date);
                            if (placeStr=="-") bigBase[key].add_points(page, -1, -1, leader, TypeResult::removed);
                            else bigBase[key].add_points(page, -1, -1, leader, TypeResult::outOfCompetition);
                            continue;
                        }
                        int place = stoi(placeStr);

                        iss.clear();
                        iss.str(to_utf8(u32line.substr(timePtr)));
                        std::string resTimeStr;
                        iss >> resTimeStr;
                        // std::cout << "resTimeStr: " << resTimeStr << std::endl; 
                        int resTime = getTime(resTimeStr);
                        
                        if (place == 1) leader.getScore(resTime, place, leader, &page);
                        std::u32string key = u32name + U" " + u32surname + U" " + to_u32(date);
                        if (bigBase.find(key) == bigBase.end()) bigBase[key].makeAthlete(u32name, u32surname, group, date);
                        bigBase[key].add_points(page, resTime, place, leader);
                    }
                    break;
                }
            }
        }
    }
}