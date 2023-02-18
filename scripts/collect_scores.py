from asyncio.subprocess import DEVNULL
from operator import sub
import os
import time
import glob
import shutil
import subprocess


NUM_FILES = 10


def get_job_status():
    res = subprocess.run(['gcloud', 'beta', 'run', 'jobs', 'executions', 'list'], capture_output=True).stdout.decode()
    stat = res.splitlines()[1]
    if stat.find('✔') != -1:
        return 'OK'
    elif stat.find('X') != -1:
        return 'NG'
    elif stat.find('…') != -1:
        return 'Executing'


def collect(download=True):
    os.makedirs('./results', exist_ok=True)
    if download:
        shutil.rmtree('./results/raw_result', ignore_errors=True)
        subprocess.run(['sh', 'shell_scripts/download_results.sh', './results/'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        shutil.move('./results/results', './results/raw_result')
    assert os.path.isdir('./results/raw_result')

    results = []

    pathes = glob.glob('./results/raw_result/*.txt')
    for path in pathes:
        with open(path) as f:
            results += list(map(lambda x: (int(x[0]), float(x[1])), map(lambda x: x.strip().split(','), f.readlines())))

    results.sort(key=lambda x: x[1])
    total = sum([s[1] for s in results])
    num_cases = len(results)
    ave = total / num_cases
    result = {}
    result['cases'] = num_cases
    result['total'] = total
    # result['submit'] = ave * NUM_PRETEST_CASES
    result['min'] = results[0]
    result['max'] = results[-1]
    result['ave'] = ave
    result['scores'] = results

    return result


def run_collect():
    subprocess.run(['sh', 'shell_scripts/remove_results.sh'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.run(['gcloud', 'beta', 'run', 'jobs', 'execute', 'ahc-cloud-run-jobs'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    while True:
        res = subprocess.run(['sh', 'shell_scripts/ls_results.sh'], capture_output=True)
        len = int(res.stdout.decode().strip())
        stat = get_job_status()
        print(f'status:{stat:8}, finished:{len:4}')
        if len == NUM_FILES:
            break
        assert stat != 'NG'
        assert stat == 'OK' and len != NUM_FILES
        time.sleep(1)
    result = collect()
    return result


if __name__ == '__main__':
    result = run_collect()
    for key, value in result.items():
        if key == 'scores':
            continue
        print(f'{key:6}: {value}')
