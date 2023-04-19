#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <dirent.h>

#include "utility.h"
#include "Process.h"
#include "Thread.h"
#include "system.h"

using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;
using std::istream_iterator;
using std::numeric_limits;

using std::all_of;
using std::to_string;

constexpr char* proc_dir = "/proc";
constexpr char* task_dir = "/task";
constexpr char* proc_stat_file = "/stat";
constexpr char* stat_path = "/proc/stat";
constexpr char* mem_path = "/proc/meminfo";
constexpr char* uptime_path = "/proc/uptime";
constexpr char* network_path = "/proc/net/dev";

/*!
    \return Array of process (thread) ids in specific directory
*/
vector<pid_t> get_ids(string dir);

/*!
    \param pid process id
    \param tid thread id
    \return Data of specific thread
*/
Thread get_thread_data(pid_t pid, pid_t tid);

/*!
    \param pid process id
    \param tid thread id
    \param thread previous thread state
    \return Data of specific thread
*/
Thread update_thread_data(pid_t pid, pid_t tid, Thread thread);

/*!
    \param pid process id
    \return Data of specific process
*/
Process get_process_data(pid_t pid);

/*!
    \param pid process id
    \param process previous process state
    \return Data of specific process
*/
Process update_process_data(pid_t pid, Process process);

/*!
    \return Amount of used virtual memory
*/
uint32_t get_used_mem();

/*!
    \return Amount of installed memory
*/
uint32_t get_total_mem();

/*!
    \return Amount of cpu time in kernel or user mode
*/
uint32_t get_cpu_usage();

/*!
    \return Time since processor start
*/
uint32_t get_cpu_time();

/*!
    \return Time since processor start
*/
uint32_t get_uptime();

/*!
    \return Number of bytes transmitted since system start
*/
uint32_t get_network_transmitted();

/*!
    \return Number of bytes recieved since system start
*/
uint32_t get_network_recieved();