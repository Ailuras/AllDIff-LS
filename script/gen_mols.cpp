#include <iostream>
#include <fstream>
#include <vector>

// 变量映射函数
int var(int type, int i, int j, int k, int n) {
    // type: 1 表示 x_{i,j} 的变量，2 表示 y_{i,j} 的变量
    return (type - 1) * n * n * n + (i - 1) * n * n + (j - 1) * n + k;
}

// 生成 CNF 文件的函数
void generate_cnf(int n) {
    int num_vars = 2 * n * n * n; // 变量总数
    std::vector<std::string> clauses; // 用于存储CNF子句

    // 每个位置的变量取值唯一
    for (int type = 1; type <= 2; ++type) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                // 至少取一个值
                std::string clause;
                for (int k = 1; k <= n; ++k) {
                    clause += std::to_string(var(type, i, j, k, n)) + " ";
                }
                clause += "0";
                clauses.push_back(clause);

                // 至多取一个值
                for (int k1 = 1; k1 <= n; ++k1) {
                    for (int k2 = k1 + 1; k2 <= n; ++k2) {
                        clause = "-" + std::to_string(var(type, i, j, k1, n)) + " -" + std::to_string(var(type, i, j, k2, n)) + " 0";
                        clauses.push_back(clause);
                    }
                }
            }
        }
    }

    // 行约束：每行元素互不相同
    for (int type = 1; type <= 2; ++type) {
        for (int i = 1; i <= n; ++i) {
            for (int k = 1; k <= n; ++k) {
                for (int j1 = 1; j1 <= n; ++j1) {
                    for (int j2 = j1 + 1; j2 <= n; ++j2) {
                        std::string clause = "-" + std::to_string(var(type, i, j1, k, n)) + " -" + std::to_string(var(type, i, j2, k, n)) + " 0";
                        clauses.push_back(clause);
                    }
                }
            }
        }
    }

    // 列约束：每列元素互不相同
    for (int type = 1; type <= 2; ++type) {
        for (int j = 1; j <= n; ++j) {
            for (int k = 1; k <= n; ++k) {
                for (int i1 = 1; i1 <= n; ++i1) {
                    for (int i2 = i1 + 1; i2 <= n; ++i2) {
                        std::string clause = "-" + std::to_string(var(type, i1, j, k, n)) + " -" + std::to_string(var(type, i2, j, k, n)) + " 0";
                        clauses.push_back(clause);
                    }
                }
            }
        }
    }

    // 组合唯一性约束
    for (int i1 = 1; i1 <= n; ++i1) {
        for (int j1 = 1; j1 <= n; ++j1) {
            for (int i2 = i1; i2 <= n; ++i2) {
                for (int j2 = (i1 == i2 ? j1 + 1 : 1); j2 <= n; ++j2) {
                    for (int k = 1; k <= n; ++k) {
                        for (int l = 1; l <= n; ++l) {
                            std::string clause = "-" + std::to_string(var(1, i1, j1, k, n)) + " -" + std::to_string(var(2, i1, j1, l, n)) +
                                                 " -" + std::to_string(var(1, i2, j2, k, n)) + " -" + std::to_string(var(2, i2, j2, l, n)) + " 0";
                            clauses.push_back(clause);
                        }
                    }
                }
            }
        }
    }

    // 输出到CNF文件
    std::string filename = "../benchmarks_sat/2-MOLS-" + std::to_string(n) + ".cnf";
    std::ofstream outfile(filename);
    outfile << "p cnf " << num_vars << " " << clauses.size() << "\n";
    for (const auto& clause : clauses) {
        outfile << clause << "\n";
    }
    outfile.close();

    std::cout << "已生成CNF文件：" << filename << "\n";
}

int main() {
    for (int n = 3; n <= 10; n++) {
        generate_cnf(n);
    }
    return 0;
}