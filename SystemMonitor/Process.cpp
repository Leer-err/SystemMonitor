#include "Process.h"

#include <sys/signal.h>

#include "utility.h"

Process::Process(pid_t pid, string name, State state, pid_t ppid, uint64_t mem_size, uint32_t processor_time, uint64_t start_time, const vector<pid_t>& threads) :
	m_name(name), m_state(state), m_pid(pid), m_ppid(ppid), m_mem(mem_size),
	m_current_time(processor_time), m_last_time(processor_time), m_start_time(start_time), m_threads(threads) {}

Process::Process():
	m_name(""), m_state(State::invalid) {}
