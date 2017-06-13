#include <gtest/gtest.h>
#include "enumerator.h"

using namespace std;
using namespace dsl;

TEST(EnumerateTest, EnumerateFunctionTest) {
    Restriction r = {
            1, 2,
            { Function::ReadInt, Function::ReadList },
            {}, {}, {}
    };

    vector<Program> ps;
    auto process = [&ps](const Program &p, const int &x) {
        ps.push_back(p);

        return true;
    };
    enumerate(r, [](const Program &p, const int &x) { return p.size(); }, process, 0);

    EXPECT_EQ(6, ps.size());
    EXPECT_TRUE(find_if(ps.begin(), ps.end(), [](const Program& p) {
        return p[0].function == Function::ReadInt;
    }) != ps.end());
}

TEST(EnumerateTest, EnumerateArgumentsTest) {
    Restriction r = {
            1, 2,
            { Function::ReadList, Function::Head },
            {}, {}, {}
    };

    vector<Program> ps;
    auto process = [&ps](const Program &p, const int &x) {
        ps.push_back(p);

        return true;
    };
    enumerate(r, [](const Program &p, const int &x) { return p.size(); }, process, 0);

    EXPECT_EQ(3, ps.size());
    EXPECT_TRUE(find_if(ps.begin(), ps.end(), [](const Program& p) {
        return p.size() == 2 && p[1].function == Function::Head && p[1].arguments[0].variable().value() == 0;
    }) != ps.end());
}

TEST(EnumerateTest, EnumerateLambdaTest) {
    Restriction r = {
            1, 2,
            { Function::ReadList, Function::Count },
            { PredicateLambda::IsNegative, PredicateLambda::IsPositive}, {}, {}
    };

    vector<Program> ps;
    auto process = [&ps](const Program &p, const int &x) {
        ps.push_back(p);

        return true;
    };
    enumerate(r, [](const Program &p, const int &x) { return p.size(); }, process, 0);

    EXPECT_EQ(4, ps.size());
    EXPECT_TRUE(find_if(ps.begin(), ps.end(), [](const Program& p) {
        return p.size() == 2 &&
                p[1].function == Function::Count && p[1].arguments[0].predicate().value() == PredicateLambda::IsNegative;
    }) != ps.end());
    EXPECT_TRUE(find_if(ps.begin(), ps.end(), [](const Program& p) {
        return p.size() == 2 &&
               p[1].function == Function::Count && p[1].arguments[0].predicate().value() == PredicateLambda::IsPositive;
    }) != ps.end());
}

TEST(EnumerateTest, MinLengthTest) {
    Restriction r = {
            2, 2,
            { Function::ReadInt, Function::ReadList },
            {}, {}, {}
    };

    vector<Program> ps;
    auto process = [&ps](const Program &p, const int &x) {
        ps.push_back(p);

        return true;
    };
    enumerate(r, [](const Program &p, const int &x) { return p.size(); }, process, 0);

    EXPECT_EQ(4, ps.size());
}

TEST(EnumerateTest, InformationTest) {
    Restriction r = {
            2, 2,
            { Function::ReadInt, Function::ReadList },
            {}, {}, {}
    };

    vector<Program> ps;
    auto process = [&ps](const Program &p, const int &x) {
        EXPECT_EQ(p.size(), x + 1);
        ps.push_back(p);

        return true;
    };
    enumerate(r, [](const Program &p, const int &x) { return x + 1; }, process, 0);

    EXPECT_EQ(4, ps.size());
}