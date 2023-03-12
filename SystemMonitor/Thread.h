#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

enum class State;

/*!
	\brief Class representing single thread
*/
class Thread {
public:
	/*!
		Constructor
		\param pid process id
		\param tid thread id
		\param processor_time used CPU time
		\param start_time process start time
		\param state current thread state
	*/
	Thread(pid_t pid, pid_t tid, uint32_t processor_time, uint64_t start_time, int priority, State state);
	Thread();

	/*!
		\return parent process id
	*/
	inline pid_t get_pid() const { return m_pid; }
	inline pid_t get_tid() const { return m_tid; }
	inline pid_t get_priority() const { return m_priority; }
	/*!
		\return CPU time used by thread since last update (in system ticks)
	*/
	inline uint32_t get_exec_time_diff() const { return m_current_time - m_last_time; }
	/*!
		\return time since thread spawned (in system ticks)
	*/
	inline uint32_t get_start_time() const { return m_start_time; }
	inline State get_state() const { return m_state; }

	inline void set_priority(int priority) {
		m_priority = priority;
	}
	/*!
		\return update CPU time (in system ticks)
	*/
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