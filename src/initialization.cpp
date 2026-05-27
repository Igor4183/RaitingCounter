#include "initialization.h"

void init(){
    system("mkdir -p build");
    sqlite3_open("main_base.db", &db);

    std::string sql = R"(
CREATE TABLE athlete (
    athlete_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT,  
    birth_year INTEGER, 
    category TEXT
);
)";
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}