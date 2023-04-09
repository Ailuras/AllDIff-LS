"""OR-Tools solution to the N-queens problem."""
import sys
import time
from ortools.sat.python import cp_model


class NQueenSolutionPrinter(cp_model.CpSolverSolutionCallback):
    """Print intermediate solutions."""

    def __init__(self, queens):
        cp_model.CpSolverSolutionCallback.__init__(self)
        self.__queens = queens
        self.__solution_count = 0
        self.__start_time = time.time()

    def solution_count(self):
        return self.__solution_count

    def on_solution_callback(self):
        current_time = time.time()
        print('Solution %i, time = %f s' %
              (self.__solution_count, current_time - self.__start_time))
        self.__solution_count += 1

        all_queens = range(len(self.__queens))
        for i in all_queens:
            for j in all_queens:
                if self.Value(self.__queens[j]) == i:
                    # There is a queen in column j, row i.
                    print('Q', end=' ')
                else:
                    print('_', end=' ')
            print()
        print()



def main(filename):
    
    info = filename.split('/')
    order = int(info[-1].split('_')[0].split('t')[-1])
    
    lines = []

    with open(filename) as f:
        f.readline()
        f.readline()
        for i in range(order):
            lines.append(int(f.readline().strip()))

    # Creates the solver.
    model = cp_model.CpModel()

    # Creates the variables.
    # The array index is the column, and the value is the row.
    queens = [
        model.NewIntVar(1, order, 'x%i' % i) for i in range(order)
    ]

    # Creates the constraints.
    # All rows must be different.
    model.AddAllDifferent(queens)

    # All columns must be different because the indices of queens are all
    # different.

    # No two queens can be on the same diagonal.
    model.AddAllDifferent(queens[i] + i for i in range(order))
    model.AddAllDifferent(queens[i] - i for i in range(order))

    # Initial values.
    for i in range(order):
            if lines[i] > 0:
                model.Add(queens[i] == lines[i])

    # Solve the model.
    solver = cp_model.CpSolver()
    # solution_printer = NQueenSolutionPrinter(queens)
    # solver.parameters.enumerate_all_solutions = False
    # solver.Solve(model, solution_printer)
    status = solver.Solve(model)
    if status == cp_model.OPTIMAL or status == cp_model.FEASIBLE:
        print("SAT")

    # Statistics.
    # print('\nStatistics')
    # print(f'  conflicts      : {solver.NumConflicts()}')
    # print(f'  branches       : {solver.NumBranches()}')
    # print(f'  wall time      : {solver.WallTime()} s')
    # print(f'  solutions found: {solution_printer.solution_count()}')


if __name__ == '__main__':
    # By default, solve the 8x8 problem.
    file = sys.argv[1]
    if len(sys.argv) > 1:
        file = sys.argv[1]
    main(file)
    
        # model.AddAllDifferent([A[i][j]*n + B[i][j] for i in range(n) for j in range(n)])