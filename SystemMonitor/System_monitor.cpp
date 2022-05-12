#include "System_monitor.h"

#include <algorithm>

#include "system.h"

using namespace std;

void System_monitor::update()
{
	vector<pid_t> pids = ::get_pids();
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
			m_processes.emplace(pid, ::get_process_data(pid));
		}
		else {
			::update_process_data(pid, m_processes[pid]);
		}
	}

	m_last_time = m_current_time;
	m_current_time = ::get_processor_time();

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
