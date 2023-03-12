#pragma once
#include <map>
#include <vector>

#include "Process.h"
#include "Thread.h"

using std::map;
using std::vector;

/*!
	\brief Class for interacting with system
*/
class System_monitor {
public:
    /*!
	\return class instance
	*/
    static System_monitor& get() {
        static System_monitor instance;
        return instance;
    }

    /*!
	Updates data about system
	*/
    void update();

    /*!
	\return used virtual memory (in bytes)
	*/
    inline uint32_t get_used_mem() const { return m_used_mem; }
    /*!
	\return installed RAM size (in bytes)
	*/
    inline uint32_t get_sys_mem() const { return m_sys_mem; }

    /*!
	\return CPU time in user and kernel modes (in system ticks)
	*/
    inline uint32_t get_load_delta() const { return m_current_load_time - m_last_load_time; }
    /*!
	\return CPU time since last update (in system ticks)
	*/
    inline uint32_t get_cpu_delta() const { return m_current_time - m_last_time; }

    /*!
	\return system uptime (in system ticks)
	*/
    inline uint32_t get_uptime() const { return m_uptime; }

    vector<Process> get_processes() const;
    vector<Thread> get_threads() const;
private:
    System_monitor() = default;
    ~System_monitor() = default;

    System_monitor(const System_monitor&) = delete;
    System_monitor& operator=(const System_monitor&) = delete;

    map<pid_t, Process> m_processes;
    map<pid_t, Thread> m_threads;

    uint32_t m_current_time;
    uint32_t m_last_time;

    uint32_t m_current_load_time;
    uint32_t m_last_load_time;

    uint32_t m_uptime;

    uint32_t m_sys_mem;
    uint32_t m_used_mem;
};
