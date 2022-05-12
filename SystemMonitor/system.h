#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>

#include "Process.h"

using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;

using std::all_of;
using std::to_string;

constexpr char* proc_dir = "/proc";
constexpr char* proc_stat_file = "/stat";
constexpr char* stat_path = "/proc/stat";
constexpr char* mem_path = "/proc/meminfo";
constexpr char* uptime_path = "/proc/uptime";
constexpr char* kernel_path = "/proc/version";

Process get_process_data(pid_t pid) {
    pid_t ppid;
    string name;
    char state;
    int tty, num_threads;
    uint32_t user_time, kernel_time, mem_size;
    uint64_t start_time, dump;

    string path = proc_dir;
    path += "/";
    path += to_string(pid);
    path += proc_stat_file;

    ifstream data_file(path);

    if (data_file.is_open()) {
        string file_data;
        getline(data_file, file_data);

        istringstream st(file_data);
        st >> dump >> name >> state
            >> ppid >> dump >> dump >> tty >> dump
            >> dump >> dump >> dump >> dump >> dump >> user_time
            >> kernel_time >> dump >> dump >> dump
            >> dump >> num_threads >> dump >> start_time
            >> mem_size;

        return Process(pid, { name.begin() + 1, name.end() - 1 }, Process::parse_state(state), ppid, num_threads, mem_size, kernel_time + user_time, tty, start_time);
    }
    return Process();
}

void update_process_data(pid_t pid, Process& process) {
    pid_t ppid;
    string name;
    char state;
    int tty, num_threads;
    uint32_t user_time, kernel_time, mem_size;
    uint64_t start_time, dump;

    string path = proc_dir;
    path += "/";
    path += to_string(pid);
    path += proc_stat_file;

    ifstream data_file(path);

    if (data_file.is_open()) {
        string file_data;
        getline(data_file, file_data);

        istringstream st(file_data);
        st >> dump >> name >> state
            >> ppid >> dump >> dump >> tty >> dump
            >> dump >> dump >> dump >> dump >> dump >> user_time
            >> kernel_time >> dump >> dump >> dump
            >> dump >> num_threads >> dump >> start_time
            >> mem_size;

        process.set_mem_size(mem_size);
        process.set_num_threads(num_threads);
        process.set_state(Process::parse_state(state));
        process.set_time(kernel_time + user_time);
    }
}

vector<pid_t> get_pids() {
    vector<int> pids;
    DIR* directory = opendir(proc_dir);
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        if (file->d_type == DT_DIR) {
            string filename(file->d_name);
            if (all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

string get_kernel() {
    ifstream kernel(kernel_path);
    string line, res;

    if (kernel.is_open()) {
        getline(kernel, line);
        istringstream linestream(line);
        linestream >> res >> res >> res;
    }
    return res;
}

uint32_t get_used_mem() {
    ifstream mem(mem_path);
    uint32_t total, available;

    if (mem.is_open()) {
        while (!mem.eof()) {
            string line, key;
            getline(mem, line);
            istringstream linestream(line);
            linestream >> key;
            if (key == "MemTotal:") {
                linestream >> total;
            }
            else if (key == "MemAvailable:") {
                linestream >> available;
            }
        }
    }
    return total - available;
}

uint32_t get_total_mem() {
    ifstream mem(mem_path);
    uint32_t total = 0;

    if (mem.is_open()) {
        while (!mem.eof()) {
            string line, key;
            getline(mem, line);
            istringstream linestream(line);
            linestream >> key;
            if (key == "MemTotal:") {
                linestream >> total;
                break;
            }
        }
    }
    return total;
}

uint32_t get_cpu_usage() {
    ifstream data_file(stat_path);
    uint32_t res_time = 0;

    if (data_file.is_open()) {
        string file_data, dump;
        getline(data_file, file_data);
        istringstream st(file_data);

        st >> dump;
        for (int i = 0; !st.eof(); i++) {
            uint32_t time;

            st >> time;
            if (i != 3) res_time += time;
        }
    }

    return res_time;
}

uint32_t get_processor_time() {
    ifstream data_file(stat_path);
    uint32_t res_time = 0;

    if (data_file.is_open()) {
        string file_data, dump;
        getline(data_file, file_data);
        istringstream st(file_data);

        st >> dump;
        while (!st.eof()) {
            uint32_t time;

            st >> time;
            res_time += time;
        }
    }

    return res_time;
}

uint32_t get_uptime() {
    ifstream data_file(uptime_path);
    float res_time = 0;

    if (data_file.is_open()) {
        string file_data, dump;
        getline(data_file, file_data);
        istringstream st(file_data);

        st >> res_time;
    }

    return res_time;
}