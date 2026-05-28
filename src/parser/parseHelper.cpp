#include "parseHelper.h"

int getTime (const std::string hmmss){ // h:mm:ss
    // std::string hmmss = to_utf8(u32hmmss);
    return stoi(hmmss.substr(0,1))*3600 + stoi(hmmss.substr(2,2))*60 + stoi(hmmss.substr(5,2));
} 

bool isNumber(const std::string s){
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::string getText(sqlite3_stmt* stmt, int col) {
    const unsigned char* text = sqlite3_column_text(stmt, col);
    return text ? reinterpret_cast<const char*>(text) : "";
}

int getInt(sqlite3_stmt* stmt, int col) { // NULL == -1
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL)
        return -1;
    return sqlite3_column_int(stmt, col);
}
