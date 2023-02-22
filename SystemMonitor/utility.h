#pragma once
#include <string>

using std::string;

enum class State {
	invalid = 0,
	running,
	sleeping,
	waiting,
	zombie,
	stopped,
	dead
};

State parse_state(char state);

string get_state(State state);