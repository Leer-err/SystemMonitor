#pragma once
#include <sys/types.h>
#include <stdint.h>
#include <string>
#include <vector>

enum class State;

using std::string;
using std::vector;

class Process {
public:
	Process(pid_t pid, string name, State state, pid_t ppid, uint64_t mem_size, uint32_t processor_time, uint64_t start_time, const vector<pid_t>& threads);
	Process();

	vector<pid_t> get_tids() const { return m_threads; }

	inline string get_name() const { return m_name; }
	inline pid_t get_ppid() const { return m_ppid; }
	inline pid_t get_pid() const { return m_pid; }
	inline uint32_t get_mem_size() const { return m_mem; }
	inline uint32_t get_num_threads() const { return m_threads.size(); }
	inline State get_state() const { return m_state; }
	inline uint32_t get_exec_time_diff() const { return m_current_time - m_last_time; }
	inline uint32_t get_start_time() const { return m_start_time; }

	void set_threads(const vector<pid_t>& threads) { m_threads = threads; }
	void set_mem_size(uint64_t mem_size) { m_mem = mem_size; }
	void set_state(State state) { m_state = state; }
	void set_time(uint32_t time) { 
		m_last_time = m_current_time;
		m_current_time = time;
	}
private:
	string m_name;

	State m_state;

	pid_t m_pid;
	pid_t m_ppid;

	uint32_t m_mem;

	uint32_t m_current_time;
	uint32_t m_last_time;

	uint64_t m_start_time;

	vector<pid_t> m_threads;
};