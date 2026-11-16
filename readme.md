# AllDiff-LS

AllDiff-LS is an early-stage research prototype for solving constraint satisfaction problems with alldifferent constraints by local search. This repository accompanies the paper in [Paper and Citation](#paper-and-citation). It contains C++ solvers for Sudoku, N-Queens, All-Interval, and 2-MOLS, benchmark instances under [benchmarks/](benchmarks/), and scripts under [Scripts/](Scripts/) and [experiment/](experiment/).

The core algorithms are available, and the scripts under [Scripts/](Scripts/) are usable for quick experiments. Interfaces and outputs may change in future updates. The batch path depends on a manual `AI-LS` symlink, on invoking [Scripts/run.sh](Scripts/run.sh) and [Scripts/parallel.sh](Scripts/parallel.sh) through `bash`, and on a display argument that `run.sh` passes only for Sudoku. Those details are recorded below.

## Overview

The four programs in [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/) are separate local-search solvers:

- [Sudoku-LS.cpp](Solvers/AllDiff-LS/Sudoku-LS.cpp) reads a Sudoku grid from a file.
- [NQueen-LS.cpp](Solvers/AllDiff-LS/NQueen-LS.cpp), [AllInterval-LS.cpp](Solvers/AllDiff-LS/AllInterval-LS.cpp), and [MOLS-LS.cpp](Solvers/AllDiff-LS/MOLS-LS.cpp) take the order as a decimal string and parse it with `std::stoi`.

[Scripts/pre.sh](Scripts/pre.sh) compiles those four translation units with `g++ -std=c++11 -O3` and writes the executables beside the sources. Until that script has been run, [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/) contains the `.cpp` files.

[Scripts/run.sh](Scripts/run.sh) dispatches one benchmark folder. [Scripts/parallel.sh](Scripts/parallel.sh) launches a benchmark group in the background and writes logs under `Results/`. The same driver can call baseline solvers elsewhere under [Solvers/](Solvers/). Those baselines sit outside the `pre.sh` build.

The accompanying paper describes AllDiff-LS as a local-search algorithm that represents alldifferent constraints as graphs, simplifies the graphs with two polynomial-time reduction rules, and selects moves in two steps together with tabu and restart strategies.

## Requirements

Building and running the AllDiff-LS solvers uses:

- `g++` with C++11 and the GCC header `<bits/stdc++.h>`. Each solver in [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/) includes that header, and [Scripts/pre.sh](Scripts/pre.sh) calls `g++ -std=c++11 -O3`.
- The POSIX headers included by those sources: `<sys/times.h>`, `<unistd.h>`, and `<libgen.h>`.
- `bash`. The commands in this file invoke the scripts through `bash`.

[Scripts/pre.sh](Scripts/pre.sh) has no shebang. [Scripts/run.sh](Scripts/run.sh) and [Scripts/parallel.sh](Scripts/parallel.sh) begin with the bytes `#！/bin/bash`, where the second character is U+FF01 (a fullwidth exclamation mark). Call them with `bash`.

Python, MiniZinc, OR-Tools, CPLEX, and Kissat enter only when [Scripts/run.sh](Scripts/run.sh) is given a baseline solver name. This repository does not record install steps for those packages. Three baseline programs are already stored as Linux x86-64 executables, and `pre.sh` does not build them:

- [Solvers/Sudoku_acs/sudoku_acs](Solvers/Sudoku_acs/sudoku_acs)
- [Solvers/Sudoku_ils/build/ilsSolver](Solvers/Sudoku_ils/build/ilsSolver)
- [Solvers/Kissat/Kissat/kissat](Solvers/Kissat/Kissat/kissat)

The `sudoku_ils` branch calls `../solvers/Sudoku_ils/build/ilsSolver`. The committed path is [Solvers/Sudoku_ils/build/ilsSolver](Solvers/Sudoku_ils/build/ilsSolver). On a case-sensitive filesystem those two strings differ.

## Building

From the repository root:

```bash
bash Scripts/pre.sh
```

[Scripts/pre.sh](Scripts/pre.sh) changes into [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/), runs the four commands below, and returns to the repository root:

```bash
g++ Sudoku-LS.cpp -std=c++11 -O3 -o Sudoku-LS
g++ NQueen-LS.cpp -std=c++11 -O3 -o NQueen-LS
g++ AllInterval-LS.cpp -std=c++11 -O3 -o AllInterval-LS
g++ MOLS-LS.cpp -std=c++11 -O3 -o MOLS-LS
```

The outputs are `Sudoku-LS`, `NQueen-LS`, `AllInterval-LS`, and `MOLS-LS` in [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/). The script stops there. [Scripts/run.sh](Scripts/run.sh) calls `../Solvers/AllDiff-LS/AI-LS` for the solver name `allinterval_AllDiff-LS`, so create that name as a symlink whose target is resolved in the solver directory:

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

Further direct checks:

```bash
./Solvers/AllDiff-LS/NQueen-LS 1000 1 60 0
./Solvers/AllDiff-LS/AllInterval-LS 20 1 60 0
./Solvers/AllDiff-LS/MOLS-LS 7 1 60 0
```

Each `main` reads four arguments and does not test `argc`:

1. Instance path for Sudoku, or the order string for N-Queens, All-Interval, and 2-MOLS.
2. Seed, passed to `srand` through `atoi`.
3. Time limit in seconds, parsed with `atof`. The solver keeps process CPU time from `times()` (`tms_utime` + `tms_stime`) divided by `sysconf(_SC_CLK_TCK)`, and it continues while that value is below the limit. The value is refreshed after each local-search iteration.
4. Display flag, parsed with `atof` and stored as an `int`. Value `0` prints `<argument> find answer: <cpu seconds>` when the best clash count reaches 0. Value `1` prints the assignment. The commands above pass `0`, which keeps large-run logs to that one line.

The 30-second and 60-second limits are these short checks. The batch example in [Reproducing Experiments](#reproducing-experiments) uses a 1000-second limit.

When Sudoku-LS reaches the end of the search loop it prints `current min clash length`. If initialization already finds a solution, `main` returns before that line. NQueen-LS, AllInterval-LS, and MOLS-LS leave `main` without that print.

## Implementation Guide

### Programs and Arguments

- Sudoku: [Solvers/AllDiff-LS/Sudoku-LS.cpp](Solvers/AllDiff-LS/Sudoku-LS.cpp), binary `Sudoku-LS`. The first argument is a filesystem path. `mRead` opens it and reads the order, a second integer stored in `is_sat`, and the grid. Later search code leaves `is_sat` unread. `compare` in the same file also reads `is_sat`, and its call sites are commented out.
- N-Queens: [Solvers/AllDiff-LS/NQueen-LS.cpp](Solvers/AllDiff-LS/NQueen-LS.cpp), binary `NQueen-LS`. The first argument is the order (`std::stoi`).
- All-Interval: [Solvers/AllDiff-LS/AllInterval-LS.cpp](Solvers/AllDiff-LS/AllInterval-LS.cpp), binary `AllInterval-LS`. The batch name is `AI-LS`. The first argument is the order (`std::stoi`).
- 2-MOLS: [Solvers/AllDiff-LS/MOLS-LS.cpp](Solvers/AllDiff-LS/MOLS-LS.cpp), binary `MOLS-LS`. The first argument is the order (`std::stoi`).

NQueen-LS, AllInterval-LS, and MOLS-LS expect an order number as that first argument. Pass display `0` on direct runs when the log should stay on the `find answer` line.

### Batch Driver

Run [Scripts/parallel.sh](Scripts/parallel.sh) with the working directory set to [Scripts/](Scripts/). It calls `bash run.sh` from the current directory and writes logs to `../Results/`.

Arguments, in order: benchmark directory, time limit, solver name, seed.

For `sudoku_AllDiff-LS`, [Scripts/run.sh](Scripts/run.sh) executes:

```bash
../Solvers/AllDiff-LS/Sudoku-LS $file $seed $time_t 0
```

For `nqueens_AllDiff-LS`, `allinterval_AllDiff-LS`, and `mols_AllDiff-LS`, the script takes digit runs from the instance path with `grep -o -E '[0-9]+'` and passes that string as the order. Each current filename under [benchmarks/N-Queens/](benchmarks/N-Queens/), [benchmarks/All-Interval/](benchmarks/All-Interval/), and [benchmarks/MOLS/](benchmarks/MOLS/) contains one integer. Those three branches pass three arguments:

```bash
../Solvers/AllDiff-LS/NQueen-LS $number $seed $time_t
../Solvers/AllDiff-LS/AI-LS $number $seed $time_t
../Solvers/AllDiff-LS/MOLS-LS $number $seed $time_t
```

The corresponding `main` functions still evaluate `display = atof(argv[4])`. The direct commands in [Quick Start](#quick-start) supply the fourth argument. These batch branches do not.

`parallel.sh` starts one `nohup` background job per subgroup and returns without waiting. [Scripts/run.sh](Scripts/run.sh) prints a separator and then `<path> : <milliseconds> ms.` for each instance. Those milliseconds come from `date` and measure wall-clock time around that process.

[Scripts/gen_cnf_formula.py](Scripts/gen_cnf_formula.py) and [Scripts/gen_dzn_formula.py](Scripts/gen_dzn_formula.py) are separate from `pre.sh` and `parallel.sh`. The CNF generator takes one path and rewrites its first component to `benchmarks_SAT` before writing. In the `__main__` block the Sudoku size used for that encoding is fixed at 9. The DZN generator reads a directory named `proportion`. The root layout has neither `benchmarks_SAT` nor `proportion`.

### Baseline Names

[Scripts/run.sh](Scripts/run.sh) also branches on the names below. They are separate from the AllDiff-LS binaries produced by `pre.sh`:

- `sudoku_acs` calls [Solvers/Sudoku_acs/sudoku_acs](Solvers/Sudoku_acs/sudoku_acs) with `--alg 0 --file <file> --timeout <seconds>`.
- `sudoku_ils` calls `../solvers/Sudoku_ils/build/ilsSolver` with `--instanceFile`, `--timeLimit`, `--mcProbability 100`, `--iterationLimit 1000`, `--tabuListSize 0.05`, `--alpha 0.8`, `--acceptanceProb 15`, `--outfile output.txt`, and `--logfile log.txt`.
- `sudoku_ortools`, `nqueens_ortools`, `allinterval_ortools`, and `mols_ortools` run the Python scripts in [Solvers/OR-Tools/](Solvers/OR-Tools/), which import `ortools.sat.python`.
- `sudoku_cplex`, `nqueens_cplex`, `allinterval_cplex`, and `mols_cplex` run the Python scripts in [Solvers/CPLEX/](Solvers/CPLEX/), which import `docplex.cp.model`.
- `sudoku_choco`, `sudoku_yuck`, `sudoku_gecode`, and the matching `nqueens_`, `allinterval_`, and `mols_` names call `minizinc --solver choco`, `yuck`, or `gecode` on the models in [Solvers/MiniZinc/](Solvers/MiniZinc/). `--time-limit` is the given limit multiplied by 1000.
- `sudoku_kissat` calls [Solvers/Kissat/Kissat/kissat](Solvers/Kissat/Kissat/kissat) with `-n -q`.

`timeout` wraps the Python and Kissat commands. The solver name `test` only prints the instance path.

## Benchmarks

### Sudoku

[benchmarks/Sudoku/](benchmarks/Sudoku/) has six generated groups and one logic-solvable group.

`INST_16x16`, `INST_25x25`, `INST_36x36`, `INST_49x49`, `INST_64x64`, and `INST_81x81` each contain the subdirectories `0`, `10`, `20`, `30`, `40`, `50`, `60`, `70`, `80`, and `90`. Each subdirectory holds 100 files, and each order has 1000 files. Names follow `inst<side>x<side>_<percent>_<index>.txt`. One example is [benchmarks/Sudoku/INST_16x16/0/inst16x16_0_0.txt](benchmarks/Sudoku/INST_16x16/0/inst16x16_0_0.txt).

[benchmarks/Sudoku/logic-solvable/](benchmarks/Sudoku/logic-solvable/) contains 22 named instances, including `platinumblond.txt`.

Sudoku-LS reads every one of these files as follows. The first integer is the order: 4, 5, 6, 7, 8, and 9 for the six `INST_*` groups, so the side length is `order * order`, and 3 for all 22 logic-solvable files. The second integer is `1` in every file and is stored in `is_sat`. The remaining lines are one grid row each, with whitespace-separated integers. A cell equal to `-1` receives every color from 1 through the side length. Any other value is kept as a fixed color.

### N-Queens

[benchmarks/N-Queens/](benchmarks/N-Queens/) contains `inst500.txt`, `inst1000.txt`, `inst1500.txt`, `inst2000.txt`, `inst2500.txt`, `inst3000.txt`, `inst3500.txt`, `inst4000.txt`, `inst4500.txt`, `inst5000.txt`, `inst5500.txt`, and `inst6000.txt`.

NQueen-LS parses the order from its first argument. [Scripts/run.sh](Scripts/run.sh) passes the integer in the filename. The file bodies disagree with several of those names. `inst1000.txt`, `inst2000.txt`, `inst3000.txt`, `inst4000.txt`, and `inst5000.txt` begin with that order. `inst500.txt` begins with 400, `inst1500.txt` with 1400, `inst2500.txt` with 2400, `inst3500.txt` with 3400, `inst4500.txt` with 4400, `inst5500.txt` with 5200, and `inst6000.txt` with 600.

### All-Interval

[benchmarks/All-Interval/](benchmarks/All-Interval/) contains `inst12.txt`, `inst14.txt`, `inst16.txt`, `inst18.txt`, `inst20.txt`, `inst22.txt`, `inst24.txt`, and `inst26.txt`. Each file is the single character `9`. AllInterval-LS parses the order from its first argument. The batch driver passes the integer in the filename. The direct example uses order 20.

### 2-MOLS

[benchmarks/MOLS/](benchmarks/MOLS/) contains `inst3.txt`, `inst4.txt`, `inst5.txt`, `inst7.txt`, `inst8.txt`, and `inst9.txt`. Each file holds a single integer equal to the order in its name. MOLS-LS parses the order from its first argument. The batch driver passes the integer in the filename. The direct example uses order 7.

## Reproducing Experiments

### Root Batch Runs

Example with seed 1 and a time limit of 1000 seconds. Run from [Scripts/](Scripts/) after [Building](#building):

```bash
cd Scripts
bash parallel.sh ../benchmarks/Sudoku 1000 sudoku_AllDiff-LS 1
bash parallel.sh ../benchmarks/N-Queens 1000 nqueens_AllDiff-LS 1
bash parallel.sh ../benchmarks/All-Interval 1000 allinterval_AllDiff-LS 1
bash parallel.sh ../benchmarks/MOLS 1000 mols_AllDiff-LS 1
```

For paper-style statistics, run with seeds 1 to 10. Repeat the four commands with the last argument set to each seed in that range. `parallel.sh` returns after launching background jobs, so those jobs need to finish before the logs are complete.

A `Results/` directory appears after `parallel.sh` runs. The committed tree has no sample logs and no summary counts from the paper.

### Experiment Directory

[experiment/Sudoku/](experiment/Sudoku/) and [experiment/Others/](experiment/Others/) are a separate harness. Each directory has its own `run.sh`, `parallel.sh`, solver tree, and result path. Commented lines at the top of [experiment/Sudoku/parallel.sh](experiment/Sudoku/parallel.sh) and [experiment/Others/parallel.sh](experiment/Others/parallel.sh) show sample invocations meant to be started inside those directories. Those scripts are independent of [Scripts/pre.sh](Scripts/pre.sh).

[experiment/Sudoku/README.md](experiment/Sudoku/README.md) records four compile-and-run lines local to that tree. Its N-Queens line passes a filename such as `inst1000.txt`. [NQueen-LS.cpp](Solvers/AllDiff-LS/NQueen-LS.cpp) in the root solver directory parses the first argument with `std::stoi`, and the root commands in this file are the interface of [Solvers/AllDiff-LS/](Solvers/AllDiff-LS/). [experiment/Others/README.md](experiment/Others/README.md) contains the heading `AllDifferent-LS`.

[experiment/Sudoku/conda.sh](experiment/Sudoku/conda.sh) downloads a Miniconda installer and installs notebook packages. It is separate from the AllDiff-LS build.

## Results

[Scripts/parallel.sh](Scripts/parallel.sh), when started from [Scripts/](Scripts/), creates `Results/{solver}/{benchmark_group}/` from the repository root.

Sudoku groups contain subdirectories, and the driver writes one log per subgroup:

- `Results/sudoku_AllDiff-LS/INST_16x16/result_0.log` through `result_90.log`, with the same pattern for `INST_25x25`, `INST_36x36`, `INST_49x49`, `INST_64x64`, and `INST_81x81`
- `Results/sudoku_AllDiff-LS/logic-solvable/result.log`, because that folder's immediate entries are files and the driver then emits a single `result.log`

N-Queens, All-Interval, and MOLS are flat directories of files, and the driver writes one log for the group:

- `Results/nqueens_AllDiff-LS/N-Queens/result.log`
- `Results/allinterval_AllDiff-LS/All-Interval/result.log`
- `Results/mols_AllDiff-LS/MOLS/result.log`

Each log receives standard output and standard error from [Scripts/run.sh](Scripts/run.sh). For Sudoku with display `0`, a solved instance adds a `find answer` line, and the script appends the instance path with wall-clock milliseconds. The N-Queens, All-Interval, and MOLS batch commands leave `argv[4]` unset, so the display value in those logs follows that call. This repository ships neither sample logs nor aggregate counts.

## Paper and Citation

This repository accompanies:

Rui Han, Minghao Liu, Yuhang Dong, Fuqi Jia, Yiyuan Wang, Feifei Ma, Minghao Yin, and Jian Zhang. AllDiff-LS: solving alldifferent constraints with efficient local search. Frontiers of Computer Science, 20(8):2008406, 2026.

DOI: [https://doi.org/10.1007/s11704-025-41190-x](https://doi.org/10.1007/s11704-025-41190-x)

The title, authors, journal, year 2026, volume 20, issue 8, article number 2008406, and DOI match the Crossref record and the journal page. In the BibTeX below, `pages` holds that article number, which is how this journal's citation line is written (`20(8): 2008406`).

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
