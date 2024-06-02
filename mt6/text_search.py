import os
import re
import time
from time import time_ns
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor
import random


def count_string_in_chunk(chunk, target_string):
    return chunk.count(target_string)
    # return len(re.findall(target_string, chunk))


def get_total_size_and_files(directory_path):
    total_size = 0
    file_paths = []
    for root, dirs, files in os.walk(directory_path):
        
        for file in files:
            file_path = os.path.join(root, file)
            if file.endswith('.txt'):
                file_size = os.path.getsize(file_path)
                total_size += file_size
                file_paths.append((file_path, file_size))
    return total_size, file_paths


def read_in_chunks(file_path, start, size):
    with open(file_path, 'r', encoding='utf-8') as file:
        file.seek(start)
        chunk = file.read(size)
    return chunk


def process_chunk(file_path, start, size, target_string):
    t_0 = time_ns()
    chunk = read_in_chunks(file_path, start, size)
    t_1 = time_ns()
    x = count_string_in_chunk(chunk, target_string)
    t_2 = time_ns()
    # print(f'Read time: {(t_1-t_0)/10**6}, Find time: {(t_2-t_1)/10**6}')
    return x


def process_tasks(tasks):
    counts = {}
    for task in tasks:
        cnt = process_chunk(*task)
        file_path = task[0]
        if file_path not in counts:
            counts[file_path] = 0
        counts[file_path] += cnt

    return counts


def threaded_count_string(directory_path, target_string):
    total_size, file_paths = get_total_size_and_files(directory_path)

    counts = {}
    futures = []
    with ThreadPoolExecutor() as executor:
        for file_path, file_size in file_paths:
            futures.append(executor.submit(process_chunk, file_path, 0, file_size, target_string))
        for file_path, future in zip(file_paths, futures):
            cnt = future.result()
            if cnt > 0:
                if file_path not in counts:
                    counts[file_path] = 0
                counts[file_path] += cnt

    return counts


def processed_count_string(directory_path, target_string, num_processes):
    total_size, file_paths = get_total_size_and_files(directory_path)
    chunk_size = total_size // num_processes
    process_tasks_list = [[] for _ in range(num_processes)]
    process_sizes = [0] * num_processes
    remaining_files = []

    for file_path, file_size in file_paths:
        assigned = False
        for i in range(num_processes):
            if process_sizes[i] + file_size <= chunk_size:
                process_tasks_list[i].append((file_path, 0, file_size, target_string))
                process_sizes[i] += file_size
                assigned = True
                break
        if not assigned:
            remaining_files.append((file_path, file_size))

    for file_path, file_size in remaining_files:
        start = 0
        for i in range(num_processes):
            if process_sizes[i] < chunk_size:
                remaining_size = chunk_size - process_sizes[i]
                size = min(remaining_size, file_size - start)
                process_tasks_list[i].append((file_path, start, size, target_string))
                process_sizes[i] += size
                start += size
                if start >= file_size:
                    break

    counts = {}
    with ProcessPoolExecutor(max_workers=num_processes) as executor:
        futures = [executor.submit(process_tasks, tasks) for tasks in process_tasks_list]
        for future in futures:
            result = future.result()
            for file_path, count in result.items():
                if count > 0:
                    if file_path not in counts:
                        counts[file_path] = 0
                    counts[file_path] += count

    return counts


def count_string(directory_path, target_string):
    total_size, file_paths = get_total_size_and_files(directory_path)

    counts = {}
    for file_path, file_size in file_paths:
        cnt = process_chunk(file_path, 0, file_size, target_string)
        if cnt:
            counts[file_path] = cnt

    return counts


def main():
    directory_paths = ['big_book', 'BIG_FILES'] 
    target_strings = ["god", 'abcde']
    num_processes = 8

    for _ in range(10):
        print(f"\n\t\tITERATION {_}")
        directory_path = 'BIG_FILES'
        target_strings = ['abcde', 'DDDDD', 'aaaaaa']

        for target_string in target_strings:
            print(f"\n\t\t---------")
            
            st = time.time()
            total_count = threaded_count_string(directory_path, target_string)
            print(f"mt Total occurrences of '{target_string}': {total_count}")
            print(f"Time for mt: {time.time() - st} sec\n")

            st = time.time()
            total_count = processed_count_string(directory_path, target_string, num_processes)
            print(f"processed Total occurrences of '{target_string}': {total_count}")
            print(f"Time for processed: {time.time() - st} sec\n")

            st = time.time()
            cnt = count_string(directory_path, target_string)
            print(f"race Total occurrences of '{target_string}': {cnt}")
            print(f"Time for race: {time.time() - st} sec")


if __name__ == "__main__":
    main()
