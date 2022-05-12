#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <string>

using std::string;

class Process {
	friend class System_monitor;
public:
	enum class State {
		invalid = 0,
		running,
		sleeping,
		waiting,
		zombie,
		stopped,
		dead
	};

	static State parse_state(char state) {
		switch (state)
		{
		case 'R':
			return Process::State::running;
		case 'S':
			return Process::State::sleeping;
		case 'X':
			return Process::State::dead;
		case 'Z':
			return Process::State::zombie;
		case 'D':
			return Process::State::waiting;
		case 'T':
		case 't':
			return Process::State::stopped;
		default:
			return Process::State::invalid;
		}
	}

	Process(pid_t pid, string name, State state, pid_t ppid, uint32_t num_threads, uint64_t mem_size, uint32_t processor_time, int tty, uint64_t start_time);
	Process();

	inline string get_name() const { return m_name; }
	inline pid_t get_ppid() const { return m_ppid; }
	inline pid_t get_pid() const { return m_pid; }
	inline uint32_t get_mem_size() const { return m_mem; }
	inline uint32_t get_num_threads() const { return m_num_threads; }
	inline State get_state() const { return m_state; }
	inline uint32_t get_exec_time_diff() const { return m_current_time - m_last_time; }
	inline uint32_t get_start_time() const { return m_start_time; }

	void set_name(string name) { m_name = name; }
	void set_pid(pid_t ppid) { m_ppid = ppid; }
	void set_ppid(pid_t ppid) { m_ppid = ppid; }
	void set_mem_size(uint64_t mem_size) { m_mem = mem_size; }
	void set_num_threads(uint64_t num_threads) { m_num_threads = num_threads; }
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

	uint32_t m_num_threads;

	uint32_t m_mem;

	uint32_t m_current_time;
	uint32_t m_last_time;

	uint64_t m_start_time;

	int m_tty;
};