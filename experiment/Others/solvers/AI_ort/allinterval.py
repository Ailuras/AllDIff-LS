import sys
from ortools.sat.python import cp_model

def solve_ais(n):
    model = cp_model.CpModel()
    variables = [model.NewIntVar(1, n, f'x{i}') for i in range(n)]
    differences = [model.NewIntVar(1, n*n, f'd{i}') for i in range(n - 1)]

    for i in range(n - 1):
        model.AddAbsEquality(differences[i], variables[i + 1] - variables[i])
    # 每个数都不同
    model.AddAllDifferent(variables)
    model.AddAllDifferent(differences)

    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
        print("SAT")
        # for var in variables:
        #     print(f'{var.Name()}: {solver.Value(var)}')
        # for var in differences:
        #     print(f'{var.Name()}: {solver.Value(var)}')

if __name__ == '__main__':
    order = int(sys.argv[1])
    solve_ais(order)
