#pragma once
#include "model.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using json = nlohmann::json;

void readConfig();
