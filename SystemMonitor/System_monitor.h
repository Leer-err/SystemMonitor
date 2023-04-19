#pragma once
#include <map>
#include <vector>
#include <mutex>

#include "Process.h"
#include "Thread.h"

using std::map;
using std::vector;
using std::mutex;
using std::lock_guard;

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
    inline uint32_t get_used_mem() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_used_mem; 
    }
    /*!
	\return installed RAM size (in bytes)
	*/
    inline uint32_t get_sys_mem() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_sys_mem; 
    }

    /*!
	\return CPU time in user and kernel modes (in system ticks)
	*/
    inline uint32_t get_load_delta() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_current_load_time - m_last_load_time; 
    }
    /*!
	\return CPU time since last update (in system ticks)
	*/
    inline uint32_t get_cpu_delta() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_current_time - m_last_time; 
    }

    /*!
	\return Bytes transmitted to network since last update
	*/
    inline uint32_t get_net_transmit_usage() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_net_trans_curr - m_net_trans_prev; 
    }
    /*!
	\return Bytes recieved from network since last update
	*/
    inline uint32_t get_net_recv_usage() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_net_recv_curr - m_net_recv_prev; 
    }

    /*!
	\return system uptime (in system ticks)
	*/
    inline uint32_t get_uptime() { 
        lock_guard<mutex> lock = lock_guard<mutex>(m_update_mutex);
        return m_uptime; 
    }

    vector<Process> get_processes();
    vector<Thread> get_threads();
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

    uint32_t m_net_recv_prev;
    uint32_t m_net_recv_curr;
    uint32_t m_net_trans_prev;
    uint32_t m_net_trans_curr;

    mutex m_update_mutex;
};
