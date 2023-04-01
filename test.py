# The quasigroup existence problem is to determine whether there exists an $n \times n$ array where each cell can be filled with an integer from 1 to $n$, and the elements in each row and each column are all different from each other. Model the quasigroup existence problem using the Python API of OR-Tools.
from ortools.sat.python import cp_model

def main():
    # Create the model.
    model = cp_model.CpModel()

    # Create the variables.
    n = 4
    x = {}
    for i in range(n):
        for j in range(n):
            x[i, j] = model.NewIntVar(1, n, 'x[%i,%i]' % (i, j))

    # Create the constraints.
    # Each row and each column must contain all the numbers from 1 to n.
    for i in range(n):
        model.AddAllDifferent([x[i, j] for j in range(n)])
        model.AddAllDifferent([x[j, i] for j in range(n)])

    # Create a solver and solve.
    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    if status == cp_model.OPTIMAL:
        for i in range(n):
            for j in range(n):
                print(solver.Value(x[i, j]), end=' ')
            print()
            
if __name__ == '__main__':
    main()