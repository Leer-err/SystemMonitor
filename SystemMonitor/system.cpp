#include "system.h"

Thread get_thread_data(pid_t pid, pid_t tid) {
    int priority;
    uint32_t user_time, kernel_time;
    uint64_t start_time;
    char state;
    string dump;

    string path = proc_dir;
    path += "/";
    path += to_string(pid);
    path += task_dir;
    path += "/";
    path += to_string(tid);

    ifstream data_file(path + proc_stat_file);

    if (data_file.is_open()) {
        string file_data;
        getline(data_file, file_data);

        istringstream st(file_data);
        st >> dump >> dump >> state
            >> dump >> dump >> dump >> dump >> dump
            >> dump >> dump >> dump >> dump >> dump >> user_time
            >> kernel_time >> dump >> dump >> priority
            >> dump >> dump >> dump >> start_time;

        return Thread(pid, tid, kernel_time + user_time, start_time, priority, parse_state(state));
    }
    return Thread();
}

Thread update_thread_data(pid_t pid, pid_t tid, Thread thread) {
    int priority;
    uint32_t user_time, kernel_time;
    uint64_t start_time;
    string dump;

    string path = proc_dir;
    path += "/";
    path += to_string(pid);
    path += task_dir;
    path += "/";
    path += to_string(tid);

    ifstream data_file(path + proc_stat_file);

    if (data_file.is_open()) {
        string file_data;
        getline(data_file, file_data);

        istringstream st(file_data);
        st >> dump >> dump >> dump
            >> dump >> dump >> dump >> dump >> dump
            >> dump >> dump >> dump >> dump >> dump >> user_time
            >> kernel_time >> dump >> dump >> priority
            >> dump >> dump >> dump >> start_time;

        thread.set_priority(priority);
        thread.set_time(kernel_time + user_time);
    }
    return thread;
}

Process get_process_data(pid_t pid) {
    pid_t ppid;
    string name;
    char state;
    uint32_t user_time, kernel_time, mem_size;
    uint64_t start_time, dump;
    vector<pid_t> threads;

    string path = proc_dir;
    path += "/";
    path += to_string(pid);

    ifstream data_file(path + proc_stat_file);

    if (data_file.is_open()) {
        string file_data, mem_data;
        getline(data_file, file_data);

        istringstream st(file_data);
        st >> dump >> name >> state
            >> ppid >> dump >> dump >> dump >> dump
            >> dump >> dump >> dump >> dump >> dump >> user_time
            >> kernel_time >> dump >> dump >> dump
            >> dump >> dump >> dump >> start_time;

        threads = get_ids(path + task_dir);
        mem_size = 0;

        ifstream mem_file(path + proc_stat_file);
        if(mem_file.is_open()){
            getline(mem_file, mem_data);

            istringstream mem_st(mem_data);
            mem_st >> mem_size;
        }

        return Process(pid, { name.begin() + 1, name.end() - 1 }, parse_state(state), ppid, mem_size, kernel_time + user_time, start_time, threads);
    }
    return Process();
}

Process update_process_data(pid_t pid, Process process) {
    pid_t ppid;
    string name;
    char state;
    uint32_t user_time, kernel_time, mem_size;
    uint64_t start_time, dump;
    vector<pid_t> threads;

    string path = proc_dir;
    path += "/";
    path += to_string(pid);

    ifstream data_file(path + proc_stat_file);

    if (data_file.is_open()) {
        string file_data, mem_data;
        getline(data_file, file_data);

        istringstream st(file_data);
        st >> dump >> name >> state
            >> ppid >> dump >> dump >> dump >> dump
            >> dump >> dump >> dump >> dump >> dump >> user_time
            >> kernel_time >> dump >> dump >> dump
            >> dump >> dump >> dump >> start_time;

        threads = get_ids(path + task_dir);
        mem_size = 0;

        ifstream mem_file(path + "/statm");
        if(mem_file.is_open()){
            getline(mem_file, mem_data);

            istringstream mem_st(mem_data);
            mem_st >> mem_size >> mem_size;
        }

        process.set_mem_size(mem_size);
        process.set_state(parse_state(state));
        process.set_time(kernel_time + user_time);
        process.set_threads(threads);
    }
    return process;
}

uint32_t get_network_transmitted() {
    ifstream net(network_path);

    int recv = 0;
    int trans = 0;

    if (net.is_open()) {
        string file_data;
        net.ignore(numeric_limits<std::streamsize>::max(), '\n');
        net.ignore(numeric_limits<std::streamsize>::max(), '\n');

        while(true){
            string dump, recieved, transmitted;

            getline(net, file_data);
            if(file_data.size() == 0) break;

            istringstream st(file_data);
            st >> dump >> dump >> dump
            >> dump >> dump >> dump >> dump >> dump
            >> dump >> transmitted;

            trans += std::stoi(transmitted);
        }
    }
    return trans;
}


uint32_t get_network_recieved() {
    ifstream net(network_path);

    int recv = 0;
    int trans = 0;

    if (net.is_open()) {
        string file_data;
        net.ignore(numeric_limits<std::streamsize>::max(), '\n');
        net.ignore(numeric_limits<std::streamsize>::max(), '\n');

        while(true){
            string dump, recieved, transmitted;

            getline(net, file_data);
            if(file_data.size() == 0) break;

            istringstream st(file_data);
            st >> dump >> recieved;

            recv += std::stoi(recieved);
        }
    }
    return recv;
}

vector<pid_t> get_ids(string dir) {
    vector<int> pids;
    DIR* directory = opendir(dir.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
        if (file->d_type == DT_DIR) {
            string filename(file->d_name);
            if (all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

uint32_t get_used_mem() {
    ifstream mem(mem_path);
    uint32_t total, available;

    if (mem.is_open()) {
        while (!mem.eof()) {
            string line, key;
            getline(mem, line);
            istringstream linestream(line);
            linestream >> key;
            if (key == "MemTotal:") {
                linestream >> total;
            }
            else if (key == "MemAvailable:") {
                linestream >> available;
            }
        }
    }
    return total - available;
}

uint32_t get_total_mem() {
    ifstream mem(mem_path);
    uint32_t total = 0;

    if (mem.is_open()) {
        while (!mem.eof()) {
            string line, key;
            getline(mem, line);
            istringstream linestream(line);
            linestream >> key;
            if (key == "MemTotal:") {
                linestream >> total;
                break;
            }
        }
    }
    return total;
}

uint32_t get_cpu_usage() {
    ifstream data_file(stat_path);
    uint32_t res_time = 0;

    if (data_file.is_open()) {
        string file_data, dump;
        getline(data_file, file_data);
        istringstream st(file_data);

        st >> dump;
        for (int i = 0; !st.eof(); i++) {
            uint32_t time;

            st >> time;
            if (i != 3) res_time += time;
        }
    }

    return res_time;
}

uint32_t get_cpu_time() {
    ifstream data_file(stat_path);
    uint32_t res_time = 0;

    if (data_file.is_open()) {
        string file_data, dump;
        getline(data_file, file_data);
        istringstream st(file_data);

        st >> dump;
        while (!st.eof()) {
            uint32_t time;

            st >> time;
            res_time += time;
        }
    }

    return res_time;
}

uint32_t get_uptime() {
    ifstream data_file(uptime_path);
    float res_time = 0;

    if (data_file.is_open()) {
        string file_data, dump;
        getline(data_file, file_data);
        istringstream st(file_data);

        st >> res_time;
    }

    return res_time;
}