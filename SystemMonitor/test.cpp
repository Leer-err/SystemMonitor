#include "Interface.h"
#include "system.h"

int main(int argc, char **argv){
    if(string(argv[1]) == "pid"){
        return Interface::get().check_pid(65535) == false ? 0 : 1;
    }else if(string(argv[1]) == "signal"){
        return Interface::get().send_signal(1, 9);
    }else if(string(argv[1]) == "poll"){
        return (get_cpu_time() != 0 && get_total_mem() != 0) ? 0 : 1;
    }
}