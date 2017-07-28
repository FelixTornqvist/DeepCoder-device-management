#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <tuple>
#include "dsl/utils.h"
#include "dataset-generator.h"
#include "random-enumerator.h"
#include "attribute.h"

using namespace std;
using namespace dsl;

void output_value(ofstream &stream, const Value &value) {
    if (value.integer()) {
        stream << value.integer().value();
    } else {
        auto l = value.list().value();
        stream << "[";
        for (auto i = 0; i < l.size(); i++) {
            auto x = l.at(i);

            stream << x;
            if (i != (l.size() - 1)) {
                stream << ",";
            }
        }
        stream << "]";
    }
}
void output_input(ofstream &stream, const Input &input) {
    stream << "[";
    for (auto i = 0; i < input.size(); i++) {
        auto x = input.at(i);

        output_value(stream, x);
        if (i != (input.size() - 1)) {
            stream << ",";
        }
    }
    stream << "]";
}

void output_attribute(const Attribute &attr) {
    std::vector<double> vec = attr;
    cout << "[";
    for (auto i = 0; i < vec.size(); i++) {
        cout << vec.at(i);
        if (i != (vec.size() - 1)) {
            cout << ",";
        }
    }
    cout << "]";
}

int main(int argc, char **argv) {
    size_t max_length = 4;
    size_t dataset_size = 100;

    if (argc < 2) {
        cout << "no-dir" << endl;
        return 1;
    }
    auto dir = argv[1];

    if (argc >= 3) {
        max_length = atoi(argv[2]);
    }
    if (argc >= 4) {
        dataset_size = atoi(argv[3]);
    }

    auto dataset_per_length = dataset_size / max_length;

    auto functions = all_functions;
    functions.erase(find(functions.begin(), functions.end(), Function::ReadInt));
    functions.erase(find(functions.begin(), functions.end(), Function::ReadList));

    // Enumerate read_{list, int}
    Restriction r_for_read;
    r_for_read.functions = { Function::ReadInt, Function::ReadList };

    Restriction r;
    r.functions = functions;
    r.predicates = all_predicate_lambdas;
    r.one_argument_lambda = all_one_argument_lambdas;
    r.two_arguments_lambda = all_two_arguments_lambdas;

    vector<tuple<Program, vector<Example>>> programs;
    programs.reserve(dataset_per_length * max_length);
    for (auto i = 1; i <= max_length; i++) {
        r_for_read.min_length = 1;
        r_for_read.max_length = i;
        r.min_length = i;
        r.max_length = i;
        while (programs.size() < dataset_per_length * i) {
            auto read = generate_random_program(r_for_read);
            if (read) {
                auto tenv = generate_type_environment(read.value());
                if (tenv) {
                    auto p = generate_random_program(r, read.value(), tenv.value());
                    if (p && !has_unused_variable(p.value())) {
                        auto examples = generate_examples(p.value(), EXAMPLE_NUM);
                        if (examples) {
                            auto es = examples.value();
                            programs.push_back(make_tuple(p.value(), es));
                        }
                    }
                }
            }
        }
    }

    for (auto i = 0 ; i < programs.size(); i++) {
        auto &x = programs[i];
        stringstream pfile;
        pfile << dir << "/" << i << "-program";
        ofstream ofs(pfile.str());
        ofs << get<0>(x);

        stringstream efile;
        efile << dir << "/" << i << "-example";
        ofstream ofs2(efile.str());
        ofs2 << "[\n";
        auto examples = get<1>(x);
        for (auto j = 0; j < examples.size(); j++) {
            ofs2 << "{\"input\":";
            output_input(ofs2, examples[j].input);
            ofs2<< ",\"output\":";
            output_value(ofs2, examples[j].output);
            ofs2 << "}";
            if (j != (examples.size() - 1)) {
                ofs2 << ",";
            }
            ofs2 << "\n";
        }
        ofs2 << "]";
    }

    /*
    cout << "[\n";
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
                cout << "{\"examples\":[\n";
                for (auto k = 0; k < EXAMPLE_NUM; ++k) {
                    const auto &example = examples.at(j * EXAMPLE_NUM + k);

                    cout << "{\"input\":";
                    output_input(example.input);
                    cout << ",\"output\":";
                    output_value(example.output);
                    cout << "}";
                    if (k != EXAMPLE_NUM - 1) {
                        cout << ",";
                    }
                    cout << "\n";

                }
                cout << "],\n\"attribute\":";
                output_attribute(attribute);

                cout << "}";
                if (cnt != x.programs.size() ||
                    j != pair_num - 1) {
                    cout << ",";
                }
                cout << "\n" << flush;
            }
        }
    } else {
        cerr << "Fail to generate dataset" << endl;
    }
    cout << "]" << endl;
     */
    return 0;
}