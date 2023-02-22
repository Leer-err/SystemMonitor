#include <sys/signal.h>

#include "System_monitor.h"
#include "Interface.h"

using namespace std;

int main() {
	System_monitor::get().update();
	Interface::get().update();
	slint::Timer update_timer(std::chrono::seconds(1), [=] () {
		System_monitor::get().update();
		Interface::get().update();
	});

	Interface::get().run();

	
	return 0;
}