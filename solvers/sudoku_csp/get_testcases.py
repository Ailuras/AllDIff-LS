import pickle
import sys
import os

def get_index(index, order):
    index -= 1
    i = int(index/order) + 1
    j = index%order + 1
    return i, j

def get_mzn(origin_file, target_file):
    info = origin_file.split('/')[-1].split('-')
    order = int(info[1])
    outfile = open(target_file, "w")
    outfile.write('int: n=' + str(order) + ';\n')
    outfile.write('set of int:elem = 1..n;')
    outfile.write('array [1..n, 1..n] of var elem: latin;\n')
    outfile.write('include "alldifferent.mzn";\n')
    outfile.write('constraint forall (i in elem) (alldifferent([latin[i, j] | j in elem]));\n')
    outfile.write('constraint forall (j in elem) (alldifferent([latin[i, j] | i in elem]));\n')
    with open(origin_file) as f:
        lines = f.readlines()
        for line in lines:
            if line[0] == 'f' and len(line) < 20:
                index = line.split(' ')[1]
                color = line.split(' ')[2]
                i , j = get_index(int(index), order)
                s = 'constraint latin[' + str(i) + ', ' + str(j) + '] = ' + color + ';\n'
                # s1 = get_index(0, int(index)-1, int(color)-1)+' 0\n'
                outfile.write(s)
        outfile.write('solve satisfy;')
    outfile.close()

def cp_dir(origin_path, target_path):
    # if os.path.isdir(origin_path):
    origin_list = os.listdir(origin_path)
    for item in origin_list:
        path1 = os.path.join(origin_path, item)
        if os.path.isdir(path1):
            path2 = target_path + '/' + item
            if not os.path.exists(path2):
                os.mkdir(path2)
            cp_dir(path1, path2)
        elif os.path.isfile(path1):
            path2 = target_path + '/' + item.split('.')[0] + '.mzn'
            get_mzn(path1, path2)
    # elif os.path.isfile(path):

if __name__ == "__main__":
    origin_path = '../benchmarks/Latin-Square-Completion/'
    target_path = '../benchmarks/testcases_CP/'
    cp_dir(origin_path, target_path)