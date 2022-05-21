#include <sys/signal.h>

#include "System_monitor.h"
#include "Interface.h"

using namespace std;

void resizeHandler(int sig)
{
	Interface::get().set_window_size();
}

int main() {
	signal(SIGWINCH, resizeHandler);

	Interface::get().set_window_size();

	while (true) {
		char a;

		erase();

		a = getch();
		switch (a) {
		case 'q':
			return 0;
		case 'h':
			Interface::get().print_help();
			continue;
		case 'k':
			Interface::get().send_signal();
			continue;
		case 'm':
			Interface::get().set_sort_mem();
			break;
		case 'c':
			Interface::get().set_sort_cpu();
			break;
		case 'n':
			Interface::get().set_sort_thread();
			break;
		case 't':
			Interface::get().set_show_thread();
			break;
		case 'i':
			Interface::get().set_show_proc();
			break;
		case 'p':
			Interface::get().set_show_num();
			continue;
		case 'a':
			Interface::get().show_all();
			continue;
		default:
			break;
		}

		System_monitor::get().update();

		Interface::get().update();
		refresh();

		sleep(1);
	}
	return 0;
}