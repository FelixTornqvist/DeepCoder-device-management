#include <iostream>
#include <string>
#include <experimental/optional>
#include <sstream>
#include <fstream>
#include "dsl/programloader.h"
#include "dsl/ast.h"
#include "dsl/interpreter.h"
#include "dsl/utils.h"

using namespace std;
using namespace dsl;

void doEval(Program& prog, Input& input) {
    auto result = eval(prog, input);

    if (!result) {
        cout << "null" << endl;
    } else {
        Output o = result.value();

        experimental::optional<int> i = o.integer();
        experimental::optional<std::vector<int>> l = o.list();

        if (i) {
            int val = i.value();
            cout << val << endl;
        }
        if (l) {
            vector<int> vals = l.value();
            cout << '[';
            for (int i = 0; i < vals.size() - 1; i++) {
                cout << vals[i] << ',';
            }
            if (vals.size() > 0)
                cout << vals[vals.size() - 1];
            cout << ']' << endl;
        }
    }
}

Input readInput() {
    string line;
    getline(cin, line);
    Input input;

    string numStr;
    bool listMode = false;
    vector<int> list;
    for (char c : line) {
        if (listMode) {
            if (c == ',') {
                int number = stoi(numStr);
                list.push_back(number);
                numStr = "";
            } else if (c == ']') {
                int number = stoi(numStr);
                list.push_back(number);
                numStr = "";
                listMode = false;
                input.push_back(list);
                list = {};
            } else {
                numStr += c;
            }

        } else {
            if (c == ',' ) {
                if (numStr != "") {
                    int number = stoi(numStr);
                    input.push_back(number);
                    numStr = "";
                }
            } else if (c == '[') {
                listMode = true;
            } else {
                numStr += c;
            }
        }
    }
    if (numStr != "")
        input.push_back(stoi(numStr));

    return input;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "ERROR: supply only one DSL sourcecode file" << endl;
        return -1;
    }

    string fileName(argv[1]);
    Program prog = load_program(fileName);
    Input input;

    while (true) {
        Input input = readInput();
        doEval(prog, input);
    }
}
