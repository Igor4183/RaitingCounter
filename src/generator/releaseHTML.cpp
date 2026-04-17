#include <releaseHTML.h>

std::u32string genLink(std::u32string group){
    std::u32string link;
    for (auto c : group){
        if (c==U'М' or c==U'м') link+=U'm';
        else if (c==U'Ж' or c==U'ж') link+=U'w';
        else link+=c;
    }
    return link;
}

void addOut (std::ofstream& out, std::u32string& sample, std::u32string targ, int& last, int &now){
    now = sample.find(targ, last);
    if (now == -1) myError("releaseHTML::" +to_utf8(targ) + "error");
    out << sample.substr(last, now-last);
    last = now + targ.size();
}

void writeHeader(std::ofstream& out){
    std::ifstream in("template/releaseTemplate.html");

    std::ostringstream ss;
    ss << in.rdbuf();
    std::u32string sample = to_u32(ss.str());

    int last = 0, now;

    addOut(out, sample, U"(flag1)", last, now);
    out << cntCompetitions;

    addOut(out, sample, U"(flag2)", last, now);
    out << groupsInJSON.size();

    addOut(out, sample, U"(flag3)", last, now);
    out << cntAthletes;

    addOut(out, sample, U"(flag4)", last, now);
    out << bestScore;

    addOut(out, sample, U"<nav class=\"tabs\">", last, now);
}

void writeTabs(std::ofstream& out){ 
    out << "<nav class=\"tabs\">\n";
    for (std::u32string group : groupsInJSON){
        std::u32string link = genLink(group);
        out << "      <a href=\"#" << link << "\">" << group << "</a>\n";
    }
    out << "    </nav>\n\n";
}

void writeGroupStart(std::ofstream& out, std::u32string group){
    std::ifstream in("template/releaseTemplate.html");

    std::ostringstream ss;
    ss << in.rdbuf();
    std::u32string sample = to_u32(ss.str());

    int last = sample.find(U"<!--flag5-->"), now;
    if (last == -1) myError("releaseHTML::writeGroupStart <!--flag5--> error");
    last += 12;

    addOut(out, sample, U"link", last, now);
    out << genLink(group);

    addOut(out, sample, U"groupInJSON", last, now);
    out << group;

    addOut(out, sample, U"group.size()", last, now);
    out << groupBase[group].size() << " participants";

    addOut(out, sample, U"              <th><a href=\"link1.html\" target=\"_blank\">1 старт</a></th>", last, now);
    for (Competition& page : pages){
        out << "              <th><a href=\"" << page.url << "\" target=\"_blank\">" << page.title << "</a></th>\n";
    }
    out << "            </tr>\n          </thead>\n          <tbody>\n";
}

std::string checkRes(std::vector<int>& idxs, int targ){
    if (find(idxs.begin(), idxs.end(), targ) == idxs.end()) return "out";
    else return "in";
}

void writeRow(std::ofstream& out, Athlete& a, int top){
    if (top<=3) out << "            <tr class=\"top" << top << "\">\n";
    else out << "            <tr>\n";
    out << "              <td class=\"rank\">" << top << "</td>\n";
    out << "              <td class=\"name\">" << a.surname << ' ' << a.name << "<span class=\"sub\"></span></td>\n"; //<span class=\"sub\">club</span>
    out << "              <td>" << a.DOB << "</td>\n";
    out << "              <td class=\"sum\">" << a.sum << "</td>\n";
    for (int i = 0; i<cntCompetitions; i++){
        out << "              <td>\n";
        if (a.points[i].type == TypeResult::valid){
            out << "                <div class=\"res res-score " << checkRes(a.idxs, i) << "-sum\">\n";
            out << "                  <div class=\"res-main\">" << a.points[i].score << "</div>\n";
            out << "                  <div class=\"res-sub\">" << makeRes(a.points[i].place, a.points[i].time) << "</div>\n";
            out << "                </div>\n";
        } else{
            if (a.points[i].type == TypeResult::removed) out << "                <div class=\"res res-status\">снят</div>\n";
            if (a.points[i].type == TypeResult::outOfCompetition) out << "                <div class=\"res res-status\">в/к</div>\n";
            if (a.points[i].type == TypeResult::undefinded) out << "                <div class=\"res res-status\">-</div>\n"; 
        }

        out << "              </td>\n";
        // out << "              <td><span class=\"score " << checkRes(a.idxs, i) << "\">" << a.points[i].score;
        // out << "<small>" << makeRes(a.points[i].place, a.points[i].time) << "</small></span></td>\n";
    }
    out << "            </tr>\n";

}

void writeGroupEnd(std::ofstream& out){
    out << "          </tbody>\n        </table>\n      </div>\n    </section>\n";
}

void writeFooter(std::ofstream& out){
    std::ifstream in("template/releaseTemplate.html");

    std::ostringstream ss;
    ss << in.rdbuf();
    std::u32string sample = to_u32(ss.str());

    out << "    ";
    int ptr = sample.find(U"<footer class=\"footer\">");
    if (ptr==-1) myError("releaseHTML::writeFooter ptr==-1");
    out << sample.substr(ptr);
}

void makeReleaseHTML(){
    std::ofstream out("output/release result.html");

    writeHeader(out);
    writeTabs(out);

    for (std::u32string group : groupsInJSON) {
        writeGroupStart(out, group);
        int top = 1;
        for (Athlete& a : groupBase[group])
            writeRow(out, a, top++);
        writeGroupEnd(out);
    }

    writeFooter(out);
}