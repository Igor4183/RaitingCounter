#include "config.h"
#include "model.h"
#include <iostream>
#include <string>
#include <vector>

void downloadPage(std::string link){
    std::string path = "build/page.html";
    std::string cmd = "curl https://example.com > " + path;

    system("mkdir -p build"); // лишнее
    system(cmd.c_str());
}