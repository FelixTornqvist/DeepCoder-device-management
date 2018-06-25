#include <vector>
#include <string>
#include "dsl/ast.h"
#pragma once

namespace dsl {
    std::vector<Argument> read_arguments(const Function& func, std::istringstream& iss);
    Program load_program(std::string fname);
}
