#include "utility.h"


State parse_state(char state) {
	switch (state)
	{
	case 'R':
		return State::running;
	case 'S':
		return State::sleeping;
	case 'X':
		return State::dead;
	case 'Z':
		return State::zombie;
	case 'D':
		return State::waiting;
	case 'T':
	case 't':
		return State::stopped;
	default:
		return State::invalid;
	}
}

string get_state(State state) {
	switch (state)
	{
	case State::running:
		return "running";
	case State::sleeping:
		return "sleeping";
	case State::dead:
		return "dead";
	case State::zombie:
		return "zombie";
	case State::waiting:
		return "waiting";
	case State::stopped:
		return "stopped";
	default:
		return "";
	}
}