#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <functional>
#include "System_monitor.h"

#define OUT_STATE 1
#define OUT_PPID 2
#define OUT_NUM_THREADS 4
#define OUT_VMEM 8
#define OUT_PROCESSOR_USAGE 16
#define OUT_START_TIME 32
#define OUT_TTY 64

constexpr int state_len = 8;
constexpr int name_len = 20;
constexpr int pid_len = 5;
constexpr int num_threads_len = 7;
constexpr int vmem_len = 5;
constexpr int processor_usage_len = 5;
constexpr int start_time_len = 6;
constexpr int tty_len = 8;

using std::cout;
using std::vector;
using std::function;

using std::sort;
using std::bind;
using std::right;
using std::setw;
using std::setprecision;
using std::fixed;
using std::setfill;
using std::endl;

class Output_manager {
	static bool sort_by_cpu_usage(const Process& a, const Process& b) {
		return a.get_exec_time_diff() > b.get_exec_time_diff();
	}

	static bool sort_by_mem_usage(const Process& a, const Process& b) {
		return a.get_mem_size() > b.get_mem_size();
	}

	static bool sort_by_thread_num(const Process& a, const Process& b) {
		return a.get_num_threads() > b.get_num_threads();
	}
public:
	static Output_manager& get() {
		static Output_manager instance;
		return instance;
	}

	inline void set_sort_mem() { m_sort_fun = sort_by_mem_usage; }
	inline void set_sort_thread() { m_sort_fun = sort_by_thread_num; }
	inline void set_sort_cpu() { m_sort_fun = sort_by_cpu_usage; }

	inline void set_show_num(int show_num) { m_proc_show_num = show_num; }

	void output_sys_info() {
		system("clear");

		uint32_t uptime = System_monitor::get().get_uptime();

		cout << endl;
		cout << setw(11) << "Memory: ";
		cout << setprecision(1) << fixed
			<< (float)System_monitor::get().get_used_mem() / 1024 << "/"
			<< (float)System_monitor::get().get_sys_mem() / 1024
			<< " MB" << endl;

		cout << setw(9) << "CPU:  ";
		cout << setprecision(1) << fixed
			<< (float)System_monitor::get().get_load_delta() / System_monitor::get().get_cpu_delta()
			<< " %" << endl;

		cout << setw(11) << "Uptime: ";
		cout << uptime / 3600 << ":";
		uptime %= 3600;
		cout << uptime / 60 << "." << uptime % 60 << endl << endl;
	}

	void output_processes_info() {
		uint32_t processor_time = System_monitor::get().get_cpu_delta();

		cout << setw(pid_len) << "PID";
		cout << setw(name_len + 1) << "Name";
		if (m_output_mask && OUT_PPID) {
			cout << setw(pid_len + 1) << "PPID";
		}
		if (m_output_mask && OUT_VMEM) {
			cout << setw(vmem_len + 1) << "Vmem";
		}
		if (m_output_mask && OUT_PROCESSOR_USAGE) {
			cout << setw(processor_usage_len + 1) << "%Proc";
		}
		if (m_output_mask && OUT_STATE) {
			//cout << setw(state_len + 1) << "State";
		}
		if (m_output_mask && OUT_NUM_THREADS) {
			cout << setw(num_threads_len + 1) << "Threads";
		}
		if (m_output_mask && OUT_START_TIME) {
			cout << setw(start_time_len + 1) << "Time+";
		}
		if (m_output_mask && OUT_TTY) {
			//cout << setw(tty_len + 1) << "Tty";
		}
		cout << endl;

		vector<Process> processes = System_monitor::get().get_processes();
		sort(processes.begin(), processes.end(), m_sort_fun);

		int num = m_proc_show_num;
		for (const auto& process : processes) {
			if (num == 0) break;
			if (process.get_state() == Process::State::invalid) continue;
			cout << setw(pid_len) << process.get_pid();
			cout << setw(name_len + 1) << process.get_name();
			if (m_output_mask && OUT_PPID) {
				cout << setw(pid_len + 1) << process.get_ppid();
			}
			if (m_output_mask && OUT_VMEM) {
				cout << setw(vmem_len + 1) << process.get_mem_size() / 1048576;
			}
			if (m_output_mask && OUT_PROCESSOR_USAGE) {
				cout << setw(processor_usage_len + 1) << (float)process.get_exec_time_diff() / processor_time * 100;
			}
			if (m_output_mask && OUT_STATE) {
				//cout << setw(state_len + 1) << process.get_state();
			}
			if (m_output_mask && OUT_NUM_THREADS) {
				cout << setw(num_threads_len + 1) << process.get_num_threads();
			}
			if (m_output_mask && OUT_START_TIME) {
				int time = process.get_start_time() / sysconf(_SC_CLK_TCK);
				cout << setw(start_time_len/2 + 1) << time / 3600 << '.'
					<< setfill('0') << setw(start_time_len - start_time_len / 2 - 1) 
					<< (time % 3600) / 60 << setfill(' ');
			}
			if (m_output_mask && OUT_TTY) {
				//cout << setw(tty_len + 1) << ;
			}
			cout << endl;
			num--;
		}
		cout << "type h for help" << endl;
	}
private:
	Output_manager() = default;
	~Output_manager() = default;

	Output_manager(const Output_manager&) = delete;
	Output_manager& operator=(const Output_manager&) = default;

	uint32_t m_output_mask = 0xffffffff;

	function<bool (const Process&, const Process&)> m_sort_fun = sort_by_mem_usage;

	int m_terminal_width;
	int m_terminal_height;

	int m_proc_show_num = 10;
};