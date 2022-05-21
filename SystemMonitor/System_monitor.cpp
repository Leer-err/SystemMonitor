#include "System_monitor.h"

#include <algorithm>

#include "system.h"

using namespace std;

void System_monitor::update()
{
	vector<pid_t> pids = ::get_ids(proc_dir);
	vector<pid_t> unused_pids;

	for (const auto& [pid, process] : m_processes) {
		if (find(pids.begin(), pids.end(), pid) == pids.end()) {
			unused_pids.push_back(pid);
		}
	}

	for (const auto& pid : unused_pids) {
		m_processes.erase(pid);
	}

	for (const auto& pid : pids) {
		if (m_processes.find(pid) == m_processes.end()) {
			m_processes.emplace(pid, get_process_data(pid));
			vector<pid_t> process_tids = m_processes[pid].get_tids();
			for (const auto& tid : process_tids) {
				m_threads.emplace(tid, get_thread_data(pid, tid));
			}
		}
		else {
			vector<pid_t> process_tids = m_processes[pid].get_tids();
			update_process_data(pid, m_processes[pid]);
			for (const auto& tid : process_tids) {
				update_thread_data(pid, tid, m_threads[tid]);
			}
		}
	}

	m_last_time = m_current_time;
	m_current_time = ::get_cpu_time();

	m_last_load_time = m_current_load_time;
	m_current_load_time = ::get_cpu_usage();

	m_sys_mem = ::get_total_mem();
	m_used_mem = ::get_used_mem();

	m_uptime = ::get_uptime();
}

vector<Process> System_monitor::get_processes() const
{
	vector<Process> processes;

	for (const auto& [pid, process] : m_processes) {
		processes.push_back(process);
	}

	return processes;
}

vector<Thread> System_monitor::get_threads() const
{
	vector<Thread> threads;

	for (const auto& [tid, thread] : m_threads) {
		threads.push_back(thread);
	}

	return threads;
}
