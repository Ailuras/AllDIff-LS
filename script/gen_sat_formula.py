#!/usr/bin/python
# -*- coding: UTF-8 -*-

import os
import sys

class nqueens:
    def __init__(self, file, target, type=1):
        if type == 1:
            self.path = file
            self.n = int(file.split('/')[-1].split('_')[0].split('t')[-1])
            self.target = target
            self.outfile = open(self.target, "w")
            self.clausN = 0
            self.varN = self.n
        else:
            self.path = file
            info = file.split('/')
            self.n = 8
            info[0] = 'benchmarks_SAT'
            info[-1] = info[-1].split('.')[0] + '.cnf'
            self.target = '/'.join(info)
            self.outfile = open(self.target, "w")
            self.clausN = 0
            self.varN = self.n

    def finite_domain(self):
        # self.outfile.write("c finite domain\n")
        for i in range(self.n):
            s1 = ""
            for j in range(self.n):
                s1 = s1 + self.get_index(i, j) + ' '
            s1 = s1 + "0\n"
            self.outfile.write(s1)
            self.clausN += 1
        # self.outfile.write("\n")
        for i in range(self.n):
            for j1 in range(self.n):
                s1 = '-' + self.get_index(i, j1) + ' '
                for j2 in range(j1+1, self.n):
                    s2 = s1 + '-' + self.get_index(i, j2) + ' 0\n'
                    self.outfile.write(s2)
                    self.clausN += 1
        # self.outfile.write("\n")
        for j in range(self.n):
            for i1 in range(self.n):
                s1 = '-' + self.get_index(i1, j) + ' '
                for i2 in range(i1+1, self.n):
                    s2 = s1 + '-' + self.get_index(i2, j) + ' 0\n'
                    self.outfile.write(s2)
                    self.clausN += 1
        # self.outfile.write("\n")

    def diag_constraint(self):
        # self.outfile.write("c diag constraint\n")
        for i in range(self.n):
            for c1 in range(self.n):
                for j in range(self.n):
                    for c2 in range(self.n):
                        index1 = self.get_index(i, c1)
                        index2 = self.get_index(j, c2)
                        if i < j:
                            if i-c1 == j-c2:
                                s1 = f'-{index1} -{index2} 0\n'
                                self.outfile.write(s1)
                                self.clausN += 1
        for i in range(self.n):
            for c1 in range(self.n):
                for j in range(self.n):
                    for c2 in range(self.n):
                        index1 = self.get_index(i, c1)
                        index2 = self.get_index(j, c2)
                        if i < j:
                            if i+c1 == j+c2:
                                s1 = f'-{index1} -{index2} 0\n'
                                self.outfile.write(s1)
                                self.clausN += 1

    def fixed_point(self):
        # self.outfile.write("c fixed point\n")
        with open(self.path) as f:
            f.readline()
            f.readline()
            for i in range(self.n):
                item = f.readline().strip()
                if item != '-1':
                    s1 = self.get_index(i, int(item)-1)+' 0\n'
                    self.outfile.write(s1)
                    self.clausN += 1
        # self.outfile.write("\n")
              
    def get_index(self, i, j):
        index = i*self.n + j + 1
        return str(index)

    def save(self):
        self.outfile.close()
        with open(self.target, 'r+') as f:
            content = f.read()
            f.seek(0, 0)
            f.write('p cnf ' + str(self.varN * self.varN) + ' ' + str(self.clausN) + '\n' + content)

if __name__ == "__main__":
    file = sys.argv[1]
    target = sys.argv[2]
    la = nqueens(file, target, type=1)
    la.finite_domain()
    la.diag_constraint()
    la.fixed_point()
    la.save()