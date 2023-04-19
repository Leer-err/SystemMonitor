#include "Interface.h"

#include <sys/signal.h>
#include <memory>

#include "System_monitor.h"

#include "ui.h"

slint::ComponentHandle<HelloWorld> window = HelloWorld::create();

Interface::Interface() {
	auto time_stamps = std::make_shared<slint::VectorModel<TimeStamp>>();
	TimeStamp init = {};
	init.cpu = 0;
	init.mem = 0;
	for(int i = 0;i < 5;i++){
		time_stamps->push_back(init);
	}
	window->set_graph(time_stamps);

	window->on_set_out_threads([=] () {
		m_upd_fun = bind(&Interface::output_threads_info, this);
	});
	window->on_set_out_processes([=] () {
		m_upd_fun = bind(&Interface::output_processes_info, this);
	});
	window->on_set_sort_cpu([=] () {
		m_sort_fun = sort_by_cpu_usage;
	});
	window->on_set_sort_mem([=] () {
		m_sort_fun = sort_by_mem_usage;
	});
	window->on_check_pid([=] (int pid) {
		return check_pid(pid);
	});
	window->on_send_signal([=] (int pid, int signal) {
		return send_signal(pid, signal);
	});

	m_upd_fun = std::bind(&Interface::output_processes_info, this);
}

void Interface::run() {
    window->run();
}

void Interface::update() {
	output_sys_info();
	m_upd_fun();
}

bool Interface::send_signal(int pid, int signal) {
	return kill(pid, signal) == 0;
}

bool Interface::check_pid(int pid) {
	vector<Process> processes = System_monitor::get().get_processes();
	return any_of(processes.begin(), processes.end(), [=](Process& a) {return a.get_pid() == pid;});
}

void Interface::output_sys_info() {
	uint32_t uptime = System_monitor::get().get_uptime();
	float memory_utilization = (float)System_monitor::get().get_used_mem() / System_monitor::get().get_sys_mem();
 	float cpu_utilization = (float)System_monitor::get().get_load_delta() / System_monitor::get().get_cpu_delta();
	int trans = System_monitor::get().get_net_transmit_usage();
	int recv = System_monitor::get().get_net_recv_usage();
	int network_utilization = recv + trans;

	auto graph = window->get_graph();
	for(int i = 0;i < 4;i++){
		graph->set_row_data(i, graph->row_data(i+1).value());
	}

	TimeStamp time_stamp = {};
	time_stamp.cpu = cpu_utilization * 100;
	time_stamp.mem = memory_utilization * 100;
	time_stamp.net = network_utilization / 1024 / 1024;
	
	graph->set_row_data(4, time_stamp);
	window->set_graph(graph);

	window->set_cpu_used(cpu_utilization * 100);
	window->set_memory_installed(System_monitor::get().get_sys_mem() / 1024);
	window->set_memory_used(System_monitor::get().get_used_mem() / 1024);
	window->set_memory_utilization(memory_utilization * 100);
	window->set_transmission_speed(trans / 1024);
	window->set_reception_speed(recv / 1024 );
}

void Interface::output_processes_info() {
 	uint32_t cpu_time = System_monitor::get().get_cpu_delta();
	uint32_t uptime = System_monitor::get().get_uptime();
	auto processes = System_monitor::get().get_processes();

	sort(processes.begin(), processes.end(), m_sort_fun);

	auto procData = vector<ProcessData>();
	procData.reserve(processes.size());
	int i =0;
	for(const auto& process: processes){
		if (process.get_state() == State::invalid) continue;

		float cpu_utilization = (float)process.get_exec_time_diff() / cpu_time * 10;
		int start_time = process.get_start_time() / sysconf(_SC_CLK_TCK);
		int lifetime = uptime - start_time;
		ProcessData data = {};
		data.name = std::string_view(process.get_name());
		data.cpu_per = cpu_utilization * 12.5f;
		data.mem = process.get_mem_size() * sysconf(_SC_PAGE_SIZE) /1024 /1024;
		data.id = process.get_pid();
		data.parent_id = process.get_ppid();
		data.up_hours = lifetime / 3600;
		data.up_minutes = lifetime / 60 % 60;
		data.up_seconds = lifetime % 60;

		procData.push_back(data);
		if(++i ==1000) break;
	}

	std::shared_ptr<slint::VectorModel<ProcessData>> processData = std::make_shared<slint::VectorModel<ProcessData>>(procData);

	window->set_processes(processData);
}

void Interface::output_threads_info() {
	const uint32_t tick_time = sysconf(_SC_CLK_TCK);
	uint32_t cpu_time = System_monitor::get().get_cpu_delta();
	uint32_t uptime = System_monitor::get().get_uptime();

	auto threadData = std::make_shared<slint::VectorModel<ThreadData>>();

	vector<Thread> threads = System_monitor::get().get_threads();
	sort(threads.begin(), threads.end(), sort_threads);
 	int i = 0;
	for(const auto& thread: threads){

		if (thread.get_state() == State::invalid) continue;

		float cpu_utilization = (float)thread.get_exec_time_diff() / cpu_time * 10;
		int start_time = thread.get_start_time() / tick_time;
		int lifetime = uptime - start_time;
		ThreadData data = {};
		data.cpu_per = cpu_utilization * 12.5f;
		data.tid = thread.get_tid();
		data.pid = thread.get_pid();
		data.up_hours = lifetime / 3600;
		data.up_minutes = lifetime / 60 % 60;
		data.up_seconds = lifetime % 60;

		threadData->push_back(data);
		if(++i ==1000) break;
	}

	window->set_threads(threadData);
}