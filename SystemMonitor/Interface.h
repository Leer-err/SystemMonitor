#pragma once
#include <functional>
#include <ncurses.h>

#include "utility.h"
#include "Process.h"
#include "Thread.h"

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

	inline void set_window_size() {
		getmaxyx(stdscr, m_terminal_height, m_terminal_width);
	}

	void update();

	void show_all();

	void send_signal();

	void set_show_num();

	void print_help();
private:
	Interface();
	~Interface() { endwin(); }

	Interface(const Interface&) = delete;
	Interface& operator=(const Interface&) = delete;

	void draw_line();
	void color(float val);
	void set_default_color();

	void press_any_key();

	int get_number();

	void print_help_hint();

	void output_sys_info();

	void output_processes_info();
	void output_threads_info();

	function<bool(const Process&, const Process&)> m_sort_fun = sort_by_mem_usage;
	function<void()> m_upd_fun;

	int m_terminal_width;
	int m_terminal_height;

	int m_show_num = 10;
};