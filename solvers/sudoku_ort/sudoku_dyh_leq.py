#!/usr/bin/env python3
# Copyright 2010-2022 Google LLC
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""This model implements a sudoku solver."""

from ortools.sat.python import cp_model
import math
import sys

def solve_sudoku(file):

    info = file.split('/')
    line_size = int(info[-1].split('_')[0].split('x')[-1])
    cell_size = int(math.sqrt(line_size))
    fixed_prop = int(info[-1].split('_')[1])
    
    """Solves the sudoku problem with the CP-SAT solver."""
    # Create the model.
    model = cp_model.CpModel()

    initial_grid = []

    # dyh: number of eqs and eqs
    eq_num = None
    eqs = []

    with open(file) as f:
        f.readline()
        f.readline()
        for i in range(line_size):
            line = f.readline().strip('\t\n').split('\t')
            initial_grid.append(line)
        
        # dyh: read number of eqs, then store eqs
        eq_num = int(f.readline().strip())
        # print(eq_num)
        for i in range(eq_num):
            line = f.readline().strip().split(' ')
            eqs.append(line)
        # print(eqs)
        
    initial_grid = [[int(x) for x in line] for line in initial_grid]
    # print(initial_grid)
    
    line = list(range(0, line_size))
    cell = list(range(0, cell_size))

    grid = {}
    for i in line:
        for j in line:
            grid[(i, j)] = model.NewIntVar(1, line_size, 'grid %i %i' % (i, j))

    # AllDifferent on rows.
    for i in line:
        model.AddAllDifferent(grid[(i, j)] for j in line)

    # AllDifferent on columns.
    for j in line:
        model.AddAllDifferent(grid[(i, j)] for i in line)

    # AllDifferent on cells.
    for i in cell:
        for j in cell:
            one_cell = []
            for di in cell:
                for dj in cell:
                    one_cell.append(grid[(i * cell_size + di,
                                          j * cell_size + dj)])

            model.AddAllDifferent(one_cell)

    # Initial values.
    for i in line:
        for j in line:
            if initial_grid[i][j] > 0:
                model.Add(grid[(i, j)] == initial_grid[i][j])

    # dyh: inequality constraints
    for eq in eqs:
        x1, y1, x2, y2 = int(eq[0]) - 1, int(eq[1]) - 1, int(eq[2]) - 1, int(eq[3]) - 1
        # print(x1, y1, x2, y2)
        model.Add(grid[(x1, y1)] <= grid[(x2, y2)])

    # Solve and print out the solution.
    solver = cp_model.CpSolver()
    status = solver.Solve(model)
    if status == cp_model.OPTIMAL:
        print("SAT")
        # for i in line:
        #     print([int(solver.Value(grid[(i, j)])) for j in line])

file = sys.argv[1]
solve_sudoku(file)