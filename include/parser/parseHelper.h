#pragma once
#include "model.h"

int getTime (const std::string hmmss);
bool isNumber(const std::string s);
std::string getText(sqlite3_stmt* stmt, int col);
int getInt(sqlite3_stmt* stmt, int col);