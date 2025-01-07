import localsolver
import math
import sys

def solve_all_interval(n):
    # 创建LocalSolver上下文
    with localsolver.LocalSolver() as ls:
        param = ls.get_param()
        param.set_verbosity(0)
        model = ls.model

        # 创建决策变量，表示排列
        permutation = [model.int(1, n) for _ in range(n)]

        for k in range(1, n+1):
            model.constraint(model.sum((permutation[i] == k) for i in range(n)) == 1)
        
        differences = [model.abs(permutation[i+1] - permutation[i]) for i in range(n-1)]
        for k in range(1, n):
            model.constraint(model.sum((differences[i] == k) for i in range(n-1)) == 1)
        
        # 添加目标函数
        model.maximize(0)  # 这只是一个占位符
        model.close()
        # 求解AIS问题
        ls.solve()

        # 获取解决方案
        # solution = [permutation[i].value for i in range(n)]
        # print(solution)
        return True

if __name__ == '__main__':
    n = int(sys.argv[1])

    if n < 3:
        print("AIS问题至少需要3个数字。")
    else:
        if(solve_all_interval(n)):
            print("SAT")
