#include <sys/signal.h>
#include <thread>

#include "System_monitor.h"
#include "Interface.h"

using namespace std;

int main() {

	bool is_running = true;

	System_monitor::get().update();
	Interface::get().update();

	std::thread update_thread([&is_running](){
		while(is_running){
			System_monitor::get().update();
			sleep(1);
		}
	});

	slint::Timer update_timer(std::chrono::seconds(1), [=] () {
		Interface::get().update();
	});

	Interface::get().run();

	is_running = false;
	update_thread.join();

	return 0;
}