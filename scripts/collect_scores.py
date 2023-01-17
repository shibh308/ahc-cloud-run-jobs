import os
import glob
import shutil
import subprocess


def main(download=True):
    os.makedirs('./results', exist_ok=True)
    if download:
        shutil.rmtree('./results/raw_result', ignore_errors=True)
        subprocess.run(['sh', 'shell_scripts/download_results.sh', './results/'])
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

    for key, value in result.items():
        if key == 'scores':
            continue
        print(f'{key:6}: {value}')


if __name__ == '__main__':
    main(download=True)