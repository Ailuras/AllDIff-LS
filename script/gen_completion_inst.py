import os
import sys
import random

order = 200
def get_benchmarks(path, percentage):
    target_path = path.split('/')
    target_path[-1] = str(percentage)
    target_path = '/'.join(target_path)
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    file_list = os.listdir(path)
    for file in file_list:
        file_path = os.path.join(path, file)
        with open(file_path) as f:
            try:
                lines = []
                f.readline()
                is_sat = f.readline().strip()
                for i in range(order):
                    lines.append(f.readline().strip())
                index = 0
                total = (100-percentage)*order/100
                while index < total:
                    x = random.randint(0, order-1)
                    if lines[x] != '-1':
                        lines[x] = '-1'
                        index += 1
                filename = file.split('_')
                filename[1] = str(percentage)
                filename = '_'.join(filename)
                with open(target_path + '/' + filename, 'w') as out:
                    out.write(f'{order}\n')
                    out.write(f'{is_sat}\n')
                    for i in range(order):
                        out.write(f'{lines[i]}\n')
            except:
                print(f'{percentage}: {file_path}')

if __name__ == "__main__":
    order = int(sys.argv[1])
    path = f'benchmarks/N-Queens/INST_{order}/100'
    for i in range(0, 100, 10):
        get_benchmarks(path, percentage=i)
