import time
import threading
import numpy as np
from numba import jit
from datetime import datetime, timezone, timedelta
from multiprocessing import Process, Queue
from heapq import merge


# -------------讀取&&寫入檔案------------- #
def read_file(filename):
    with open(f"{filename}.txt", 'r') as file:
        data = [int(line.strip()) for line in file]
    return data

def write_file(filename, sorted_data, execution_time, method_num):
    input_file_name = filename
    output_filename = f"{input_file_name}_output{method_num}.txt"
    with open(output_filename, 'w') as file:
        file.write("Sort:\n")
        for num in sorted_data:
            file.write(str(num) + "\n")
        file.write(f"CPU Time: {execution_time * 1000:.3f} ms\n") # 轉換為毫秒並取小數點後三位
        file.write(f"Output Time: {get_formatted_time()}\n")

def get_formatted_time():
    local_tz = timezone(timedelta(hours=+8))  # 設定時區 在這裡是UTC+8
    now = datetime.now(local_tz)
    return now.strftime('%Y-%m-%d %H:%M:%S.%f%z')   # year month date hout minute second microsecond timezone
# -------------讀取&&寫入檔案------------- #

# -------------氣泡排序/合併排序------------- #
@jit(nopython=True)   # 加速用 概念:把python code轉換成機器碼去執行（加速執行時間）
def bubble_sort(data):
    n = len(data)
    for i in range(n):
        for j in range(0, n-i-1):
            if data[j] > data[j+1]:
                data[j], data[j+1] = data[j+1], data[j]   # Swap
    return data

@jit(nopython=True)  # 加速用
def merge_sort(sorted_data):
    if len(sorted_data) <= 1:  # 資料只有一筆
        return sorted_data[0]  
    else:
        return list(merge(*sorted_data))  # from heapq module, 將多個已排序的序列合併成一個序列
# -------------氣泡排序/合併排序------------- #

# -------------平行/執行緒------------- #
def parallel_bubble_sort(data, q):
    q.put(bubble_sort(data))

def parallel_merge(pair_of_chunks, q):
    # 合併兩個已排序的數據塊(chunk)，並將合併後的結果存入q中
    q.put(list(merge(*pair_of_chunks)))

def bubble_sort_thread(data, sorted_chunks, index):
    sorted_chunks[index] = bubble_sort(data)

def merge_thread(chunk_pairs, merged_chunks, index):
    merged_chunks[index] = list(merge(*chunk_pairs))
# -------------平行/執行緒------------- #


def sort_data(method_num, data, k):
    # -------------1------------- #
    # 將N筆數目字直接進行氣泡排序
    if method_num == 1:
        sorted_data = bubble_sort(data)
    # -------------1------------- #

    # -------------2------------- #
    # 將N筆切成K份
    # 在一個process中對K份資料進行氣泡排序
    # 在用同一個process將K份資料進行合併排序
    elif method_num == 2:
        n = len(data)
        chunk_size = (n + k - 1) // k  # 取ceiling  10//3=3(取floor的話) 會有一個數據不能被處理到(至多處理3*3=9) 所以要取ceiling
        sorted_chunks = []

        # 在同一個process中對K個chunk內資料進行氣泡排序
        for i in range(k):
            start_index = i * chunk_size
            end_index = min((i + 1) * chunk_size, n)
            sorted_chunk = bubble_sort(data[start_index:end_index])
            sorted_chunks.append(sorted_chunk)

        # 把所有sorted chunk做合併排序
        while len(sorted_chunks) > 1:
            merged_chunks = []
            for i in range(0, len(sorted_chunks), 2):
                if i + 1 < len(sorted_chunks):
                    merged_chunk = list(merge(sorted_chunks[i], sorted_chunks[i + 1]))
                else:
                    # 如果chunk數是奇數，則直接將最後一個chunk加入
                    merged_chunk = sorted_chunks[i]
                merged_chunks.append(merged_chunk)
            sorted_chunks = merged_chunks

        sorted_data = sorted_chunks[0] if sorted_chunks else [] # 合併之後的結果會放在sorted_chunks[0]
    # -------------2------------- #


    # -------------3------------- #
    # 將N筆切成K份 並由K個process分別進行氣泡排序
    # 再由K-1個process進行合併排序
    elif method_num == 3:
        n = len(data)
        chunk_size = (n + k - 1) // k  
        processes = []
        queues = []
        sorted_chunks = []

        # 
        for i in range(k):
            start_index = i * chunk_size
            end_index = min((i+1) * chunk_size, n)
            q = Queue()   # 用於溝通process和process的queue from multiprocessing import Queue
            queues.append(q) # (queue)q存入(list)queues
            p = Process(target=parallel_bubble_sort, args=(data[start_index:end_index], q))
            # target:要執行的函數  args:要傳入的參數  最後將結果放入q
            p.start() # 開始執行process
            processes.append(p)

        # 通過queue獲取排序後的chunk並存入(list)sorted_chunks
        for q in queues:
            sorted_chunks.append(q.get())

        # 等待所有process結束
        for p in processes:
            p.join()  # 並不是把東西加入到p 而是阻塞當前執行流程 直到被調用的行程(在這是指p)執行完成
            # 目的:確保新進來的process(子process)執行完畢後 父process才能繼續執行

        # 用K-1個process進行合併排序
        while len(sorted_chunks) > 1:
            new_sorted_chunks = []
            merge_queues = []
            merge_processes = []

            # iterate by 2 且每次處理兩個chunk
            for i in range(0, len(sorted_chunks), 2):
                if i + 1 < len(sorted_chunks):
                    merge_q = Queue()
                    merge_queues.append(merge_q)
                    pair_of_chunks = (sorted_chunks[i], sorted_chunks[i+1])
                    merge_p = Process(target=parallel_merge, args=(pair_of_chunks, merge_q))
                    merge_p.start()
                    merge_processes.append(merge_p)
                else:
                    # 奇數個chunk直接放入新的sorted_chunks
                    new_sorted_chunks.append(sorted_chunks[i])

            for q in merge_queues:
                new_sorted_chunks.append(q.get())

            for p in merge_processes:
                p.join()

            sorted_chunks = new_sorted_chunks

        sorted_data = sorted_chunks[0] if sorted_chunks else []
    # -------------3------------- #


    # -------------4------------- #
    # 將N筆切成K份 並由K個thread分別進行氣泡排序
    # 再由K-1個thread進行合併排序
    elif method_num == 4:
        n = len(data)
        chunk_size = (n + k - 1) // k 
        threads = []
        sorted_chunks = [None] * k

        for i in range(k):
            start_index = i * chunk_size
            end_index = min((i + 1) * chunk_size, n)
            t = threading.Thread(target=bubble_sort_thread, args=(data[start_index:end_index], sorted_chunks, i))
            # target:要執行的函數  args:要傳入的參數 最後將結果存入sorted_chunks[i]
            threads.append(t)
            t.start()

        # 等待所有執行緒結束
        for t in threads:
            t.join()

        while len(sorted_chunks) > 1:
            merged_chunks = [None] * ((len(sorted_chunks) + 1) // 2)
            merge_threads = []
            for i in range(0, len(sorted_chunks) - 1, 2):
                t = threading.Thread(target=merge_thread, args=((sorted_chunks[i], sorted_chunks[i + 1]), merged_chunks, i // 2))
                merge_threads.append(t)
                t.start()

            # 排序結果是奇數 將排序的最後一個結果放入合併結果list的最後一位
            if len(sorted_chunks) % 2 != 0:
                merged_chunks[-1] = sorted_chunks[-1]

            
            for t in merge_threads:
                t.join()

            sorted_chunks = merged_chunks

        sorted_data = sorted_chunks[0] if sorted_chunks else []

    else:
        print("此為不合法的方法編號")
        return None

    return sorted_data

if __name__ == "__main__":
    input_filename = input("請輸入檔案名稱:\n")
    k = int(input("請輸入要切成幾份:\n"))
    method_num = int(input("請輸入方法編號:(方法1, 方法2, 方法3, 方法4)\n"))
    data = read_file(input_filename)
    start_time = time.time()
    sorted_data = sort_data(method_num, data, k)
    execution_time = (time.time() - start_time)
    print(f"CPU Time: {execution_time * 1000:.3f} ms") # 轉換為毫秒並取小數點後三位
    if sorted_data is not None:
        write_file(input_filename, sorted_data, execution_time, method_num)