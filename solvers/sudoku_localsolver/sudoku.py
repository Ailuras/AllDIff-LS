import localsolver
import math
import sys

def solve_sudoku(n, initial_grid):
    # 创建LocalSolver上下文
    with localsolver.LocalSolver() as ls:
        param = ls.get_param()
        param.set_verbosity(0)
        model = ls.model

        # 创建决策变量，每个格子代表一个数字
        grid = [[model.int(1, n) for _ in range(n)] for _ in range(n)]
        
        # 添加约束：每行的数字互不相同
        for i in range(n):
            for k in range(1, n+1):
                model.constraint(model.sum((grid[i][j] == k) for j in range(n)) == 1)


        # 添加约束：每列的数字互不相同
        for j in range(n):
            for k in range(1, n+1):
                model.constraint(model.sum((grid[i][j] == k) for i in range(n)) == 1)

        # 定义子宫格的大小
        subgrid_size = int(math.sqrt(n))
        for row_start in range(0, n, subgrid_size):
            for col_start in range(0, n, subgrid_size):
                # 对于每个子宫格
                for k in range(1, n+1):
                    model.constraint(model.sum((grid[i][j] == k) for i in range(row_start, row_start + subgrid_size) for j in range(col_start, col_start + subgrid_size)) == 1)


        # 添加初始值约束
        for i in range(n):
            for j in range(n):
                if initial_grid[i][j] > 0:
                    model.constraint(grid[i][j] == initial_grid[i][j])

        # 最小化目标函数，这只是一个占位符
        model.minimize(0)
        model.close()
        # 求解数独
        ls.solve()

        # 获取解决方案
        return True

if __name__ == '__main__':
    # n = int(input("请输入数独的阶数n："))
    file = sys.argv[1]
    info = file.split('/')
    line_size = int(info[-1].split('_')[0].split('x')[-1])
    cell_size = int(math.sqrt(line_size))
    fixed_prop = int(info[-1].split('_')[1])
    
    initial_grid = []
    with open(file) as f:
        f.readline()
        f.readline()
        for i in range(line_size):
            line = f.readline().strip('\t\n').split('\t')
            initial_grid.append(line)
    initial_grid = [[int(x) for x in line] for line in initial_grid]
    # print("请输入数独初始状态，使用0表示空格，按行输入：")
    # for i in range(n):
    #     row = list(map(int, input().split()))
    #     initial_grid[i] = row

    if(solve_sudoku(line_size, initial_grid)):
        print("SAT")
