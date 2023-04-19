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

	map<pid_t, Process> updated_processes;
	map<pid_t, Thread> updated_threads;
	for (const auto& pid : pids) {
		if (m_processes.find(pid) == m_processes.end()) {
			updated_processes.emplace(pid, get_process_data(pid));
			vector<pid_t> process_tids = updated_processes[pid].get_tids();
			for (const auto& tid : process_tids) {
				updated_threads.emplace(tid, get_thread_data(pid, tid));
			}
		}
		else {
			vector<pid_t> process_tids = m_processes[pid].get_tids();
			updated_processes[pid] = update_process_data(pid, m_processes[pid]);
			for (const auto& tid : process_tids) {
				updated_threads[tid] = update_thread_data(pid, tid, m_threads[tid]);
			}
		}
	}

	auto current_time = ::get_cpu_time();
	auto cpu_usage = ::get_cpu_usage();
	auto total_mem = ::get_total_mem();
	auto used_mem = ::get_used_mem();
	auto uptime = ::get_uptime();

	auto net_transmitted = ::get_network_transmitted();
	auto net_recieved = ::get_network_recieved();

	m_update_mutex.lock();
	m_processes = move(updated_processes);
	m_threads = move(updated_threads);
	m_last_time = m_current_time;
	m_current_time = current_time;

	m_last_load_time = m_current_load_time;
	m_current_load_time = cpu_usage;

	m_sys_mem = total_mem;
	m_used_mem = used_mem;

	m_uptime = uptime;

	m_net_recv_prev = m_net_recv_curr;
	m_net_recv_curr = net_recieved;
	m_net_trans_prev = m_net_trans_curr;
	m_net_trans_curr = net_transmitted;

	m_update_mutex.unlock();
}

vector<Process> System_monitor::get_processes()
{
	lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
	vector<Process> processes = vector<Process>();
	processes.reserve(m_processes.size());

	for (const auto& [pid, process] : m_processes) {
		processes.push_back(process);
	}

	return processes;
}

vector<Thread> System_monitor::get_threads()
{
	lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
	vector<Thread> threads = vector<Thread>(m_threads.size());
	threads.reserve(m_threads.size());

	for (const auto& [tid, thread] : m_threads) {
		threads.push_back(thread);
	}

	return threads;
}
