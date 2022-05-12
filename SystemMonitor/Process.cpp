#include "Process.h"

#include<sys/signal.h>

Process::Process(pid_t pid, string name, State state, pid_t ppid, uint32_t num_threads, uint64_t mem_size, uint32_t processor_time, int tty, uint64_t start_time) :
	m_name(name), m_state(state), m_pid(pid), m_ppid(ppid), m_num_threads(num_threads), m_mem(mem_size),
	m_current_time(processor_time), m_last_time(processor_time), m_tty(tty), m_start_time(start_time) {}

Process::Process():
	m_name(""), m_state(Process::State::invalid) {}
