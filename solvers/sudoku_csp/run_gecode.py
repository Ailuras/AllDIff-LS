import os
import sys
import ast
import time
import multiprocessing as mp


def exec_gecode_cmd(mznfile):
    _, mznname = os.path.split(mznfile)
    minizinc_path = '/home/hrcarryu/MiniZinc/bin/minizinc'
    solfile = './sol_plus2/' + mznname + '.24h.gecode.sol'
    if os.path.exists(solfile):
        print('[SKIP]', mznname)
        return
    #
    cmd = "timeout 86400 {} --solver gecode {} > {}".format(minizinc_path, mznfile, solfile)
    print('[EXEC]', cmd)
    t1 = time.perf_counter()
    os.system(cmd)
    t2 = time.perf_counter() - t1
    os.system('echo t {} sec >> {}'.format(t2, solfile))


def run_gecode():
    data_dir = 'mzn_plus2'
    data_files = []
    for home, dirs, files in os.walk(data_dir):
        for file in files:
            filename = os.path.join(home, file)
            if filename[-3:] == 'mzn':
                data_files.append(filename)
    pool = mp.Pool(mp.cpu_count()-6)
    pool.map(exec_gecode_cmd, data_files)
    pool.close()
    pool.join()


run_gecode()
