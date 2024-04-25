from z3 import *
import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument('--file')

file_path = parser.parse_args().file
path = file_path.split('/')[:-1]
file = file_path.split('/')[-1]
lines = []
with open(file_path) as f:
    lines = f.readlines()

A = []
n = int(lines[0].strip('\t\n'))**2
for i in range(n):
    A.append(lines[i+2].strip('\t\n').split('\t'))

path[0] = 'benchmarks_SAT'

file = file.split('_')
file[-1] = '0_use.smt2'
use_file = '_'.join(file)
use_path = '/'.join(path) + '/' + use_file
file[-1] = '0.smt2'
tar_file = '_'.join(file)
smt_path = '/'.join(path) + '/' + tar_file

with open(smt_path) as f:
    smt = f.readlines()

for i in range(n):
    for j in range(n):
        if A[i][j] != '-1':
            begin = 2 + i*n*n + j*n
            for v in range(n):
                smt[begin+v] = f'(define-fun x_{i+1}_{j+1}_{v+1} () Bool false)\n'
            smt[begin+int(A[i][j])-1] = f'(define-fun x_{i+1}_{j+1}_{int(A[i][j])} () Bool true)\n'


with open(use_path, 'w') as f:
    f.writelines(smt)

# if s.check() == sat:
#     print ("SAT")
    # m = s.model()
# else:
    # print ("failed to solve")