#include "parser.h"
#include "model.h"
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

int getTime (const std::string hmmss){
    return stoi(hmmss.substr(0,1))*3600 + stoi(hmmss.substr(1,2))*60 + stoi(hmmss.substr(3,2));
}

bool isNumber(const std::string& s){
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::string getGroup (char gender, int date){
    int groupNum = nowYear - date;
    groupNum += (groupNum & 1);
    return std::string(gender, 1) + std::to_string(groupNum);
}

void parse(std::string link, int nowid){
    downloadPage(link);
    if (typeResult==TypeResult::debug){
        std::cout << "--------------------------------------------------" << std::endl;
        std::cout << "Downloaded " << link << std::endl;
        std::cout << "--------------------------------------------------" << std::endl;
    }

    for (auto group : groups){
        std::ifstream fin("build/page.html");
        std::string line;

        bool findGroup = false;
        while (std::getline(fin, line) and !findGroup) { 
            if (line.substr(0,4) == "<h2>") {
                int ptr = line.find("</span>") + 7;
                const size_t len = group.size();

                std::cout << "find <h2>: " << std::endl;
                std::cout << "    " << group << ' ' << ptr << ' ' << len << std::endl;
                std::cout << "        " << line << std::endl;

                if (line.find(group) != std::string::npos) {
                    std::cout << "find group: " << group << std::endl;
                    findGroup = true;
                    std::getline(fin, line); //<pre>
                    std::getline(fin, line); //Параметры дистанции:
                    std::getline(fin, line); //-------------------------------------------------------------------------------------------
                    std::getline(fin, line);
                    int nameptr = line.find("Фамилия");
                    int datePtr = line.find("Г.");
                    int timePtr = line.find("Результат");
                    int placePtr = line.find("Место");
                    int leader = -1;

                    if (nameptr == -1 or datePtr == -1 or timePtr == -1 or placePtr == -1){
                        std::cerr << "Parser::ptr error: " << group << ' ' << link << std::endl;
                        std::cerr << "nameptr: " << nameptr << std::endl; 
                        std::cerr << "datePtr: " << datePtr << std::endl; 
                        std::cerr << "timePtr: " << timePtr << std::endl; 
                        std::cerr << "placePtr: " << placePtr << std::endl; 
                        exit(0);
                    }
                    
                    std::getline(fin, line); //-------------------------------------------------------------------------------------------

                    while (std::getline(fin, line)){
                        if (line=="</pre>") break;
                        std::istringstream iss(line.substr(nameptr));
                        std::string name, surname;
                        iss >> name >> surname;

                        iss.clear();
                        iss.str(line.substr(datePtr));
                        std::string dateStr;
                        iss >> dateStr;
                        int date = stoi(dateStr);

                        iss.clear();
                        iss.str(line.substr(timePtr));
                        std::string resTimeStr;
                        iss >> resTimeStr;
                        int resTime = getTime(resTimeStr);

                        iss.clear();
                        iss.str(line.substr(placePtr));
                        std::string placeStr;
                        iss >> placeStr;
                        if (!isNumber(placeStr)) continue;
                        int place = stoi(placeStr);

                        std::cout << "zaebis" << std::endl;

                        std::string key = name + " " + surname + " " + std::to_string(date);
                        if (place == 1){
                            leader = resTime;
                            base[key].add_points(name, surname, getGroup(group[0], date), date, pointsClass, nowid);
                        } else{
                            int addPoints = std::max<int>(round((long double)pointsClass * (2.0 * leader / resTime - 1)), 0);
                            base[key].add_points(name, surname, getGroup(group[0], date), date, addPoints, nowid);
                        }
                    }
                    break;
                }
            }
        }
    }
}