from ortools.sat.python import cp_model

def main():
    # 创建一个CP模型
    model = cp_model.CpModel()
    
    # 创建变量
    x = model.NewIntVar(0, 9, 'x')
    y = model.NewIntVar(0, 9, 'y')
    z = model.NewIntVar(0, 9, 'z')
    
    # 添加约束
    model.Add(x + y == z)
    
    # 创建一个求解器
    solver = cp_model.CpSolver()
    
    # 在不进行搜索的情况下应用弧一致性算法
    model.Add(x.NotMember([1, 2, 3]))  # 这会应用弧一致性来排除不满足约束的值
    
    # 求解模型
    status = solver.Solve(model)
    
    if status == cp_model.OPTIMAL:
        print(f'Solution found: x={solver.Value(x)}, y={solver.Value(y)}, z={solver.Value(z)}')
    else:
        print('No solution found')

if __name__ == '__main__':
    main()
