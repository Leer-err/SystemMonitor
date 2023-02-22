#include "Thread.h"

#include "utility.h"

Thread::Thread(pid_t pid, pid_t tid, uint32_t processor_time, uint64_t start_time, int priority, State state) :
	m_pid(pid), m_tid(tid), m_current_time(processor_time), m_start_time(start_time), m_priority(priority), m_state(state) {}

Thread::Thread() : m_pid(0), m_tid(0), m_state(State::invalid) {}