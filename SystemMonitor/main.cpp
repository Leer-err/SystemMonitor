#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <sys/signal.h>

#include "System_monitor.h"
#include "Output_manager.h"

using namespace std;

atomic<bool> is_running = true;
mutex output_mutex;

void input() {
	while (is_running) {
		char a;
		cin >> a;
		switch (a) {
		case 'q':
			is_running = false;
			break;
		case 'k':
			output_mutex.lock();
			{
				system("clear");
				int pid, code;
				cout << "Enter pid: ";
				cin >> pid;
				cout << "Enter signal code: ";
				cin >> code;
				if (kill(pid, code) == -1) {
					cout << "Error " << errno << ": Something went wrong while sending signal" << endl;
					cin >> a;
				}
			}
			output_mutex.unlock();
			break;
		case 'm':
			output_mutex.lock();
			Output_manager::get().set_sort_mem();
			output_mutex.unlock();
			break;
		case 'c':
			output_mutex.lock();
			Output_manager::get().set_sort_cpu();
			output_mutex.unlock();
			break;
		case 'n':
			output_mutex.lock();
			Output_manager::get().set_sort_thread();
			output_mutex.unlock();
			break;
		case 'p':
			{
				output_mutex.lock();
				system("clear");
				int num;
				cout << "Enter new process show number: " << endl;
				cin >> num;
				Output_manager::get().set_show_num(num);
				output_mutex.unlock(); 
			}
			break;
		default:
			break;
		}
	}
}

int main() {
	thread th(input);
	th.detach();
	
	while (is_running) {
		output_mutex.lock();
		system("clear");

		System_monitor::get().update();
		Output_manager::get().output_sys_info();
		Output_manager::get().output_processes_info();
		output_mutex.unlock();

		sleep(1);
	}
	return 0;
}