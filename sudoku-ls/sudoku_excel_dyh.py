from sys import prefix
import pandas as pd
import random
import os
import shutil
from tqdm.notebook import tqdm
import math
# from z3 import *

if __name__ == '__main__':
    # c = sudoku(solver='sudoku_eq_ort', execl='charts/base.xlsx')
    # c.arrange()
    # c.save_to('charts/sudoku_eq_ort.xlsx')
    datas = {}
    solver = 'sudoku_ls'
    category = 'INST_81x81'
    category_list = os.listdir('results/' + solver)
    
    for folder in category_list:
        if folder != category:
            continue
        folder_path = os.path.join('results/' + solver, folder)
        file_list = os.listdir(folder_path)
        for file in tqdm(file_list, desc=folder_path):
            path = os.path.join(folder_path, file)
            with open(path) as f:
                lines = f.readlines()
                for index in range(len(lines)):
                    info = lines[index].strip()
                    result = None
                    time = None
                    if 'find answer' in info:
                        # benchmarks_EXC/INST_64x64/60/inst64x64_60_13.txt find answer: 0.000000
                        name, time = info.split(' find answer: ')
                        time = float(time)
                        name = name.split('/')
                        base, category = name[1], name[2]
                        base = base.split('x')[-1]
                        key = f'{base}-{category}'
                        if key in datas:
                            datas[key][0] += time
                            datas[key][1] += 1
                        else:
                            datas[key] = [time, 1]
                    
                    # if info[:10] == 'benchmarks':
                    #     if lines[index - 1][0] == '-':
                    #         result = 'timeout'
                    #         time = '1000.000000'
                    #     elif lines[index - 2][0] == '-':
                    #         if lines[index - 1].strip() == 'SAT':
                    #             result = 'sat'
                    #             time = int(info.split(' : ')[-1].split(' ')[0]) / 1000.0
                    #         else:
                    #             assert -1 > 0
                    #             result = 'timeout'
                    #             time = '1000.000000'
                    #     else:
                    #         assert -1 > 0
                    #     assert result != None
                    #     if result != None:
                    #         name = info.split(' ')[0].split('/')[-1]
                    #         base, category = name.split('_')[0], name.split('_')[1]
                    #         base = base.split('x')[-1]
                    #         key = f'{base}-{category}'
                    #         if result == 'sat':
                    #             if not key in datas:
                    #                 datas[key] = [float(time), 1]
                    #             else:
                    #                 datas[key][0] += float(time)
                    #                 datas[key][1] += 1
    for key in datas:
        print('-'*50)
        print(key)
        print(datas[key])
        number = datas[key][0] / datas[key][1]
        print('%.2f' % number)
        