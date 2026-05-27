#include "data_importer.h"

bool checkTable(std::string nameTable){
    sqlite3_stmt* stmt;
    std::string sql = 
R"(
SELECT name 
FROM sqlite_master
R"(
WHERE type='table' AND name=')" + nameTable + R"(';
)";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    bool res = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return res;
}

std::u32string deleteSpaces(std::u32string str){
    while (!str.empty() and str.back() == ' ') str.pop_back();
    size_t ptr = 0;
    while (str.size() > ptr and str[ptr]==U' ') ptr++;
    str = str.substr(ptr);
    return str;
}

int getAthleteId(std::u32string name, int date){
    sqlite3_stmt* stmt = nullptr;

    std::string insertSQL = "INSERT OR IGNORE INTO athlete(name, birth_year) VALUES(?, ?);";
    sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, to_utf8(name).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, date);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    std::string selectSQL = "SELECT athlete_id FROM athlete WHERE name = ? AND birth_year = ?;";
    stmt = nullptr;

    sqlite3_prepare_v2(db, selectSQL.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, to_utf8(name).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, date);
    int athlete_id;
    if (sqlite3_step(stmt) == SQLITE_ROW) athlete_id = sqlite3_column_int(stmt, 0);
    else {
        std::cerr << "parser/data_importer.cpp::not fined athlete_id " << name << ' ' << date << '\n';
        exit(0);
    }
    sqlite3_finalize(stmt);

    return athlete_id;
}

void importData(Competition& page){
    if (checkTable(to_utf8(page.nameTable))) return;

    {
        std::string sql = 
R"(
CREATE TABLE )"
+ to_utf8(page.nameTable) +
R"(
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    athlete_id INTEGER,
    birth_year INTEGER,
    club TEXT,
    group_name TEXT,
    total_time INTEGER,
    place INTEGER,
    status TEXT,
    FOREIGN KEY (athlete_id) REFERENCES athlete(athlete_id) ON DELETE SET NULL
);
)";
        char* err = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
        if (rc != SQLITE_OK) {
            std::cout << "SQL error: " << err << "\n";
            sqlite3_free(err);
        }
    }

    std::string link = to_utf8(page.url);
    std::string path = "build/" + to_utf8(page.nameTable) + ".html";
    std::string cmd = "curl " + link + " | iconv -f WINDOWS-1251 -t UTF-8 > " + path; 
    system(cmd.c_str());
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Downloaded " << page.url << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (std::u32string group : groups){
        std::ifstream fin("build/" + to_utf8(page.nameTable) + ".html");
        std::string line;
        std::u32string u32line;

        bool findGroup = false;
        while (std::getline(fin, line) and !findGroup) { 
            u32line = to_u32(line);
            if (line.substr(0,4) == "<h2>" and u32line.find(group) != std::u32string::npos) {
                // std::cout << "find group: " << group << std::endl;
                findGroup = true;
                std::getline(fin, line); //<pre>
                std::getline(fin, line); //Параметры дистанции:
                std::getline(fin, line); //-------------------------------------------------------------------------------------------
                std::getline(fin, line);
                u32line = to_u32(line);
                int nameptr = u32line.find(U"Фамилия");
                int clubptr = u32line.find(U"Команда");
                int dateptr = u32line.find(U"Г.");
                int timeptr = u32line.find(U"Результат");
                int placeptr = u32line.find(U"Место");
                Result leader;

                if (nameptr == -1 or clubptr == -1 or dateptr == -1 or timeptr == -1 or placeptr == -1){
                    std::cerr << "Parser::ptr error: " << group << ' ' << page.url << std::endl;
                    std::cerr << "nameptr: " << nameptr << std::endl; 
                    std::cerr << "clubptr: " << nameptr << std::endl; 
                    std::cerr << "datePtr: " << dateptr << std::endl; 
                    std::cerr << "timePtr: " << timeptr << std::endl; 
                    std::cerr << "placePtr: " << placeptr << std::endl; 
                    std::cerr << "   line: " << line << std::endl;
                    std::cerr << "u32line: " << u32line << std::endl;
                    exit(0);
                }
                
                std::getline(fin, line); //-------------------------------------------------------------------------------------------

                while (std::getline(fin, line)){
                    u32line = to_u32(line);
                    if (u32line[0]==U'-') break;

                    std::u32string name = deleteSpaces(u32line.substr(nameptr, clubptr - nameptr));
                    std::u32string club = deleteSpaces(u32line.substr(clubptr, dateptr - clubptr));
                    int date = stoi(to_utf8(u32line.substr(dateptr, 4)));

                    std::string placeStr = to_utf8(deleteSpaces(u32line.substr(placeptr, 5)));
                    TypeResult status = TypeResult::valid;
                    if (!isNumber(placeStr)){
                        if (placeStr == "-") status = TypeResult::removed;
                        else status = TypeResult::outOfCompetition;
                    }

                    int resTime = -1;
                    if (status ==TypeResult::valid) resTime = getTime(to_utf8(deleteSpaces(u32line.substr(timeptr, placeptr - timeptr))));

                    int athlete_id = getAthleteId(name, date);

                    sqlite3_stmt* stmt = nullptr;
                    std::string sql = "INSERT INTO " + to_utf8(page.nameTable) + "(athlete_id, birth_year, club, group_name, total_time, place, status) VALUES(?, ?, ?, ?, ?, ?, ?);";
                    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
                    sqlite3_bind_int(stmt, 1, athlete_id);
                    sqlite3_bind_int(stmt, 2, date);
                    sqlite3_bind_text(stmt, 3, to_utf8(club).c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_text(stmt, 4, to_utf8(group).c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmt, 5, resTime);
                    if(status==TypeResult::valid) {
                        sqlite3_bind_int(stmt, 6, stoi(placeStr));
                        sqlite3_bind_text(stmt, 7, "valid", -1, SQLITE_TRANSIENT);
                    }
                    else {
                        sqlite3_bind_null(stmt, 6);
                        if (status == TypeResult::outOfCompetition )sqlite3_bind_text(stmt, 7, "outOfCompetition", -1, SQLITE_TRANSIENT);
                        else if (status == TypeResult::removed )sqlite3_bind_text(stmt, 7, "removed", -1, SQLITE_TRANSIENT);
                        else if (status == TypeResult::undefined )sqlite3_bind_text(stmt, 7, "undefined", -1, SQLITE_TRANSIENT);
                    }

                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
                break;
            }
        }
    }
}
/*
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    athlete_id INTEGER,
    birth_year INTEGER,
    club TEXT,
    group_name TEXT,
    total_time INTEGER,
    place INTEGER,
    status TEXT,
    FOREIGN KEY (athlete_id) REFERENCES athlete(athlete_id) ON DELETE SET NULL
*/