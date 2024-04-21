from sys import prefix
import pandas as pd
import random
import os
import shutil
from tqdm.notebook import tqdm
import math
# from z3 import *

class sudoku:
    
    def __init__(self, init=False, path='benchmarks', solver='sudoku_acs', execl='charts/base.xlsx', opt_path = '/INST_49x49/100'):
        self.path = path
        self.opt_path = path + opt_path
        self.solver = solver
        self.data_path = 'results/' + solver
        self.records = pd.read_excel(execl, header=[0], index_col = [0])
        if init:
            self.records = self.records.drop(index=self.records.index)
            self.get_info(path)
        
    def save_to(self, filename):
        self.records.to_excel(filename)
        
    def arrange(self):
        # for filename in tqdm(folder_list, desc=folder_path):
        category_list = os.listdir(self.data_path)
        for folder in category_list:
            folder_path = os.path.join(self.data_path, folder)
            file_list = os.listdir(folder_path)
            for file in tqdm(file_list, desc=folder_path):
                path = os.path.join(folder_path, file)
                with open(path) as f:
                    try:
                        if self.solver == 'sudoku_acs':
                            while f.readline():
                                result = f.readline()[:-1]
                                if result == '0':
                                    result = 'sat'
                                elif result == '1':
                                    result = 'timeout'
                                time = f.readline()[:-1]
                                if result == 'timeout':
                                    time = '1000.000000'
                                info = f.readline()
                                # time = info.split(' : ')[-1][:-5]
                                name = info.split(' : ')[0].split('/')[-1]
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_ort':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                if info == 'SAT':
                                    result = 'sat'
                                    info = f.readline()
                                    time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                                name = info.split(' ')[0].split('/')[-1]
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_ils':
                            while f.readline():
                                result = f.readline()[:-1]
                                info = f.readline()[:-1]
                                if result == '0':
                                    result = 'sat'
                                    time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                                else:
                                    result = 'timeout'
                                    time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_lsc':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_csp':
                            while f.readline():
                                result = f.readline()[:-1]
                                time = '1000.000000'
                                if result[:3] == 'SAT':
                                    result = 'sat'
                                    f.readline()
                                    f.readline()
                                elif result[:3] == '%% ':
                                    result = 'timeout'
                                    f.readline()
                                info = f.readline()
                                if result == 'sat':
                                    time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                                name = info.split(' : ')[0].split('/')[-1].split('.')[0] + '.txt'
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_gec':
                            while f.readline():
                                result = f.readline()[:-1]
                                time = '1000.000000'
                                if result[:3] == 'SAT':
                                    result = 'sat'
                                    f.readline()
                                elif result[:3] == '===':
                                    result = 'timeout'
                                info = f.readline()
                                if result == 'sat':
                                    time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                                name = info.split(' : ')[0].split('/')[-1].split('.')[0] + '.txt'
                                print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_choco':
                            while f.readline():
                                result = f.readline()[:-1]
                                time = '1000.000000'
                                if result[:3] == 'SAT':
                                    result = 'sat'
                                    f.readline()
                                elif result[:3] == '===':
                                    result = 'timeout'
                                info = f.readline()
                                if result == 'sat':
                                    time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                                name = info.split(' : ')[0].split('/')[-1].split('.')[0] + '.txt'
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_sat':
                            while f.readline():
                                info = ''
                                result = f.readline()[:-1]
                                time = '1000.000000'
                                if result[:3] == 's S':
                                    result = 'sat'
                                    info = f.readline()
                                else:
                                    info = result
                                    result = 'timeout'
                                if result == 'sat':
                                    time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                                name = info.split(' : ')[0].split('/')[-1].split('.')[0] + '.txt'
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    if "===" in f.readline():
                                        f.readline()
                                        f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_1':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_2':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_3':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_4':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_5':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_step':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_full':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                    f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_swap':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_test_tabu':
                            while f.readline():
                                info = f.readline()[:-1]
                                result = 'timeout'
                                time = '1000.000000'
                                name = info.split(' ')[0].split('/')[-1]
                                if 'find answer' in info:
                                    result = 'sat'
                                    time = info.split(': ')[-1]
                                f.readline()
                                # print(name)
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_rule1234':
                            while f.readline():
                                info = f.readline()[:-1]
                                name = info.split(' ')[0].split('/')[-1]
                                result = 'sat'
                                time = info.split(': ')[-1]
                                f.readline()
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_rule1':
                            while f.readline():
                                info = f.readline()[:-1]
                                name = info.split(' ')[0].split('/')[-1]
                                result = 'sat'
                                time = info.split(': ')[-1]
                                f.readline()
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                        elif self.solver == 'sudoku_rule234':
                            while f.readline():
                                info = f.readline()[:-1]
                                name = info.split(' ')[0].split('/')[-1]
                                result = 'sat'
                                time = info.split(': ')[-1]
                                f.readline()
                                self.records.loc[self.records['filename'] == name, ['result_' + self.solver, 'time_' + self.solver]] = [result, float(time)]
                    except:
                        print(path)
    