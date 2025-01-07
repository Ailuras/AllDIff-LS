from docplex.cp.model import CpoModel
from docplex.cp.config import context
import sys

# 设定问题规模
n = int(sys.argv[1])

# 创建模型
model = CpoModel("MOLS")

# 创建2个拉丁方阵
squares = [[[model.integer_var(min=1, max=n, name="X_{}_{}_{}".format(k, i, j)) for j in range(n)] for i in range(n)] for k in range(2)]

# 拉丁方阵的约束条件
for k in range(2):
    for i in range(n):
        model.add(model.all_diff(squares[k][i]))  # 行不重复
        model.add(model.all_diff([squares[k][j][i] for j in range(n)]))  # 列不重复

# 正交性约束条件
model.add(model.all_diff([squares[0][i][j] * n + squares[1][i][j] for i in range(n) for j in range(n)]))

# 求解模型
context.solver.trace_log = False
solution = model.solve()

# 输出解决方案
if solution:
    print("SAT")
