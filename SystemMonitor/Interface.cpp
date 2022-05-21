#include "Interface.h"

#include <sys/signal.h>

#include "System_monitor.h"

Interface::Interface() {
	initscr();
	start_color();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);

	cbreak();
	noecho();
	nodelay(stdscr, true);

	m_upd_fun = std::bind(&Interface::output_processes_info, this);
}

void Interface::update() {
	output_sys_info();
	m_upd_fun();
	print_help_hint();
}

void Interface::press_any_key() {
	nodelay(stdscr, false);
	getch();
	nodelay(stdscr, true);
}

void Interface::show_all() {
	int num = m_show_num;
	m_show_num = m_terminal_height - 1;
	m_upd_fun();
	m_show_num = num;
	press_any_key();
}

int Interface::get_number() {
	int res;
	nodelay(stdscr, false);
	echo();
	scanw("%d", &res);
	noecho();
	nodelay(stdscr, true);
	return res;
}

void Interface::send_signal() {
	int pid, code;
	printw("Enter pid: ");
	refresh();
	pid = get_number();
	printw("Enter signal code: ");
	refresh();
	nodelay(stdscr, false);
	code = get_number();
	nodelay(stdscr, true);
	if (kill(pid, code) == -1) {
		printw("Error %d: Something went wrong while sending signal\n", errno);
		refresh();
		press_any_key();
	}
}

void Interface::set_show_num() {
	int num;
	printw("Enter new process show number: \n");
	refresh();
	num = get_number();
	if (num < 1 || num > m_terminal_height - 7) {
		printw("Error :this value cannot be lower than 1 or greater than %d\n", m_terminal_height - 7);
		refresh();
		press_any_key();
	}
	else {
		m_show_num = num;
	}
}

void Interface::print_help_hint() {
	move(m_terminal_height - 2, 0);
	draw_line();
	move(m_terminal_height - 1, 0);
	printw("press h for help");
}

void Interface::print_help() {
	printw("This is help message. Press any key if you want to return to main screen\n");
	draw_line();
	printw("\tq - exit\n");
	printw("\tk - send signal to process\n");
	printw("\tp - change number of shown entities (10 by default)\n");
	printw("\ta - show max amount of entities\n");
	draw_line();
	printw("\tm - set sort by memory consuption\n");
	printw("\tc - set sort by cpu usage\n");
	printw("\tn - set sort by thread number\n");
	draw_line();
	printw("\tt - change output mode to threads\n");
	printw("\ti - change output mode to processes\n");
	refresh();
	press_any_key();
}

void Interface::output_sys_info() {
	system("clear");

	uint32_t uptime = System_monitor::get().get_uptime();
	float memory_utilization = (float)System_monitor::get().get_used_mem() / System_monitor::get().get_sys_mem();
	float cpu_utilization = (float)System_monitor::get().get_load_delta() / System_monitor::get().get_cpu_delta();

	color(memory_utilization);
	printw("%10s %.1f/%.1f MB\n", "Memory:", (float)System_monitor::get().get_used_mem() / 1024,
		(float)System_monitor::get().get_sys_mem() / 1024);

	color(cpu_utilization);
	printw("%7s %.1f %\n", "CPU:", cpu_utilization * 100);

	set_default_color();
	printw("%10s %02d:%02d.%02d\n", "Uptime:", uptime / 3600, (uptime / 60) % 60, uptime % 60);
	draw_line();
}

void Interface::output_processes_info() {
	uint32_t cpu_time = System_monitor::get().get_cpu_delta();

	printw("%*s", pid_len, "PID");
	printw("%*s", name_len + 1, "Name");
	printw("%*s", pid_len + 1, "PPID");
	printw("%*s", vmem_len + 1, "Vmem");
	printw("%*s", cpu_usage_len + 1, "%Proc");
	printw("%*s", state_len + 1, "State");
	printw("%*s", num_threads_len + 1, "Threads");
	printw("%*s", time_len + 1, "Time+");
	printw("%*s", time_len + 1, "STime");
	printw("\n");

	vector<Process> processes = System_monitor::get().get_processes();
	sort(processes.begin(), processes.end(), m_sort_fun);

	int num = m_show_num;
	for (const auto& process : processes) {
		if (num == 0) break;
		if (process.get_state() == State::invalid) continue;

		float memory_utilization = ((float)process.get_mem_size() / 1024) / System_monitor::get().get_sys_mem();
		float processor_utilization = (float)process.get_exec_time_diff() / cpu_time * 10;
		int start_time = process.get_start_time() / sysconf(_SC_CLK_TCK);
		int lifetime = System_monitor::get().get_uptime() - start_time;

		printw("%*d", pid_len, process.get_pid());
		printw("%*s", name_len + 1, process.get_name().c_str());
		printw("%*d", pid_len + 1, process.get_ppid());

		color(memory_utilization);
		printw("%*d", vmem_len + 1, process.get_mem_size() / 1048576);
		set_default_color();

		color(processor_utilization);
		printw("%*.1f", cpu_usage_len + 1, processor_utilization * 100);
		set_default_color();

		printw("%*s", state_len + 1, get_state(process.get_state()).c_str());
		printw("%*d", num_threads_len + 1, process.get_num_threads());
		printw("%*s%02d:%02d", time_len - 4, " ", lifetime / 3600, (lifetime / 60) % 60);
		printw("%*s%02d:%02d", time_len - 4, " ", start_time / 3600, (start_time / 60) % 60);
		printw("\n");
		num--;
	}
}

void Interface::output_threads_info() {
	uint32_t cpu_time = System_monitor::get().get_cpu_delta();

	printw("%*s", pid_len, "TID");
	printw("%*s", pid_len + 1, "PID");
	printw("%*s", priority_len + 1, "Priority");
	printw("%*s", cpu_usage_len + 1, "%Proc");
	printw("%*s", state_len + 1, "State");
	printw("%*s", time_len + 1, "Time+");
	printw("%*s", time_len + 1, "STime");
	printw("\n");

	vector<Thread> threads = System_monitor::get().get_threads();
	sort(threads.begin(), threads.end(), sort_threads);

	int num = m_show_num;
	for (const auto& thread : threads) {
		if (num == 0) break;
		if (thread.get_state() == State::invalid) continue;

		float cpu_utilization = (float)thread.get_exec_time_diff() / cpu_time * 10;
		int start_time = thread.get_start_time() / sysconf(_SC_CLK_TCK);
		int lifetime = System_monitor::get().get_uptime() - start_time;

		printw("%*d", pid_len, thread.get_tid());
		printw("%*d", pid_len + 1, thread.get_pid());
		printw("%*d", priority_len + 1, thread.get_priority());

		color(cpu_utilization);
		printw("%*.1f", cpu_usage_len + 1, cpu_utilization * 100);
		set_default_color();

		printw("%*s", state_len + 1, get_state(thread.get_state()).c_str());
		printw("%*s%02d:%02d", time_len - 4, " ", lifetime / 3600, (lifetime / 60) % 60);
		printw("%*s%02d:%02d", time_len - 4, " ", start_time / 3600, (start_time / 60) % 60);
		printw("\n");
		num--;
	}
}

void Interface::draw_line() {
	int x, y;

	hline(ACS_HLINE, m_terminal_width);

	getyx(stdscr, y, x);
	move(y + 1, 0);
}

void Interface::color(float val) {
	if (val > .7f) {
		attron(COLOR_PAIR(1));
	}
	else if (val > .5f) {
		attron(COLOR_PAIR(2));
	}
	else {
		attron(COLOR_PAIR(3));
	}
}

void Interface::set_default_color() {
	attron(COLOR_PAIR(4));
}