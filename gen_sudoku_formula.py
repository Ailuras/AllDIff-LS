#!/usr/bin/python
# -*- coding: UTF-8 -*-

import math
import pickle
import sys

class sudoku:
    def __init__(self, file):
        self.path = file
        info = file.split('/')
        self.n = int(info[-1].split('_')[0].split('x')[-1])
        self.order = int(math.sqrt(self.n))
        self.fixed_prop = int(info[-1].split('_')[1])
        info[0] = 'benchmarks_SAT'
        info[-1] = info[-1].split('.')[0] + '.cnf'
        self.filename = '/'.join(info)
        self.outfile = open(self.filename, "w")
        self.clausN = 0
        self.varN = self.n*self.n*self.n


    def finite_domain(self):
        # self.outfile.write("c finite domain\n")
        for col in range(self.n):
            for row in range(self.n):
                s1 = ""
                for color in range(self.n):
                    s1 = s1 + self.get_index(col, row, color) + ' '
                s1 = s1 + "0\n"
                self.outfile.write(s1)
                self.clausN += 1
        # self.outfile.write("\n")
        for col in range(self.n):
            for row in range(self.n):
                for color1 in range(self.n):
                    s1 = '-' + self.get_index(col, row, color1) + ' '
                    for color2 in range(color1+1, self.n):
                        s2 = s1 + '-' + self.get_index(col, row, color2) + ' 0\n'
                        self.outfile.write(s2)
                        self.clausN += 1
        # self.outfile.write("\n")


    def latin_constraint(self):
        # self.outfile.write("c latin constraint\n")
        for col in range(self.n):
            for color in range(self.n):
                # s1 = ""
                # for row in range(self.n):
                #     s1 = s1 + self.get_index(col, row, color) + ' '
                # s1 = s1 + "0\n"
                # self.outfile.write(s1)
                # self.clausN += 1
                for row1 in range(self.n):
                    s1 = '-' + self.get_index(col, row1, color) + ' '
                    for row2 in range(row1+1, self.n):
                        s2 = s1 + '-' + self.get_index(col, row2, color) + ' 0\n'
                        self.outfile.write(s2)
                        self.clausN += 1
        
        for row in range(self.n):
            for color in range(self.n):
                # s1 = ""
                # for col in range(self.n):
                #     s1 = s1 + self.get_index(col, row, color) + ' '
                # s1 = s1 + "0\n"
                # self.outfile.write(s1)
                # self.clausN += 1
                for col1 in range(self.n):
                    s1 = '-' + self.get_index(col1, row, color) + ' '
                    for col2 in range(col1+1, self.n):
                        s2 = s1 + '-' + self.get_index(col2, row, color) + ' 0\n'
                        self.outfile.write(s2)
                        self.clausN += 1
        # self.outfile.write("\n")

    def sudoku_constraint(self):
        # self.outfile.write("c sudoku constraint\n")
        for group_x in range(self.order):
            for group_y in range(self.order):
                for color in range(self.n):
                        # s1 = ""
                        # for point_x in range(self.order):
                        #     for point_y in range(self.order):
                        #         s1 = s1 + self.get_index((group_x-1)*self.order + point_x, (group_y-1)*self.order + point_y, color) + ' '
                        # s1 = s1 + "0\n"
                        # self.outfile.write(s1)
                        # self.clausN += 1
                        for point_x1 in range(self.order):
                            for point_y1 in range(self.order):
                                s1 = '-' + self.get_index(group_x*self.order + point_x1, group_y*self.order + point_y1, color) + ' '
                                for point_x2 in range(point_x1+1, self.order):
                                    for point_y2 in range(self.order):
                                        if point_y1 != point_y2:
                                            s2 = s1 + '-' + self.get_index(group_x*self.order + point_x2, group_y*self.order + point_y2, color) + ' 0\n'
                                            self.outfile.write(s2)
                                            self.clausN += 1
        # self.outfile.write("\n")

    def fixed_point(self):
        # self.outfile.write("c fixed point\n")
        with open(self.path) as f:
            f.readline()
            f.readline()
            for i in range(self.n):
                line = f.readline().strip('\t\n').split('\t')
                for j in range(self.n):
                    if line[j] != '-1':
                        s1 = self.get_index(i, j, int(line[j])-1)+' 0\n'
                        self.outfile.write(s1)
                        self.clausN += 1
        # self.outfile.write("\n")

                
    def get_index(self, col, row, color):
        index = col*self.n*self.n + row*self.n + color + 1
        return str(index)


    def save(self):
        self.outfile.close()
        with open(self.filename, 'r+') as f:
            content = f.read()
            f.seek(0, 0)
            f.write('p cnf ' + str(self.varN) + ' ' + str(self.clausN) + '\n' + content)


if __name__ == "__main__":
    file = sys.argv[1]
    la = sudoku(file)
    la.finite_domain()
    la.latin_constraint()
    la.sudoku_constraint()
    la.fixed_point()
    la.save()