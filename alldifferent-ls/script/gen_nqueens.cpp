#include <iostream>
#include <fstream>
#include <string>

// N-Queens problem encoding
void n_queens(int n, std::ofstream& f, int& clause_count) {
    int num_vars = n * n;  // Each cell is a variable

    auto var = [n](int i, int k) {
        return i * n + k + 1;  // 0-based indexing
    };

    // Each row must have exactly one queen
    for (int i = 0; i < n; ++i) {
        // At least one queen in each row
        for (int k = 0; k < n; ++k)
            f << var(i, k) << " ";
        f << "0\n";
        clause_count++;

        // At most one queen in each row
        for (int k1 = 0; k1 < n; ++k1)
            for (int k2 = k1 + 1; k2 < n; ++k2) {
                f << -var(i, k1) << " " << -var(i, k2) << " 0\n";
                clause_count++;
            }
    }

    // Each column must have at most one queen
    for (int k = 0; k < n; ++k)
        for (int i1 = 0; i1 < n; ++i1)
            for (int i2 = i1 + 1; i2 < n; ++i2) {
                f << -var(i1, k) << " " << -var(i2, k) << " 0\n";
                clause_count++;
            }

    // Diagonal constraints
    for (int i1 = 0; i1 < n; ++i1)
        for (int k1 = 0; k1 < n; ++k1)
            for (int i2 = i1 + 1; i2 < n; ++i2) {
                int k2_main = k1 + (i2 - i1);
                if (k2_main < n) {
                    f << -var(i1, k1) << " " << -var(i2, k2_main) << " 0\n";
                    clause_count++;
                }
                int k2_anti = k1 - (i2 - i1);
                if (k2_anti >= 0) {
                    f << -var(i1, k1) << " " << -var(i2, k2_anti) << " 0\n";
                    clause_count++;
                }
            }
}

// Function to generate and write the CNF based on user input
void generate_cnf(const std::string& problem_type, int n) {
    std::string filename;
    if (problem_type == "n_queens") {
        filename = "../benchmarks_sat/n_queens_" + std::to_string(n) + ".cnf";
        int num_vars = n * n;
        std::ofstream f(filename);
        if (!f) {
            std::cerr << "Error opening file " << filename << std::endl;
            return;
        }
        // Write placeholder for header
        f << "p cnf " << num_vars << " 0\n";
        int clause_count = 0;
        // Generate and write clauses
        n_queens(n, f, clause_count);
        // Seek back to the beginning and write the correct clause count
        f.seekp(0, std::ios::beg);
        f << "p cnf " << num_vars << " " << clause_count << "\n";
        std::cout << "CNF written to " << filename << std::endl;
    } else {
        std::cout << "Unknown problem type. Use 'n_queens'." << std::endl;
    }
}

int main() {
    std::string problem_type = "n_queens";  // or 'all_interval', 'mols'
    for (int n = 500; n <= 2001; n += 500) {
        generate_cnf(problem_type, n);
    }
    return 0;
}