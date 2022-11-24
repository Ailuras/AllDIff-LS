from z3 import *
# import argparse
# import os

# test n = 6 firstly
# parser = argparse.ArgumentParser()
# parser.add_argument('--order', type=int, help='the order of magic square')
# n = parser.parse_args().order
n = 50

X = [ [ Int("x_%s_%s" % (i+1, j+1)) for j in range(n) ] for i in range(n) ]

cells_c = [ And(1 <= X[i][j], X[i][j] <= n) for i in range(n) for j in range(n) ]
dists_c = []
for i in range(n):
    dists_c += [ Distinct([X[i][j] for j in range(n)]) ]
for j in range(n):
    dists_c += [ Distinct([X[i][j] for i in range(n)]) ]

# basic magic square
Latin_c = cells_c + dists_c

# for len in range(3, n-1):
#     # exist_c = []
#     # for x in range(0, n-len):
#     #     for y in range(0, n-len):
#     #         sub_sums = Sum([X[i][y] for i in range(x, x+len)])
#     #         subMS_c = [ (Sum([ X[x+i][y+j] for i in range(len) ]) == sub_sums) for j in range(len) ]
#     #         subMS_c += [ (Sum([ X[x+i][y+j] for j in range(len) ]) == sub_sums) for i in range(len) ]
#     #         subMS_c += [ Sum([ X[x+i][y+i] for i in range(len) ]) == sub_sums ]
#     #         subMS_c += [ Sum([ X[x+i][y+n-len-i-1] for i in range(len) ]) == sub_sums ]
#     #         exist_c |= subMS_c
    
#     exist_c = [Or([And(
#                     [ (Sum([ X[x+i][y+j] for i in range(len) ]) == Sum([X[k][y] for k in range(x, x+len)])) for j in range(len) ]+
#                     [ (Sum([ X[x+i][y+j] for j in range(len) ]) == Sum([X[k][y] for k in range(x, x+len)])) for i in range(len) ]+
#                     [ Sum([ X[x+i][y+i] for i in range(len) ]) == Sum([X[k][y] for k in range(x, x+len)]) ]+
#                     [ Sum([ X[x+i][y+len-i-1] for i in range(len) ]) == Sum([X[i][y] for i in range(x, x+len)]) ]
#                     ) for x in range(0, n-len) for y in range(0, n-len)]
#                     )]
#     magicSquare_c += exist_c

s = Solver()
s.add(Latin_c)
# print(s.to_smt2())
# print('test/'+str(n)+'.smt2')
# with open('test/'+str(n)+'.smt2','w') as f:
    # print(s.to_smt2(), file=f)
if s.check() == sat:
    print ("SAT")
    # m = s.model()
    # r = [ [ m.evaluate(X[i][j]) for j in range(n) ] for i in range(n) ]
    # print_matrix(r)
else:
    print ("failed to solve")