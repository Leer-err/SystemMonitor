#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

enum class State;

class Thread {
public:
	Thread(pid_t pid, pid_t tid, uint32_t processor_time, uint64_t start_time, int priority, State state);
	Thread();

	inline pid_t get_pid() const { return m_pid; }
	inline pid_t get_tid() const { return m_tid; }
	inline pid_t get_priority() const { return m_priority; }
	inline uint32_t get_exec_time_diff() const { return m_current_time - m_last_time; }
	inline uint32_t get_start_time() const { return m_start_time; }
	inline State get_state() const { return m_state; }

	inline void set_priority(int priority) {
		m_priority = priority;
	}
	void set_time(uint32_t time) {
		m_last_time = m_current_time;
		m_current_time = time;
	}
private:
	pid_t m_tid;
	pid_t m_pid;

	int m_priority;

	State m_state;

	uint32_t m_current_time;
	uint32_t m_last_time;

	uint64_t m_start_time;
};