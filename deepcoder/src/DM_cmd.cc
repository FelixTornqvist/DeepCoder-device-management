#include <iostream>
#include "dsl/interpreter.h"
#include "device_manager.h"

using namespace std;
using namespace dm;

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        cout << "ERROR: supply only one DSL sourcecode file" << endl;
        return -1;
    }
    cout << "starting Device Manager" << endl;

    string fileName(argv[1]);
    device_manager dm(fileName);

    while (cin) {
        string line;
        getline(cin, line);
    
        int splitPos = line.find_first_of(' ');
        string cmd = line.substr(0, splitPos);
        line = line.substr(splitPos + 1, line.length());
        
        if (cmd == "addDev") {
            dm.addDev(dm.getInput(line));

        } else if (cmd == "addCon") {
            int splitPos = line.find_first_of(' ');
            string context = line.substr(0, splitPos);
            int devClusterID = stoi(line.substr(splitPos + 1, line.length()));
            dm.addCon(context, devClusterID);

        } else if (cmd == "rmCon") {
            int splitPos = line.find_first_of(' ');
            string context = line.substr(0, splitPos);
            dm.rmCon(context);

        } else if (cmd == "rmDev") {
            dsl::Input input = dm.getInput(line);
            dm.rmDev(input);

        } else if (cmd == "list") {
            cout << "Context clustering is:" << endl;
            dm.print_context_clusterings();
            cout << "Device clustering is:" << endl;
            dm.print_device_clusterings();

        } else if (cmd == "end") {
            return 0;

        } else {
            cout << "Unknown command" << endl;
            continue;
        }
    }
    
    return 0;
}
