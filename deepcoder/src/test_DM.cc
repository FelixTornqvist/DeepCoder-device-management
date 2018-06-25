#include <iostream>
#include <chrono>
#include <cstdlib>
#include <vector>
#include "dsl/interpreter.h"
#include "device_manager.h"

using namespace dsl;
using namespace std::chrono;
using namespace std;
using namespace dm;

Input generateRandomInput(int amt) {
    Input input;
    vector<int> numbers(amt);
    for (int i = 0; i < amt; i++) {
        int rnd = rand() % 10;
        numbers.push_back(rnd);
    }
    input.push_back(numbers);
    return input;
}

vector<Input> generateRandomInputs(int typeAmt, int noOfEachType, int SASize) {
    vector<Input> inputs;
    for (int i = 0; i < typeAmt; i++) {
        for (int j = 0; j < noOfEachType; j++) {
            vector<int> numbers(SASize);
            numbers[0] = i;
            for (int i = 1; i < SASize; i++) {
                int rnd = rand() % 10;
                numbers[i] = rnd;
            }
            Input tmp;
            tmp.push_back(numbers);
            inputs.push_back(tmp);
        }
    }
    return inputs;
}

long addSAs(int amount, int noOfEachType, string program) {
    device_manager dm(program);
    vector<Input> inputs = generateRandomInputs(amount, noOfEachType, 5);
    auto clock = high_resolution_clock();
    auto start = clock.now();
    for (int i = 0; i < amount; i++) {
        dm.addDev(inputs[i]);
    }
    auto stop = clock.now();
    auto duration = duration_cast<nanoseconds>(stop-start);
    return duration.count();
}

device_manager* generateFilledDM(int sas, int noOfEachType, string program) {
    device_manager *dm = new device_manager(program);
    vector<Input> inputs = generateRandomInputs(sas, noOfEachType, 5);
    for (int i = 0; i < inputs.size(); i++) {
        Input input = inputs[i];
        dm->addDev(input);
    }
    return dm;
}

void addOneSAtoDM(string file, int testAmt, int sas, int noOfEachType) {
    cout << "adding 1 SA to datastructure containing " << sas << " types of SAs, with " << noOfEachType << " instances of each type (" << testAmt << " tests)" << endl;
    for (int i = 0; i < testAmt; i++) {
        device_manager* dm = generateFilledDM(sas, noOfEachType, file);
        Input newDev = generateRandomInput(5);
        auto clock = high_resolution_clock();

        auto start = clock.now();
        dm->addDev(newDev);
        auto stop = clock.now();
        auto duration = duration_cast<nanoseconds>(stop-start);

        delete dm;
        cout << duration.count() << endl;
    }
}

int main(int argc, const char *argv[]) {
    string fileName(argv[1]);

    addOneSAtoDM(fileName, 100, 10, 10);
    addOneSAtoDM(fileName, 100, 10000, 10);

    return 0;
}
