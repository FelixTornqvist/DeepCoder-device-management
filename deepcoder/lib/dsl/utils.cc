#include <string>
#include <algorithm>
#include <math.h>
#include "dsl/utils.h"

using namespace std;

namespace dsl {
    string stringify(const Function &func) {
        switch (func) {
            case Function::Head:
                return "head";
            case Function::Last:
                return "last";
            case Function::Take:
                return "take";
            case Function::Drop:
                return "drop";
            case Function::Access:
                return "access";
            case Function::Minimum:
                return "minimum";
            case Function::Maximum:
                return "maximum";
            case Function::Reverse:
                return "reverse";
            case Function::Sort:
                return "sort";
            case Function::Sum:
                return "sum";
            case Function::Map:
                return "map";
            case Function::Filter:
                return "filter";
            case Function::Count:
                return "count";
            case Function::ZipWith:
                return "zip_with";
            case Function::Scanl1:
                return "scanl1";
            case Function::ReadInt:
                return "read_int";
            case Function::ReadList:
                return "read_list";
            case Function::Rest:
                return "rest";
        }
    }

    std::string stringify(const OneArgumentLambda &lambda) {
        switch (lambda) {
            case OneArgumentLambda::Plus1:
                return "+1";
            case OneArgumentLambda::Minus1:
                return "-1";
            case OneArgumentLambda::Multiply2:
                return "*2";
            case OneArgumentLambda::Multiply3:
                return "*3";
            case OneArgumentLambda::Multiply4:
                return "*4";
            case OneArgumentLambda::MultiplyMinus1:
                return "*(-1)";
            case OneArgumentLambda::Divide2:
                return "/2";
            case OneArgumentLambda::Divide3:
                return "/3";
            case OneArgumentLambda::Divide4:
                return "/4";
            case OneArgumentLambda::Pow2:
                return "**2";
        }
    }

    std::string stringify(const TwoArgumentsLambda &lambda) {
        switch (lambda) {
            case TwoArgumentsLambda::Plus:
                return "+";
            case TwoArgumentsLambda::Minus:
                return "-";
            case TwoArgumentsLambda::Multiply:
                return "*";
            case TwoArgumentsLambda::Min:
                return "MIN";
            case TwoArgumentsLambda::Max:
                return "MAX";
        }
    }

    std::string stringify(const PredicateLambda &pred) {
        switch (pred) {
            case PredicateLambda::IsPositive:
                return ">0";
            case PredicateLambda::IsNegative:
                return "<0";
            case PredicateLambda::IsOdd:
                return "%2 == 1";
            case PredicateLambda::IsEven:
                return "%2 == 0";
        }
    }

    std::string stringify(const Variable &var) {
        std::string str;

        Variable x = var;

        while (x >= 26) {
            str += (x % 26) + 'a';
            x /= 26;
        }

        if (var >= 26) {
            x -= 1;
        }

        str += x + 'a';

        reverse(str.begin(), str.end());

        return str;
    }

    Function funkify_function(const string &func) {
        if("head" == func) {
            return Function::Head;
        } else if ("last" == func) {
            return Function::Last;
        } else if ("take" == func) {
            return Function::Take;
        } else if ("drop" == func) {
            return Function::Drop;
        } else if ("access" == func) {
            return Function::Access;
        } else if ("minimum" == func) {
            return Function::Minimum;
        } else if ("maximum" == func) {
            return Function::Maximum;
        } else if ("reverse" == func) {
            return Function::Reverse;
        } else if ("sort" == func) {
            return Function::Sort;
        } else if ("sum" == func) {
            return Function::Sum;
        } else if ("map" == func) {
            return Function::Map;
        } else if ("filter" == func) {
            return Function::Filter;
        } else if ("count" == func) {
            return Function::Count;
        } else if ("zip_with" == func) {
            return Function::ZipWith;
        } else if ("scanl1" == func) {
            return Function::Scanl1;
        } else if ("read_int" == func) {
            return Function::ReadInt;
        } else if ("read_list" == func) {
            return Function::ReadList;
        } else if ("rest" == func) {
            return Function::Rest;
        }
    }

    OneArgumentLambda funkify_oneArgumentLambda(const std::string &lambda) {
        if ("+1" == lambda) {
            return OneArgumentLambda::Plus1;
        } else if ("-1" == lambda) {
            return OneArgumentLambda::Minus1;
        } else if ("*2" == lambda) {
            return OneArgumentLambda::Multiply2;
        } else if ("*3" == lambda) {
            return OneArgumentLambda::Multiply3;
        } else if ("*4" == lambda) {
            return OneArgumentLambda::Multiply4;
        } else if ("*(-1)" == lambda) {
            return OneArgumentLambda::MultiplyMinus1;
        } else if ("/2" == lambda) {
            return OneArgumentLambda::Divide2;
        } else if ("/3" == lambda) {
            return OneArgumentLambda::Divide3;
        } else if ("/4" == lambda) {
            return OneArgumentLambda::Divide4;
        } else if ("**2" == lambda) {
            return OneArgumentLambda::Pow2;
        }
    }

    TwoArgumentsLambda funkify_twoArgumentsLambda(const std::string &lambda) {
        if ("+" == lambda) {
            return TwoArgumentsLambda::Plus;
        } else if ("-" == lambda) {
            return TwoArgumentsLambda::Minus;
        } else if ("*" == lambda) {
            return TwoArgumentsLambda::Multiply;
        } else if ("MIN" == lambda) {
            return TwoArgumentsLambda::Min;
        } else if ("MAX" == lambda) {
            return TwoArgumentsLambda::Max;
        }
    }

    PredicateLambda funkify_predicateLambda(const std::string &pred) {
        if (">0" == pred) {
            return PredicateLambda::IsPositive;
        } else if ("<0" == pred) {
            return PredicateLambda::IsNegative;
        } else if ("%2 == 1" == pred) {
            return PredicateLambda::IsOdd;
        } else if ("%2 == 0" == pred) {
            return PredicateLambda::IsEven;
        }
    }

    // TODO: implement to fully work with stringify(Variable), or re-implement stringify(Variable),
    // they only work the same below "aaa".
    Variable funkify_variable(const std::string &str) {
        string nstr = str;
        Variable x = nstr.back() - 'a';
        nstr.pop_back();
        reverse(nstr.begin(), nstr.end());

        int n = 1;
        for (char c : nstr) {
            int i = (c - 'a' + 1);
            x += pow(26, n) * i;
            n++;
        }

        return x;
    }

    std::ostream &operator<<(std::ostream &stream, const Argument &argument) {
        if (argument.one_argument_lambda()) {
            stream << stringify(argument.one_argument_lambda().value());
        } else if (argument.two_arguments_lambda()) {
            stream << stringify(argument.two_arguments_lambda().value());
        } else if (argument.predicate()) {
            stream << stringify(argument.predicate().value());
        } else if (argument.variable()) {
            stream << stringify(argument.variable().value());
        }

        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const Statement &statement) {
        stream << stringify(statement.variable) << " <- " << stringify(statement.function);
        for (const auto &arg: statement.arguments) {
            stream << " " << arg;
        }

        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const Program &program) {
        stream << "---\n";
        for (const auto &statement: program) {
            stream << statement << "\n";
        }
        stream << "---\n";

        return stream;
    }

    std::ostream &operator<<(std::ostream &stream, const Value &value) {
        if (value.integer()) {
            stream << value.integer().value();
        } else if (value.list()) {
            stream << "[";
            auto l = value.list().value();

            for (auto i = 0; i < l.size(); i++) {
                if (i != 0) {
                    stream << ",";
                }
                stream << l[i];
            }

            stream << "]";
        } else {
            stream << "NULL";
        }

        return stream;
    }
    std::ostream &operator<<(std::ostream &stream, const Input &input) {
        stream << "---\n";

        for (const auto& i: input) {
            stream << i << "\n";
        }

        stream << "---\n";

        return stream;
    }



}
