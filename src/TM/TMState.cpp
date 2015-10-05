#include "TMState.h"

#include <stdexcept>
#include "TuringMachine.h"

namespace tum {

State::State(TuringMachine* TM) : _tm(TM), _isHaltingState(false) {}

TuringMachine* State::turingMachine() const
{
	return _tm;
}

int State::index() const
{
	if (!_tm) {
		throw std::runtime_error("_tm must be set when calling tum::State::index");
	}

	std::vector<State>& states = _tm->_states;

	for (int i = 0; i < states.size(); ++i) {
		if (&states.at(i) == this) {
			return i;
		}
	}

	// if this state is not in the vector of states of the Turing machine to which the state belongs.
	return -1;
}

std::string State::name() const
{
	return _name;
}

void State::setName(std::string name)
{
	_name = name;
}

void State::setHaltingState(bool boolean)
{
	_isHaltingState = boolean;
}

bool State::isHaltingState() const
{
	return _isHaltingState;
}

State& State::setTuringMachine(TuringMachine* TM)
{
  _tm = TM;
  return *this;
}

bool State::operator==(const State& other) const
{
	return _tm == other._tm && index() == other.index();
}

}
