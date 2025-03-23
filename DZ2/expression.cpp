#include <iostream>
#include <map>
#include <memory>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "expression.h"
#include "parser.h"
#include "token.h"
#include "complex_f.h"
using namespace std;

void parseArguments(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Use: differentiator --eval <expression> [variables] or differentiator --diff <expression> --by <variable>" << endl;
        exit(1);
    }

    string mode = argv[1];

    if (mode == "--eval") {
        if (argc < 3) {
            cerr << "Error: No expression provided for evaluation." << endl;
            exit(1);
        }

        string expression = argv[2];
        map<string, complex<double>> variables;
        for (int i = 3; i < argc; i++) {
            string arg = argv[i];
            size_t eqPos = arg.find('=');
            if (eqPos == string::npos) {
                cerr << "Error: Invalid variable format: " << arg << endl;
                exit(1);
            }
            string var = arg.substr(0, eqPos);
            double value = stod(arg.substr(eqPos + 1));
            variables[var] = value;
        }
        try {
            auto tokens = tokenize(expression);
            vector<string> varNames;
            for (const auto& pair : variables) {
                varNames.push_back(pair.first);
            }
            auto expr = parseExpression<complex<double>>(tokens, varNames);
            map<string, complex<double>> params;
            for (const auto& pair : variables) {
                params[pair.first] = pair.second;
            }
            complex<double> result = expr->eval(params);
            cout << formatComplex(result) << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            exit(1);
        }
    } else if (mode == "--diff") {
        if (argc < 5 || string(argv[3]) != "--by") {
            cerr << "Error: Invalid command format for derivative calculation." << endl;
            exit(1);
        }

        string expression = argv[2];
        string var = argv[4];
        try {
            auto tokens = tokenize(expression);
            vector<string> varNames = {var};
            auto expr = parseExpression<complex<double>>(tokens, varNames);
            auto derivative = expr->dif(var);
            cout << derivative->ToString() << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            exit(1);
        }
    } else {
        cerr << "Error: Unknown mode: " << mode << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    parseArguments(argc, argv);
    return 0;
}
