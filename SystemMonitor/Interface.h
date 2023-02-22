#pragma once
#include <functional>

#include "utility.h"
#include "Process.h"
#include "Thread.h"
#include <slint/slint.h>

using std::function;

using std::sort;
using std::bind;

constexpr int state_len = 10;
constexpr int name_len = 25;
constexpr int pid_len = 5;
constexpr int num_threads_len = 7;
constexpr int vmem_len = 10;
constexpr int cpu_usage_len = 7;
constexpr int time_len = 6;
constexpr int priority_len = 8;

class Interface {
	static bool sort_by_cpu_usage(const Process& a, const Process& b) {		
		return a.get_exec_time_diff() > b.get_exec_time_diff();
	}
	static bool sort_by_mem_usage(const Process& a, const Process& b) {
		return a.get_mem_size() > b.get_mem_size();
	}
	static bool sort_by_thread_num(const Process& a, const Process& b) {
		return a.get_num_threads() > b.get_num_threads();
	}

	static bool sort_threads(const Thread& a, const Thread& b) {
		return a.get_exec_time_diff() > b.get_exec_time_diff();
	}
public:
	static Interface& get() {
		static Interface instance;
		return instance;
	}

	inline void set_sort_mem() {
		m_sort_fun = sort_by_mem_usage;
	}
	inline void set_sort_thread() {
		m_sort_fun = sort_by_thread_num;
	}
	inline void set_sort_cpu() {
		m_sort_fun = sort_by_cpu_usage;
	}

	inline void set_show_thread() {
		m_upd_fun = bind(&Interface::output_threads_info, this);
	}
	inline void set_show_proc() {
		m_upd_fun = bind(&Interface::output_processes_info, this);
	}

	void update();

	bool send_signal(int pid, int signal);
	bool check_pid(int pid);

	void run();
private:
	Interface();
	~Interface() {};

	Interface(const Interface&) = delete;
	Interface& operator=(const Interface&) = delete;

	void output_sys_info();

	void output_processes_info();
	void output_threads_info();

	function<bool(const Process&, const Process&)> m_sort_fun = sort_by_mem_usage;
	function<void()> m_upd_fun;
};