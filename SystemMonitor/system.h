#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>

#include "utility.h"
#include "Process.h"
#include "Thread.h"
#include "system.h"

using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;

using std::all_of;
using std::to_string;

constexpr char* proc_dir = "/proc";
constexpr char* task_dir = "/task";
constexpr char* proc_stat_file = "/stat";
constexpr char* stat_path = "/proc/stat";
constexpr char* mem_path = "/proc/meminfo";
constexpr char* uptime_path = "/proc/uptime";
constexpr char* kernel_path = "/proc/version";

vector<pid_t> get_ids(string dir);

Thread get_thread_data(pid_t pid, pid_t tid);

void update_thread_data(pid_t pid, pid_t tid, Thread& thread);

Process get_process_data(pid_t pid);

void update_process_data(pid_t pid, Process& process);

vector<pid_t> get_ids(string dir);

uint32_t get_used_mem();

uint32_t get_total_mem();

uint32_t get_cpu_usage();

uint32_t get_cpu_time();

uint32_t get_uptime();