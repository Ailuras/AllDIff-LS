# AllDiff-LS

AllDiff-LS is a research prototype for solving CSPs with alldifferent constraints using local search. The project is still early-stage. The core algorithms are available, the scripts are usable for quick experiments, and interfaces and outputs may change.

## Overview

- C++ solvers for Sudoku, N-Queens, All-Interval, and 2-MOLS in [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/)
- Benchmark instances under [benchmarks/](benchmarks/)
- Experiment scripts under [Scripts/](Scripts/) and [experiment/](experiment/)

Sudoku-LS reads an instance file. NQueen-LS, AllInterval-LS, and MOLS-LS take an order number. [Scripts/pre.sh](Scripts/pre.sh) compiles the four programs. The executables appear in that directory after the script runs.

## Requirements

- GCC `g++` with C++11, on a POSIX system. [Scripts/pre.sh](Scripts/pre.sh) uses `g++ -std=c++11 -O3`.
- `bash`. Run the scripts with `bash`, as in the commands below.

## Building

From the repository root:

```bash
bash Scripts/pre.sh
```

[Scripts/pre.sh](Scripts/pre.sh) compiles the sources in [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/) with:

```bash
g++ Sudoku-LS.cpp -std=c++11 -O3 -o Sudoku-LS
g++ NQueen-LS.cpp -std=c++11 -O3 -o NQueen-LS
g++ AllInterval-LS.cpp -std=c++11 -O3 -o AllInterval-LS
g++ MOLS-LS.cpp -std=c++11 -O3 -o MOLS-LS
```

[Scripts/run.sh](Scripts/run.sh) expects an All-Interval binary named `AI-LS`. From the repository root:

```bash
ln -sf AllInterval-LS Solvers/AllDiff-LS/AI-LS
```

## Quick Start

Run from the repository root:

```bash
bash Scripts/pre.sh
ln -sf AllInterval-LS Solvers/AllDiff-LS/AI-LS
./Solvers/AllDiff-LS/Sudoku-LS benchmarks/Sudoku/INST_16x16/0/inst16x16_0_0.txt 1 30 0
```

Optional checks:

```bash
./Solvers/AllDiff-LS/NQueen-LS 1000 1 60 0
./Solvers/AllDiff-LS/AllInterval-LS 20 1 60 0
./Solvers/AllDiff-LS/MOLS-LS 7 1 60 0
```

Each program takes four arguments:

1. Sudoku instance path, or the order for N-Queens, All-Interval, and 2-MOLS.
2. Seed.
3. Time limit in seconds.
4. Display flag. Use `0` for a short log on large runs, or `1` to print the assignment.

## Implementation Guide

- Solvers: [Sudoku-LS.cpp](Solvers/AllDiff-LS/Sudoku-LS.cpp), [NQueen-LS.cpp](Solvers/AllDiff-LS/NQueen-LS.cpp), [AllInterval-LS.cpp](Solvers/AllDiff-LS/AllInterval-LS.cpp), [MOLS-LS.cpp](Solvers/AllDiff-LS/MOLS-LS.cpp)
- One folder: [Scripts/run.sh](Scripts/run.sh)
- One benchmark group: [Scripts/parallel.sh](Scripts/parallel.sh), which writes under `Results/`
- Instances: [benchmarks/](benchmarks/)

Run both scripts with `bash` from [Scripts/](Scripts/). `parallel.sh` takes the benchmark directory, the time limit, the solver name, and the seed. The All-Interval name calls `AI-LS`.

## Benchmarks

- [benchmarks/Sudoku/](benchmarks/Sudoku/): `INST_16x16`, `INST_25x25`, `INST_36x36`, `INST_49x49`, `INST_64x64`, `INST_81x81`, and [logic-solvable/](benchmarks/Sudoku/logic-solvable/). Pass the instance path to Sudoku-LS.
- [benchmarks/N-Queens/](benchmarks/N-Queens/), [benchmarks/All-Interval/](benchmarks/All-Interval/), and [benchmarks/MOLS/](benchmarks/MOLS/): pass the order. [Scripts/run.sh](Scripts/run.sh) reads that integer from the filename.

## Reproducing Experiments

Sudoku batch, seed 1, time limit 1000 seconds. From [Scripts/](Scripts/) after [Building](#building):

```bash
cd Scripts
bash parallel.sh ../benchmarks/Sudoku 1000 sudoku_AllDiff-LS 1
```

For paper-style statistics, repeat with seeds 1 to 10.

`Scripts/run.sh` passes the display argument only for `sudoku_AllDiff-LS`. The batch names `nqueens_AllDiff-LS`, `allinterval_AllDiff-LS`, and `mols_AllDiff-LS` omit it. Run those three programs directly and pass display `0` or `1`. With a 1000-second limit:

```bash
./Solvers/AllDiff-LS/NQueen-LS 1000 1 1000 0
./Solvers/AllDiff-LS/AllInterval-LS 20 1 1000 0
./Solvers/AllDiff-LS/MOLS-LS 7 1 1000 0
```

Use the integer in the benchmark filename as the order, and use each seed from 1 to 10.

## Results

[Scripts/parallel.sh](Scripts/parallel.sh) stores batch logs under `Results/{solver}/{benchmark_group}/`.

Examples:

- `Results/sudoku_AllDiff-LS/INST_16x16/`
- `Results/nqueens_AllDiff-LS/N-Queens/`

## Paper and Citation

This repository accompanies:

Rui Han, Minghao Liu, Yuhang Dong, Fuqi Jia, Yiyuan Wang, Feifei Ma, Minghao Yin, and Jian Zhang. AllDiff-LS: solving alldifferent constraints with efficient local search. Frontiers of Computer Science, 20(8):2008406, 2026.

DOI: [https://doi.org/10.1007/s11704-025-41190-x](https://doi.org/10.1007/s11704-025-41190-x)

```bibtex
@article{Han2026AllDiffLS,
  author = {Han, Rui and Liu, Minghao and Dong, Yuhang and Jia, Fuqi and Wang, Yiyuan and Ma, Feifei and Yin, Minghao and Zhang, Jian},
  title = {AllDiff-LS: solving alldifferent constraints with efficient local search},
  journal = {Frontiers of Computer Science},
  year = {2026},
  volume = {20},
  number = {8},
  pages = {2008406},
  doi = {10.1007/s11704-025-41190-x}
}
```
