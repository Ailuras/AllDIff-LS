from ortools.sat.python import cp_model

def print_solution(solver, A, B, n):
    print("Solution for A:")
    for i in range(n):
        for j in range(n):
            print(f"{solver.Value(A[i][j])} ", end="")
        print()
    print()

    print("Solution for B:")
    for i in range(n):
        for j in range(n):
            print(f"{solver.Value(B[i][j])} ", end="")
        print()
    print()

def solve_orthogonal_latin_squares(n):
    model = cp_model.CpModel()

    A = [[model.NewIntVar(1, n, f'A_{i}_{j}') for j in range(n)] for i in range(n)]
    B = [[model.NewIntVar(1, n, f'B_{i}_{j}') for j in range(n)] for i in range(n)]

    for i in range(n):
        model.AddAllDifferent([A[i][j] for j in range(n)])
        model.AddAllDifferent([A[j][i] for j in range(n)])
        model.AddAllDifferent([B[i][j] for j in range(n)])
        model.AddAllDifferent([B[j][i] for j in range(n)])

    for i in range(n):
        for j in range(n):
            for x in range(n):
                for y in range(n):
                    if i == x and j == y:
                        continue
                    model.Add(A[i][j] * n + B[i][j] != A[x][y] * n + B[x][y])

    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
        print_solution(solver, A, B, n)
    else:
        print("No solution found.")

if __name__ == "__main__":
    n = 9
    solve_orthogonal_latin_squares(n)