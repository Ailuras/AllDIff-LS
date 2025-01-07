from docplex.cp.model import CpoModel
from docplex.cp.config import context
import numpy as np
import sys
import math

file = sys.argv[1]
info = file.split('/')
SIZE = int(info[-1].split('_')[0].split('x')[-1])
SUBGRID_SIZE = int(math.sqrt(SIZE))
fixed_prop = int(info[-1].split('_')[1])
# 输入数独问题
sudoku_problem = []

with open(file) as f:
    f.readline()
    f.readline()
    for i in range(SIZE):
        line = f.readline().strip('\t\n').split('\t')
        sudoku_problem.append(line)
        
sudoku_problem = [[int(x) for x in line] for line in sudoku_problem]

# 创建CP模型
model = CpoModel()

# 定义决策变量
grid = [[model.integer_var(1, SIZE, f'grid_{row}_{col}') for col in range(SIZE)] for row in range(SIZE)]

# 添加约束条件
# 行和列约束
for i in range(SIZE):
    model.add(model.all_diff([grid[i][j] for j in range(SIZE)]))  # 行约束
    model.add(model.all_diff([grid[j][i] for j in range(SIZE)]))  # 列约束

# 子网格约束
for r in range(0, SIZE, SUBGRID_SIZE):
    for c in range(0, SIZE, SUBGRID_SIZE):
        model.add(model.all_diff([grid[r + i][c + j] for i in range(SUBGRID_SIZE) for j in range(SUBGRID_SIZE)]))

# 给定值约束
for i in range(SIZE):
    for j in range(SIZE):
        if sudoku_problem[i][j] != -1:
            model.add(grid[i][j] == sudoku_problem[i][j])

# 求解模型
context.solver.trace_log = False
solution = model.solve()

# 打印解决方案
if solution:
    print("SAT")
