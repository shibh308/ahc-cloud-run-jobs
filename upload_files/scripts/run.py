import os
import subprocess


CASE_START = 1
CASE_END = 1000


def execute_single_case(filename):
    try:
        # timeoutをまともな値に設定していないので注意！
        result_str = subprocess.run(['sh', 'scripts/execute_onecase.sh', filename], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True, timeout=100.0).stderr
        prefix = 'Score = '
        if result_str.startswith(prefix):
            return float(result_str[len(prefix):])
        else:
            raise RuntimeError(f'score parse error: "{result_str}"')
    except Exception:
        return 0


def main():
    task_index = int(os.environ['CLOUD_RUN_TASK_INDEX'])
    task_count = int(os.environ['CLOUD_RUN_TASK_COUNT'])

    filename = f'result_{task_index}.txt'

    st = CASE_START + (CASE_END - CASE_START) * task_index // task_count
    en = CASE_START + (CASE_END - CASE_START) * (task_index + 1) // task_count

    filenames = []
    for seed in range(st, en):
        filenames.append(f'in/{seed:04}.txt')

    subprocess.run(['unzip', 'tester/in.zip'] + filenames)
    
    results = []
    for seed, filename in zip(range(st, en), filenames):
        score = execute_single_case(filename)
        results.append((seed, score))
    
    filename = f'result_{task_index}.txt'
    with open(filename, 'w') as f:
        for (seed, score) in results:
            f.write(f'{seed},{score}\n')

    subprocess.run(['sh', 'scripts/upload_result.sh', filename])


if __name__ == '__main__':
    main()
