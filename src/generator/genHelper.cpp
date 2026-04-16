#include <genHelper.h>

std::u32string makeTime(int time){
    if (time == -1) return U"--:--";
    std::u32string res = to_u32(time/60) + U":";
    if (time%60<10) res+=U"0";
    res+=to_u32(time%60);
    return res;
}

std::u32string makeRes(int place, int time){
    std::u32string res = makeTime(time) + U" | ";
    if (place <= 0) res += U"-";
    else res += to_u32(place);
    // res += U" место";
    return res;
}

void myError(std::string m){
    std::cerr << m << std::endl;
    exit(0);
}

void td(std::ofstream& out, const std::string text) {
    out << "<td>" << text << "</td>";
}