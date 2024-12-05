#11027142 林蕙郁
from collections import defaultdict, deque
import heapq

def read_input(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    method, time_slice = map(int, lines[0].strip().split())     # 第一行第一個數字:method 第二個數字:time_slice
    processes = []
    for line in lines[2:]:
        if line.strip():  # Ensure the line is not empty
            parts = line.split()
            processes.append({
                'pid': parts[0],
                'cpu_burst': int(parts[1]),
                'arrival_time': int(parts[2]),
                'priority': int(parts[3])
            })

    return method, time_slice, processes

def write_output(filename, method, gantt_chart, waiting_time_list, turnaround_time_list):
    # 利用dict把method name存起來 在寫檔案的時候用來寫入檔案的標頭
    method_name = {
        1: "FCFS",
        2: "RR",
        3: "SJF",
        4: "SRTF",
        5: "HRRN",
        6: "PPRR",
        7: "All"
    }.get(method, "Unknown")

    with open(filename, 'w') as file:
        file.write(f"== {method_name} ==\n")
        file.write(f"{gantt_chart}\n\n")
        file.write("Waiting Time\nID\t" + method_name + "\n")
        for pid in sorted(waiting_time_list.keys(), key=int):
            file.write(f"{pid}\t{waiting_time_list[pid]}\n")
        file.write("\nTurnaround Time\nID\t" + method_name + "\n")
        for pid in sorted(turnaround_time_list.keys(), key=int):
            file.write(f"{pid}\t{turnaround_time_list[pid]}\n")

def pid_to_label(pid):
    pid = int(pid)
    if pid < 10:
        return str(pid)
    else:
        return chr(pid - 10 + ord('A'))
    


def sort_key_arr_pid(process): # 先依據arrival time排序 如果相同的話再依據pid
    return (process['arrival_time'], int(process['pid']))

def sort_key_cpu_arr_pid(process): # 先依據cpu burst  再來arrival time排序 如果相同的話再依據pid
    return (process['cpu_burst'], process['arrival_time'], int(process['pid']))

def sort_key_arr_cpu_pid(process): 
    return ( process['arrival_time'], process['cpu_burst'], int(process['pid']))

def sort_key_arr_pri_pid(process): 
    return ( process['arrival_time'], process['priority'], int(process['pid'])) 

def sort_key_pri_pid(process): 
    return ( process['priority'], int(process['pid']))

def get_arrival_time(process):
    return process['arrival_time']


def fcfs(processes):
    #依照arrival time先後次序進行排程
    #arrival time相同的話依照pid小至大進行排程
    processes.sort(key=sort_key_arr_pid)  # 利用sort_key_arr_pid排序
    current_time = 0
    gantt_chart = ''
    waiting_time_list = {}
    turnaround_time_list = {}

    for proc in processes:
        if current_time < proc['arrival_time']:
            gantt_chart += '-' * (proc['arrival_time'] - current_time)  # idle
            current_time = proc['arrival_time']
    
        start_time = current_time  # 記錄開始執行時間
        proc_label = pid_to_label(proc['pid']) * proc['cpu_burst']
        gantt_chart += proc_label
    
        completion_time = current_time + proc['cpu_burst']  # 結束這個procrss的時間
        waiting_time = start_time - proc['arrival_time']  # waiting = 開始執行時間 - 到達時間
        turnaround_time = completion_time - proc['arrival_time']  # turnaround = 結束時間 - 到達時間
    
        waiting_time_list[proc['pid']] = waiting_time
        turnaround_time_list[proc['pid']] = turnaround_time
        current_time = completion_time

    return gantt_chart, waiting_time_list, turnaround_time_list


def rr(processes, time_slice):
    # 依照arrival time先後次序進行排程
    # arrival time相同的話依照pid小至大進行排程
    # timeout後重新排隊 如果有新的process進來的話 讓新來的排前面
    # process time slice未用完 讓下一個process執行 擁有完整的time slice
    processes.sort(key=sort_key_arr_pid)
    queue = deque() #做事用的queue
    current_time = 0
    gantt_chart = ''
    waiting_time_list = {}
    turnaround_time_list = {}
    last_end_times = {}

    # 初始化
    for proc in processes:
        waiting_time_list[proc['pid']] = 0
        turnaround_time_list[proc['pid']] = 0
        last_end_times[proc['pid']] = proc['arrival_time']   # 上次開始執行的時間 會在做完一次RR的時候依據pid更新
    

    all_processes = deque(processes) #所有的processes 而會用arrival time去判斷要不要把new_arr內的process加入到process_list裡面
    

    while all_processes or queue: #all_processes還有process或是queue還有process
        while all_processes and all_processes[0]['arrival_time'] <= current_time: # all_preocesses非空且第一個process的arrival time小於等於current time
            queue.append(all_processes.popleft()) # 要把他加入到要做事的queue內
        
        if queue:
            current_proc = queue.popleft()
            execute_time = min(current_proc['cpu_burst'], time_slice) # 決定好他會做多久 如果time_slice比cpu_burst大的話就做cpu_burst(取小的) 然後把這個process清掉
            #反之就是exe_time=time_slice 最後會timeout 重新回到queue的尾端排隊
            
            if current_time >= last_end_times[current_proc['pid']]: # 每次執行的時候都要更新waiting time
                waiting_time_list[current_proc['pid']] += (current_time - last_end_times[current_proc['pid']])
                #更新後的等待時間=原本的等待時間+(這次開始執行的時間-上次執行結束的時間)
            
            proc_label = pid_to_label(current_proc['pid'])
            gantt_chart += proc_label * execute_time
            
            current_proc['cpu_burst'] -= execute_time
            last_end_times[current_proc['pid']] = current_time + execute_time #更新上次執行完後的結束時間
            current_time += execute_time
            
            if current_proc['cpu_burst'] > 0:  # timeout
                while all_processes and all_processes[0]['arrival_time'] <= current_time: # 這個等於包含了新進來的process要排在重新排隊的process的前面
                    queue.append(all_processes.popleft())
                queue.append(current_proc) # 重新排隊
            else:  # Process finished
                # 因為deque 所以在他被選中執行的時候會從queue前面Popleft出來 如果完成執行不會加回到queue內
                turnaround_time_list[current_proc['pid']] = current_time - current_proc['arrival_time']

        else:  # queue is empty
            if all_processes: # processes裡面還有process還沒跑完--->idle
                idle_time = all_processes[0]['arrival_time'] - current_time
                gantt_chart += '-' * idle_time
                current_time += idle_time
    
    return gantt_chart, waiting_time_list, turnaround_time_list


def sjf(processes):
    #shortest job first
    # CPU最小的process先執行
    # CPU相同的話arrival time小的先執行
    # CPU&&arrival time相同的話pid小的先執行
    current_time = 0
    gantt_chart = ''
    waiting_time_list = {}
    turnaround_time_list = {}

    processes.sort(key=sort_key_cpu_arr_pid)  # 利用sort_key_cput_arr_pid排序

    while processes:
        available_processes = []   # 可以執行的process
        for p in processes:
            if p['arrival_time'] <= current_time: # 依據arrival time來判斷是否可以放入可執行的list
                available_processes.append(p) 

        if not available_processes: # 如果沒有可以執行的process 直接去找最接近的那個 且甘特圖要補上idle
            next_proc = min(processes, key=get_arrival_time)
            
            idle_time = next_proc['arrival_time'] - current_time
            gantt_chart += '-' * idle_time
            current_time = next_proc['arrival_time']
            continue
        
        current_proc = available_processes[0] # 取出CPU最小的process processes已經排序過 所以第一個會是CPU最小 arr最小 PID最小的process
        
        proc_label = pid_to_label(current_proc['pid'])
        gantt_chart += proc_label * current_proc['cpu_burst']
        current_time += current_proc['cpu_burst']

       
        waiting_time_list[current_proc['pid']] = current_time - current_proc['arrival_time'] - current_proc['cpu_burst']
        turnaround_time_list[current_proc['pid']] = current_time - current_proc['arrival_time']

        processes.remove(current_proc)

    return gantt_chart, waiting_time_list, turnaround_time_list

def srtf(processes):
    # shortest remaining time first
    # 剩餘CPU最小的process先排序
    # 剩餘CPU相同的話arrival time小的先執行
    # 剩餘CPU && arrival time相同的話pid小的先執行
    processes.sort(key=sort_key_arr_cpu_pid)  
    ready_queue = []
    current_time = 0
    gantt_chart = ''
    waiting_time_list = {}
    turnaround_time_list = {}
    process_index = 0  # 正在處理的processes的index紀錄(可能第0個位置的東西放入ready queue了 下次檢查要從1開始)

    for proc in processes:
        waiting_time_list[proc['pid']] = 0
        turnaround_time_list[proc['pid']] = 0

    while process_index < len(processes) or ready_queue:
        while process_index < len(processes) and processes[process_index]['arrival_time'] <= current_time:
            # 如果arrival time小於等於current time 就把這個process加入到ready queue(heapq)
            heapq.heappush(ready_queue, (
                processes[process_index]['cpu_burst'],
                processes[process_index]['arrival_time'],
                int(processes[process_index]['pid']),
                processes[process_index]
            ))
            process_index += 1
        
        if ready_queue:
            # ready queue最上面的process是arr cpu pid都是最小的
            # 把這個process從heapq中pop出來 且提取要的資料
            min_burst, _, pid, proc = heapq.heappop(ready_queue)
            
            # 確認min cpu burst了 所以直接往上加 去看下一個process的start time
            next_event_time = current_time + min_burst
            # 如果有process的arr比next event time小的話 就把next event time設為這個process的arr time
            if process_index < len(processes) and processes[process_index]['arrival_time'] < next_event_time:
                next_event_time = processes[process_index]['arrival_time']

            execute_time = next_event_time - current_time  # 計算這個process在這輪的總執行時間
            gantt_chart += pid_to_label(pid) * execute_time  

            # 更新current time
            current_time += execute_time

            # 更新process的cpu burst
            remaining_burst = min_burst - execute_time
            if remaining_burst > 0:
                # 如果還沒結束(CPU burst!=0) 就把這個process重新放回heapq
                heapq.heappush(ready_queue, (remaining_burst, proc['arrival_time'], pid, proc))
            else:
                turnaround_time_list[proc['pid']] = current_time - proc['arrival_time']
                waiting_time_list[proc['pid']] = turnaround_time_list[proc['pid']] - proc['cpu_burst']
        else: # ready queue is empty
            if process_index < len(processes):
                gantt_chart += '-' * (processes[process_index]['arrival_time'] - current_time)
                current_time = processes[process_index]['arrival_time']

    return gantt_chart, waiting_time_list, turnaround_time_list


def hrrn(processes):
    # Highest Response Ratio Next
    # 比率越高的process優先執行
    # 比率相同 按照arrival time小的先執行
    # 比率相同&&arrival time相同 按照pid小的先執行
    current_time = 0
    gantt_chart = ''
    waiting_time_list = {}
    turnaround_time_list = {}

    copy_procrsses = processes[:] # copy一份processes

    for proc in processes:
        waiting_time_list[proc['pid']] = 0
        turnaround_time_list[proc['pid']] = 0

    while copy_procrsses:
        highest_response_ratio = -1
        hrratio_list = []
        for proc in copy_procrsses:
            if proc['arrival_time'] <= current_time:
                burst_time = proc['cpu_burst'] #CPU burst
                wait_time = current_time - proc['arrival_time'] #等待時間
                if burst_time == 0:
                    response_ratio = -99999  # 以免跑錯用 因為不能讓分母是0
                else:
                    response_ratio = (wait_time + burst_time) / burst_time
                if response_ratio > highest_response_ratio:
                    highest_response_ratio = response_ratio
                    hrratio_list = [(proc['arrival_time'], int(proc['pid']), proc)] #只有highest response ratio的process 其他會被清掉
                elif response_ratio == highest_response_ratio:
                    hrratio_list.append((proc['arrival_time'], int(proc['pid']), proc)) # 同個ratio的process 會加進去


        if hrratio_list:  # 在這裡才依照arr pid去排序
            hrratio_list.sort(key=lambda x: (x[0], x[1]))  # 用def get arr pid會type error 改用lambda去排序[0]:arr [1]:pid
            next_proc = hrratio_list[0][2]  # hhratio_list[0]是arr [1]:pid [2]:process 所以取第三個
        else:
            next_arrival = min(copy_procrsses, key=get_arrival_time) # 如果沒有可以執行的process 就找最接近的那個
            idle_time = next_arrival['arrival_time'] - current_time
            gantt_chart += '-' * idle_time
            current_time = next_arrival['arrival_time']
            continue

        execute_time = next_proc['cpu_burst']
        proc_label = pid_to_label(next_proc['pid']) * execute_time
        gantt_chart += proc_label
        current_time += execute_time

        # 更新等待時間和完成時間
        waiting_time_list[next_proc['pid']] += current_time - next_proc['arrival_time'] - next_proc['cpu_burst']
        turnaround_time_list[next_proc['pid']] = current_time - next_proc['arrival_time']

        # 移除已完成的process
        copy_procrsses.remove(next_proc)

    return gantt_chart, waiting_time_list, turnaround_time_list


#1.依照priority大小依序處理 priority"小"的process代表優先處理
#2.如果priority相同的process不只一個 採用RR原則進行排程
#3.若有priority相同的Process正在執行中 需等待他的時間片段用完
#4.當timeout或被preemptive時 從佇列尾端開始依照priority大小排序 如果恰巧有新來的process且priority相同 就讓新來的process排在前面
def pprr(processes, time_slice):
    # process : pid, cpu_burst, arrival_time, priority
    gantt_chart = ''
    t = 0
    waiting_time_list = {proc['pid']: 0 for proc in processes}
    turnaround_time_list = {}
    processes.sort(key=sort_key_arr_pri_pid)  # arrival time -> priority -> pid
    initial_cpu_bursts = {proc['pid']: proc['cpu_burst'] for proc in processes}  
    time_sliceInit = time_slice
    pre_min_priority = None
    timeout = False
    process_list = []
    samearr_list = []

    while processes or process_list:
        for p in processes[:]: 
            if p['arrival_time'] == t: #要看是不是有不只一個process同時進入
                samearr_list.append(p)
                processes.remove(p)

            elif p['arrival_time'] < t: #不是同時進入就依照priority去排
                insert_index = next((i for i, proc in enumerate(process_list) 
                                if proc['priority'] < p['priority'] ), len(process_list)) # 依照pri去插入應該的位置
                process_list.insert(insert_index, p)
                processes.remove(p)

        if samearr_list.__len__() > 1: # 多個同時進入的 先排pri 在排pid並加入到proceses_list
            samearr_list.sort(key=sort_key_pri_pid) 
            process_list = process_list + samearr_list
        elif samearr_list.__len__() == 1:
            process_list = process_list + samearr_list

        samearr_list = [] #init
        available = {}
        for p in process_list:
            if p['priority'] not in available:
                available[p['priority']] = []
            available[p['priority']].append(p)

        
        if not available:
            t += 1
            gantt_chart += '-'  # idle time
            continue
        else:
            min_priority = min(available.keys())
            if timeout: #timeout 為了不要讓他跑進去elif那個條件 所以要先判斷好
                timeout = False
            elif min_priority != pre_min_priority and pre_min_priority is not None: #被搶奪
                time_slice = time_sliceInit
                if pre_process in process_list:
                    process_list.remove(pre_process)
                    process_list.append(pre_process)

            process = available[min_priority][0]  # priority最小&&arrival time最前面的的process

            if process['cpu_burst'] > time_slice and time_slice > 0:
                t += 1
                time_slice -= 1
                process['cpu_burst'] -= 1
                ppid = pid_to_label(process['pid'])
                gantt_chart += ppid * 1
            else:
                t += 1
                process['cpu_burst'] -= 1
                ppid = pid_to_label(process['pid'])
                gantt_chart += ppid * 1

            if process['cpu_burst'] == 0 or time_slice == 0:
                process_list.remove(process)
            if process['cpu_burst'] == 0:
                turnaround_time_list[process['pid']] = t - process['arrival_time']
                waiting_time_list[process['pid']] = turnaround_time_list[process['pid']] - initial_cpu_bursts[process['pid']]
                time_slice = time_sliceInit

            elif time_slice == 0:
                # timeout 重新排隊
                process_list.append(process)
                time_slice = time_sliceInit
                timeout = True

            else: #紀錄上一次的process跟priority 下一秒的時候要判斷是否會被搶奪
                pre_process = process
                pre_min_priority = min_priority
                continue

    return gantt_chart, waiting_time_list, turnaround_time_list



def run_all_methods_and_combine(processes, time_slice):
    # Create deep copies of the process list for each method
    import copy            #不使用deep copy的話他只會跑到RR 後面全部都跑不到
    methods = {
        'FCFS': fcfs(copy.deepcopy(processes)),
        'RR': rr(copy.deepcopy(processes), time_slice),
        'SJF': sjf(copy.deepcopy(processes)),
        'SRTF': srtf(copy.deepcopy(processes)),
        'HRRN': hrrn(copy.deepcopy(processes)),
        'PPRR': pprr(copy.deepcopy(processes), time_slice)
    }
    return methods


def write_combined_output(filename, results):
    # Open the file for writing
    with open(filename, 'w') as file:
        # Write Gantt charts for all methods
        file.write("All\n")
        for method_name, data in results.items():
            gantt_chart, _, _ = data
            file.write(f"==        {method_name}==\n{gantt_chart}\n")
        file.write("===========================================================\n\n")

        # Ensure all PIDs are represented and sorted
        pids = sorted({pid for method in results for pid in results[method][1]}, key=lambda x: int(x))

        # Write waiting times for all methods
        file.write("Waiting Time\nID\t" + "\t".join(results.keys()) + "\n")
        file.write("===========================================================\n")
        
        for pid in pids:
            waiting_time_list = [str(results[method][1].get(pid, '-')) for method in results]
            file.write(f"{pid}\t" + "\t".join(waiting_time_list) + "\n")
        
        file.write("===========================================================\n\n")
        
        # Write turnaround times for all methods
        file.write("Turnaround Time\nID\t" + "\t".join(results.keys()) + "\n")
        file.write("===========================================================\n")
        
        for pid in pids:
            turnaround_time_list = [str(results[method][2].get(pid, '-')) for method in results]
            file.write(f"{pid}\t" + "\t".join(turnaround_time_list) + "\n")
        file.write("===========================================================\n")

        

def main():
    input_file = input("Please enter the input file name(with .txt): ")
    method, time_slice, processes = read_input(input_file)
    
    if method == 1:    # FCFS
        gantt_chart, waiting_time_list, turnaround_time_list = fcfs(processes)
    elif method == 2:  # RR
        gantt_chart, waiting_time_list, turnaround_time_list = rr(processes, time_slice)
    elif method == 3:  # SJF
        gantt_chart, waiting_time_list, turnaround_time_list = sjf(processes)
    elif method == 4:  # SRTF
        gantt_chart, waiting_time_list, turnaround_time_list = srtf(processes)
    elif method == 5:  # HRRN
        gantt_chart, waiting_time_list, turnaround_time_list = hrrn(processes)
    elif method == 6:  # PPRR
        gantt_chart, waiting_time_list, turnaround_time_list = pprr(processes, time_slice)
    elif method == 7:
        results = run_all_methods_and_combine(processes, time_slice)
        output_file = f'out_{input_file}'
        write_combined_output(output_file, results)
        print(f"Written to {output_file} successfully")
        return
    else:
        print("Unknown scheduling method")
        return
    
    output_file = f'out_{input_file}.txt'
    write_output(output_file, method, gantt_chart, waiting_time_list, turnaround_time_list)
    print(f"Output written to {output_file}")

if __name__ == "__main__":
    main()

