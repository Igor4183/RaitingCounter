#include "parser.h"
#include "model.h"
#include "u32string.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

void downloadPage(std::string link){
    std::string path = "build/page.html";
    std::string cmd = "curl " + link + " | iconv -f WINDOWS-1251 -t UTF-8 > " + path; 

    system("mkdir -p build"); // лишнее
    system(cmd.c_str());
}
void downloadPage(std::u32string link){
    downloadPage(to_utf8(link));
}

int getTime (const std::string hmmss){ // h:mm:ss
    return stoi(hmmss.substr(0,1))*3600 + stoi(hmmss.substr(2,2))*60 + stoi(hmmss.substr(5,2));
}
int getTime (const std::u32string hmmss){
    return getTime(to_utf8(hmmss));
}

bool isNumber(const std::string s){
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}
bool isNumber(const std::u32string s){
    return isNumber(to_utf8(s));
}

std::u32string getGroup (char32_t gender, int date){
    int groupNum = nowYear - date;
    groupNum += (groupNum & 1);
    if (groupNum>18) groupNum = 21;
    return std::u32string(1, gender) + to_u32(groupNum);
}

void parse(std::u32string link, int nowid){
    downloadPage(link);
    if (typeResult==TypeResult::debug){
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Downloaded " << link << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
    }

    for (std::u32string group : groups){
        std::ifstream fin("build/page.html");
        std::string line;
        std::u32string u32line;

        bool findGroup = false;
        while (std::getline(fin, line) and !findGroup) { 
            u32line = to_u32(line);
            if (line.substr(0,4) == "<h2>") {
                // int ptr = line.find("</span>") + 7;
                // const size_t len = group.size();
                // 
                // std::cout << "find <h2>: " << std::endl;
                // std::cout << "    " << group << ' ' << ptr << ' ' << len << std::endl;
                // std::cout << "        " << line << std::endl;

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
                    int leader = -1;

                    if (nameptr == -1 or datePtr == -1 or timePtr == -1 or placePtr == -1){
                        std::cerr << "Parser::ptr error: " << group << ' ' << link << std::endl;
                        std::cerr << "nameptr: " << nameptr << std::endl; 
                        std::cerr << "datePtr: " << datePtr << std::endl; 
                        std::cerr << "timePtr: " << timePtr << std::endl; 
                        std::cerr << "placePtr: " << placePtr << std::endl; 
                        std::cerr << "   line: " << line << std::endl;
                        std::cerr << "u32line: " << u32line << std::endl;
                        exit(0);
                    } else std::cout << "------------------pointers taken------------------" << std::endl;
                    
                    std::getline(fin, line); //-------------------------------------------------------------------------------------------

                    while (std::getline(fin, line)){
                        u32line = to_u32(line);
                        if (u32line[0]==U'-') break;
                        std::istringstream iss(line.substr(nameptr));
                        std::string name, surname;
                        iss >> name >> surname;
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
                        if (!isNumber(placeStr)) continue; // "в/к, -"
                        int place = stoi(placeStr);

                        iss.clear();
                        iss.str(to_utf8(u32line.substr(timePtr)));
                        std::string resTimeStr;
                        iss >> resTimeStr;
                        // std::cout << "resTimeStr: " << resTimeStr <<  std::endl; 
                        int resTime = getTime(resTimeStr);

                        std::u32string key = u32name + U" " + u32surname + U" " + to_u32(date);
                        if (place == 1){
                            leader = resTime;
                            allBase[key].add_points(u32name, u32surname, getGroup(group[0], date), date, pointsClass, nowid);
                        } else{
                            int addPoints = std::max<int>(round((long double)pointsClass * (2.0 * leader / resTime - 1)), 0);
                            allBase[key].add_points(u32name, u32surname, getGroup(group[0], date), date, addPoints, nowid);
                        }
                    }
                    break;
                }
            }
        }
    }
}