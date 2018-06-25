#include <sstream>
#include <fstream>
#include "dsl/programloader.h"
#include "dsl/utils.h"

using namespace dsl;

namespace dsl {
    Program load_program(std::string fname) {
        Program prog;

        std::ifstream fileStream;
        fileStream.open(fname);
        while (fileStream) {
            std::string line;
            getline(fileStream, line);

            std::istringstream iss(line);
            std::string s;

            iss >> s;
            if (s.length() < 1)
                break;
            Variable var = funkify_variable(s);
            iss >> s; // reads: <-
            iss >> s;
            Function func = funkify_function(s);
            std::vector<Argument> args = read_arguments(func, iss);

            Statement stmt(var, func, args);

            prog.push_back(stmt);
        }
        return prog;
    }

// Hopefully this can read all kinds of arguments..
    std::vector<Argument> read_arguments(const Function& func, std::istringstream& iss) {
        std::vector<Argument> args;
        std::string s;
        iss >> s;

        if (func == Function::Head) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Last) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Take) {
            args.push_back(funkify_variable(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::Access) {
            args.push_back(funkify_variable(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::Minimum) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Maximum) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Reverse) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Sort) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Sum) {
            args.push_back(funkify_variable(s));

        } else if (func == Function::Map) {
            args.push_back(funkify_oneArgumentLambda(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::Filter) {
            args.push_back(funkify_predicateLambda(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::Count) {
            args.push_back(funkify_predicateLambda(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::ZipWith) {
            args.push_back(funkify_twoArgumentsLambda(s));
            iss >> s;
            args.push_back(funkify_variable(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::Scanl1) {
            args.push_back(funkify_twoArgumentsLambda(s));
            iss >> s;
            args.push_back(funkify_variable(s));

        } else if (func == Function::ReadInt) {

        } else if (func == Function::ReadList) {

        } else if (func == Function::Rest) {
            args.push_back(funkify_variable(s));

        }
        return args;
    }

}

