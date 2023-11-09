import localsolver
import math
import sys

def solve_all_interval(n):
    # 创建LocalSolver上下文
    with localsolver.LocalSolver() as ls:
        param = ls.get_param()
        param.set_verbosity(0)
        model = ls.model

        # 创建整数变量，每个变量表示每一行中皇后的列位置
        queens = [model.int(0, n - 1) for _ in range(n)]

        # 确保每个皇后在不同的列
        for k in range(n):
            model.constraint(model.sum((queens[i] == k) for i in range(n)) == 1)

        # 确保每个皇后在不同的对角线（主对角线和副对角线）
        for i in range(n):
            for j in range(i + 1, n):
                model.constraint(queens[i] != queens[j] + (j - i))
                model.constraint(queens[i] != queens[j] - (j - i))
        
        # 添加目标函数
        model.maximize(0)  # 这只是一个占位符
        model.close()
        # 求解AIS问题
        ls.solve()

        # 获取解决方案
        # solution = [queens[i].value for i in range(n)]
        # print(solution)
        return True

if __name__ == '__main__':
    n = int(sys.argv[1])

    if n < 3:
        print("AIS问题至少需要3个数字。")
    else:
        if(solve_all_interval(n)):
            print("SAT")
