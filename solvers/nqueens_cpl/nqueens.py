from docplex.cp.model import CpoModel
from docplex.cp.config import context
import sys

# 设定问题规模
n = int(sys.argv[1])

# 创建模型
model = CpoModel("nQueens")

# 创建变量矩阵
queens = [model.integer_var(min=0, max=n-1, name="queen_{}".format(i)) for i in range(n)]

# 约束条件
model.add(model.all_diff(queens))  # 不在同一列
model.add(model.all_diff(queens[i] + i for i in range(n)))  # 不在同一对角线（左上-右下）
model.add(model.all_diff(queens[i] - i for i in range(n)))  # 不在同一对角线（左下-右上）

# 求解模型
context.solver.trace_log = False
solution = model.solve()

# 输出解决方案
if solution:
    print("SAT")