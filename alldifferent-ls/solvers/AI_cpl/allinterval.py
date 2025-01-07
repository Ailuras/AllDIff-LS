from docplex.cp.model import CpoModel
from docplex.cp.config import context
import sys

def solve_allinterval(n):
    # 创建模型
    model = CpoModel(name='All-Interval')
    
    # 创建变量
    allinterval = [model.integer_var(min=0, max=n-1, name=f'x_{i+1}') for i in range(n)]
    
    # 添加约束
    model.add(model.all_diff(allinterval))
    model.add(model.all_diff((allinterval[i] - allinterval[i+1])*(allinterval[i] - allinterval[i+1]) for i in range(n-1)))
    # model.add(model.all_diff([abs(allinterval[i] - allinterval[i+1]) for i in range(n-1)]))

    
    # 求解模型
    context.solver.trace_log = False
    solution = model.solve()
    
    # 输出结果
    if solution:
        print("SAT")

# 设置问题规模
n = int(sys.argv[1])

# 求解All-Interval问题
solve_allinterval(n)
