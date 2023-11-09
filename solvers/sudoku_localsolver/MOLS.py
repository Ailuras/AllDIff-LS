import localsolver
import math
import sys

def solve_mols(n):
    # 创建LocalSolver上下文
    with localsolver.LocalSolver() as ls:
        param = ls.get_param()
        param.set_verbosity(0)
        model = ls.model

        # 创建决策变量，每个格子代表一个数字
        grid1 = [[model.int(1, n) for _ in range(n)] for _ in range(n)]
        
        # 添加约束：每行的数字互不相同
        for i in range(n):
            for k in range(1, n+1):
                model.constraint(model.sum((grid1[i][j] == k) for j in range(n)) == 1)


        # 添加约束：每列的数字互不相同
        for j in range(n):
            for k in range(1, n+1):
                model.constraint(model.sum((grid1[i][j] == k) for i in range(n)) == 1)

        # 创建决策变量，每个格子代表一个数字
        grid2 = [[model.int(1, n) for _ in range(n)] for _ in range(n)]
        
        # 添加约束：每行的数字互不相同
        for i in range(n):
            for k in range(1, n+1):
                model.constraint(model.sum((grid2[i][j] == k) for j in range(n)) == 1)


        # 添加约束：每列的数字互不相同
        for j in range(n):
            for k in range(1, n+1):
                model.constraint(model.sum((grid2[i][j] == k) for i in range(n)) == 1)

        # grid3 = [[model.int(1+n, n*n+n) for _ in range(n)] for _ in range(n)]
        
        # for i in range(n):
        #     for j in range(n):
        #         model.constraint(grid3[i][j] == grid1[i][j] + grid2[i][j])

        grid3 = [[grid1[i][j]*n + grid2[i][j] for j in range(n)] for i in range(n)]
        
        for x in range(1, n+1):
            for y in range(1, n+1):
                model.constraint(model.sum((grid3[i][j] == x*n+y) for i in range(n) for j in range(n)) == 1)

        # 最小化目标函数，这只是一个占位符
        model.minimize(0)
        model.close()
        # 求解数独
        ls.solve()
        # solution = [grid1[i][j].value for i in range(n) for j in range(n)]
        # print(solution)
        # solution = [grid2[i][j].value for i in range(n) for j in range(n)]
        # print(solution)
        # solution = [grid3[i][j].value for i in range(n) for j in range(n)]
        # print(solution)
        # 获取解决方案
        return True

if __name__ == '__main__':
    # n = int(input("请输入数独的阶数n："))
    n = int(sys.argv[1])

    if(solve_mols(n)):
        print("SAT")
