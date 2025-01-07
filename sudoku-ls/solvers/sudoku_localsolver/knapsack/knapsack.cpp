#include "localsolver.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace localsolver;
using namespace std;

class Knapsack {
public:
    // Number of items
    int nbItems;

    // Items properties
    vector<int> weights;
    vector<int> values;

    // Knapsack bound
    int knapsackBound;

    // LocalSolver
    LocalSolver localsolver;

    // Decision variables
    vector<LSExpression> x;

    // Objective
    LSExpression knapsackValue;

    /* Read instance data */
    void readInstance(const string& fileName) {
        ifstream infile;
        infile.exceptions(ifstream::failbit | ifstream::badbit);
        infile.open(fileName.c_str());

        infile >> nbItems;

        weights.resize(nbItems);
        for (int i = 0; i < nbItems; ++i)
            infile >> weights[i];

        values.resize(nbItems);
        for (int i = 0; i < nbItems; ++i)
            infile >> values[i];

        infile >> knapsackBound;
    }

    void solve(int limit) {
        // Declare the optimization model
        LSModel model = localsolver.getModel();

        // Decision variables x[i]
        x.resize(nbItems);
        for (int i = 0; i < nbItems; ++i) {
            x[i] = model.boolVar();
        }

        // Weight constraint
        LSExpression knapsackWeight = model.sum();
        for (int i = 0; i < nbItems; ++i) {
            LSExpression itemWeight = x[i] * weights[i];
            knapsackWeight.addOperand(itemWeight);
        }
        model.constraint(knapsackWeight <= knapsackBound);

        // Maximize value
        knapsackValue = model.sum();
        for (int i = 0; i < nbItems; ++i) {
            LSExpression itemValue = x[i] * values[i];
            knapsackValue.addOperand(itemValue);
        }
        model.maximize(knapsackValue);
        model.close();

        // Parametrize the solver
        localsolver.getParam().setTimeLimit(limit);

        localsolver.solve();
    }

    /* Write the solution in a file */
    void writeSolution(const string& fileName) {
        ofstream outfile;
        outfile.exceptions(ofstream::failbit | ofstream::badbit);
        outfile.open(fileName.c_str());

        outfile << knapsackValue.getValue() << endl;
        for (int i = 0; i < nbItems; ++i) {
            if (x[i].getValue() == 1)
                outfile << i << " ";
        }
        outfile << endl;
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: knapsack inputFile [outputFile] [timeLimit]" << endl;
        return 1;
    }

    const char* instanceFile = argv[1];
    const char* solFile = argc > 2 ? argv[2] : NULL;
    const char* strTimeLimit = argc > 3 ? argv[3] : "20";

    try {
        Knapsack model;
        model.readInstance(instanceFile);
        model.solve(atoi(strTimeLimit));
        if (solFile != NULL)
            model.writeSolution(solFile);
        return 0;
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
        return 1;
    }
}
