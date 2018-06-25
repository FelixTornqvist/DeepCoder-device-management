#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include "dsl/utils.h"
#include "dataset-generator.h"
#include "attribute.h"

using namespace std;
using namespace dsl;

void output_value(const Value &value, ofstream &fout) {
    if (value.integer()) {
        fout << value.integer().value();
    } else {
        auto l = value.list().value();
        fout << "[";
        for (auto i = 0; i < l.size(); i++) {
            auto x = l.at(i);

            fout << x;
            if (i != (l.size() - 1)) {
                fout << ",";
            }
        }
        fout << "]";
    }
}
void output_input(const Input &input, ofstream &fout) {
    fout << "[";
    for (auto i = 0; i < input.size(); i++) {
        auto x = input.at(i);

        output_value(x, fout);
        if (i != (input.size() - 1)) {
            fout << ",";
        }
    }
    fout << "]";
}

void output_attribute(const Attribute &attr, ofstream &fout) {
    std::vector<double> vec = attr;
    fout << "[";
    for (auto i = 0; i < vec.size(); i++) {
        fout << vec.at(i);
        if (i != (vec.size() - 1)) {
            fout << ",";
        }
    }
    fout << "]";
}

int main(int argc, char **argv) {
    size_t max_length = 4;
    size_t dataset_size = 0;
    size_t example_pair_per_program = 1;
    ofstream fout;

    if (argc >= 2) {
        max_length = atoi(argv[1]);
    }
    if (argc >= 3) {
        dataset_size = atoi(argv[2]);
    }
    if (argc >= 4) {
        example_pair_per_program = atoi(argv[3]);
    }
    fout.open(argv[4]);

    cerr << "Generate dataset\n" << "  Max-Length: " << max_length << "\n  Dataset-Size: " << dataset_size << endl;
    auto dataset = generate_dataset(1, max_length, dataset_size, example_pair_per_program * EXAMPLE_NUM);

    fout << "[" << endl;
    if (dataset) {
        auto x = dataset.value();
        long long int cnt = 0;
        for (const auto &p: x.programs) {
            cnt += 1;
            const auto &program = p.first;
            const auto &examples = p.second;
            auto attribute = Attribute(program);
            cerr << "# Program\n" << program << flush;
            auto pair_num = examples.size() / EXAMPLE_NUM;
            for (auto j = 0; j < pair_num; ++j) {
                fout << "{\"examples\":[" << endl;
                for (auto k = 0; k < EXAMPLE_NUM; ++k) {
                    const auto &example = examples.at(j * EXAMPLE_NUM + k);

                    fout << "{\"input\":";
                    output_input(example.input, fout);
                    fout << ",\"output\":";
                    output_value(example.output, fout);
                    fout << "}";
                    if (k != EXAMPLE_NUM - 1) {
                        fout << ",";
                    }
                    fout << endl;

                }
                fout << "]," << endl << "\"attribute\":";
                output_attribute(attribute, fout);

                fout << "}";
                if (cnt != x.programs.size() ||
                    j != pair_num - 1) {
                    fout << ",";
                }
                fout << endl << flush;
            }
        }
    } else {
        cerr << "Fail to generate dataset" << endl;
    }
    fout << "]" << endl;
    fout.close();

    return 0;
}
