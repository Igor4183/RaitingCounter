#include "database_reader.h"

void readDatabase(Competition& page, int colNumb){
    for (std::u32string group : groups){
        sqlite3_stmt* stmt = nullptr;
        Result leader;

        std::string sql = R"(
SELECT name, birth_year, club, total_time, place, status
FROM )" + to_utf8(page.nameTable) + R"(
    JOIN athlete USING(athlete_id, birth_year)
WHERE group_name = ')" + to_utf8(group) + R"('
ORDER BY place NULLS LAST;
)";
        
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        // std::cout << sql << std::endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = getText(stmt, 0);
            int birth_year = getInt(stmt, 1);
            std::string club = getText(stmt, 2);
            int total_time = getInt(stmt, 3);
            int place = getInt(stmt, 4);
            TypeResult status = static_cast<TypeResult>(getInt(stmt, 5));

            // std::cout << name << ' ' << birth_year << ' ' << club << ' ' << total_time << ' ' << place << ' ' << static_cast<int>(status) << '\n';
            std::u32string key = to_u32(name) + U" " + to_u32(birth_year);
            if (leader.place == -1 and place == 1) leader.getScore(total_time, place, leader, &page);
            if (bigBase.find(key) == bigBase.end()) bigBase[key].makeAthlete(to_u32(name), group, birth_year);
            bigBase[key].add_points(page, total_time, place, leader, colNumb, status);
        }
        sqlite3_finalize(stmt);
    }
}