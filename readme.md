# AllDiff-LS

AllDiff-LS is a research prototype for solving CSPs with alldifferent constraints using local search.

## What is in this repository?

- C++ solvers for Sudoku, N-Queens, All-Interval, and 2-MOLS
- benchmark instances under benchmarks/
- experiment scripts under Scripts/ and experiment/

## Current Status

This project is still early-stage.

- core algorithms are available
- scripts are usable for quick experiments
- interfaces and outputs may change in future updates

## Implementation Path (Where to Start)

If you want to trace from source code to runs:

- Core solvers: Solvers/AllDiff-LS/Sudoku-LS.cpp, Solvers/AllDiff-LS/NQueen-LS.cpp, Solvers/AllDiff-LS/AllInterval-LS.cpp, Solvers/AllDiff-LS/MOLS-LS.cpp
- Batch orchestration: Scripts/run.sh, Scripts/parallel.sh
- Benchmarks: benchmarks/

## Quick Start

Run from repository root:

```bash
bash Scripts/pre.sh
ln -sf AllInterval-LS Solvers/AllDiff-LS/AI-LS
./Solvers/AllDiff-LS/Sudoku-LS benchmarks/Sudoku/INST_16x16/0/inst16x16_0_0.txt 1 30 0
```

Optional quick checks:

```bash
./Solvers/AllDiff-LS/NQueen-LS 1000 1 60 0
./Solvers/AllDiff-LS/AllInterval-LS 20 1 60 0
./Solvers/AllDiff-LS/MOLS-LS 7 1 60 0
```

## Reproduce Basic Batch Runs

Example (seed 1, time limit 1000s):

```bash
cd Scripts
bash parallel.sh ../benchmarks/Sudoku 1000 sudoku_AllDiff-LS 1
bash parallel.sh ../benchmarks/N-Queens 1000 nqueens_AllDiff-LS 1
bash parallel.sh ../benchmarks/All-Interval 1000 allinterval_AllDiff-LS 1
bash parallel.sh ../benchmarks/MOLS 1000 mols_AllDiff-LS 1
```

For paper-style statistics, run with seeds 1 to 10.

## Results

Batch outputs are stored under:

- Results/{solver}/{benchmark_group}/

Examples:

- Results/sudoku_AllDiff-LS/INST_16x16/
- Results/nqueens_AllDiff-LS/N-Queens/

## Notes

- Scripts/run.sh expects an All-Interval binary named AI-LS.
- NQueen-LS, AllInterval-LS, and MOLS-LS expect an order number as input.
- Use display flag 0 for cleaner logs on large runs.

## Paper and Citation

This repository accompanies the paper:

AllDiff-LS: solving alldifferent constraints with efficient local search

DOI: [https://doi.org/10.1007/s11704-025-41190-x](https://doi.org/10.1007/s11704-025-41190-x)

Cite:

```text
Rui Han, Minghao Liu, Yuhang Dong, Fuqi Jia, Yiyuan Wang, Feifei Ma, Minghao Yin, Jian Zhang.
AllDiff-LS: solving alldifferent constraints with efficient local search.
Frontiers of Computer Science, 2026.
```
