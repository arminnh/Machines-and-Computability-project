#include "TMTransition.h"

#include <stdexcept>

#include "TuringMachine.h"
#include "TMState.h"

// note: Transition is defined in TuringMachine.h

namespace tum {

Transition::Transition() : _tm(nullptr), _sourceState(0), _targetState(0) {}

Transition::Transition(TuringMachine* tm) : _tm(tm), _sourceState(0), _targetState(0) {}

Transition::Transition(int sourceStateIndex, int targetStateIndex) : _tm(nullptr), _sourceState(sourceStateIndex), _targetState(targetStateIndex) {}

Transition::Transition(TuringMachine* tm, int sourceStateIndex, int targetStateIndex) : _tm(tm), _sourceState(sourceStateIndex), _targetState(targetStateIndex) {}


Transition::Transition(int source, int target, std::vector<std::tuple<Character, Character, Direction>> symbolsAndDirections) :
  _tm(nullptr),
  _sourceState(source),
  _targetState(target),
  _symbolsAndDirectionPerTape(symbolsAndDirections)
  {}

const std::tuple<Character, Character, Direction>* Transition::symbolsAndDirection(int tapeIndex) const
{
	if (!isDefinedTape(tapeIndex)) return nullptr;
	return &_symbolsAndDirectionPerTape.at(tapeIndex);
}

const TuringMachine* Transition::turingMachine() const
{
	return _tm;
}

Transition& Transition::setTuringMachine(TuringMachine* tm)
{
	_tm = tm;
	return *this;
}

Transition& Transition::setSourceState(int stateIndex)
{
	_sourceState = stateIndex;
	return *this;
}

Transition& Transition::setTargetState(int stateIndex)
{
	_targetState = stateIndex;
	return *this;
}

Transition& Transition::stateIndexChangedFromTo(int from, int to)
{
	if (_sourceState == from) {
		_sourceState = to;
	}
	if (_targetState == from) {
		_targetState = to;
	}
	return *this;
}

const State& Transition::sourceState() const
{
	if (!_tm) {
		throw std::runtime_error("_tm should be set when calling TM::State::sourceState");
	}
	return _tm->_states.at(_sourceState);
}

const State& Transition::targetState() const
{
	if (!_tm) {
		throw std::runtime_error("_tm should be set when calling TM::State::targetState");
	}
	return _tm->_states.at(_targetState);
}

int Transition::sourceStateIndex() const
{
	return _sourceState;
}

int Transition::targetStateIndex() const
{
	return _targetState;
}

Transition& Transition::setTape(int tapeIndex, Character currentTapeSymbol, Character newTapeSymbol, Direction direction)
{
	if (isDefinedTape(tapeIndex)) {
		_symbolsAndDirectionPerTape.at(tapeIndex) = std::make_tuple(currentTapeSymbol, newTapeSymbol, direction);
	} else if (tapeIndex == numberOfTapes()) {
		_symbolsAndDirectionPerTape.push_back(std::make_tuple(currentTapeSymbol, newTapeSymbol, direction));
	} else {
		throw std::runtime_error("tum::Transition::setTape: please only set a tape that already exists, or one more than currently exists.");
	}

	return *this;
}

Transition& Transition::setNextTape(Character currentTapeSymbol, Character newTapeSymbol, Direction direction)
{
	return setTape(numberOfTapes(), currentTapeSymbol, newTapeSymbol, direction);
}

int Transition::numberOfTapes() const
{
	return _symbolsAndDirectionPerTape.size();
}

bool Transition::isDefinedTape(int tapeIndex) const
{
  if (tapeIndex < 0)
    return false;
	return tapeIndex < numberOfTapes();
}

bool Transition::isCurrentTapeSymbolForTape(int tapeIndex, Character symbol) const
{
	if (!isDefinedTape(tapeIndex))
		throw std::runtime_error("tum::Transition::isCurrentTapeSymbolForTape: tape with tapeIndex not defined");

	return currentTapeSymbolForTape(tapeIndex) == symbol;
}

bool Transition::isNextTapeSymbolForTape(int tapeIndex, Character symbol) const
{
	if (!isDefinedTape(tapeIndex))
		throw std::runtime_error("tum::Transition::isNextTapeSymbolForTape: tape with tapeIndex not defined");

	return nextTapeSymbolForTape(tapeIndex) == symbol;
}

Character Transition::currentTapeSymbolForTape(int tapeIndex) const
{
	if (!isDefinedTape(tapeIndex))
		throw std::runtime_error("tum::Transition::currentTapeSymbolForTape: tape with tapeIndex not defined");
	
	return std::get<0>(_symbolsAndDirectionPerTape.at(tapeIndex));
}

Character Transition::nextTapeSymbolForTape(int tapeIndex) const
{
	if (!isDefinedTape(tapeIndex))
		throw std::runtime_error("tum::Transition::nextTapeSymbolForTape: tape with tapeIndex not defined");

	return std::get<1>(_symbolsAndDirectionPerTape.at(tapeIndex));
}

bool Transition::isDirectionForTape(int tapeIndex, Direction direction) const
{
	if (!isDefinedTape(tapeIndex))
		throw std::runtime_error("tum::Transition::isDirectionForTape: tape with tapeIndex not defined");

	return targetDirectionForTape(tapeIndex) == direction;
}

Direction Transition::targetDirectionForTape(int tapeIndex) const
{
	if (!isDefinedTape(tapeIndex))
		throw std::runtime_error("tum::Transition::targetDirectionForTape: tape with tapeIndex not defined");

	return std::get<2>(_symbolsAndDirectionPerTape.at(tapeIndex));
}

bool Transition::equalStateTransition(const Transition& other) const
{
	return _sourceState == other._sourceState && _targetState == other._targetState;
}

bool Transition::equalInputRequirements(const Transition& other) const
{
	if (_sourceState != other._sourceState || numberOfTapes() != other.numberOfTapes()) {
		return false;
	}

	Character s1, s2;

	for (int i = 0; i < numberOfTapes(); ++i) {
		s1 = std::get<0>(_symbolsAndDirectionPerTape.at(i));
		s2 = std::get<0>(other._symbolsAndDirectionPerTape.at(i));
		if (s1 != s2)
			return false;
	}

	return true;
}

std::ostream& operator << (std::ostream& os, const State& state)
{
  if (state.name() == "")
    os << state.index();
  else
    os << state.name();

  return os;
}

std::ostream& operator << (std::ostream& os, const Transition& transition)
{
	os << "Transition: "
		<< transition.sourceState() << " -> " << transition.targetState() << std::endl;
  for (std::size_t i =0; i < transition.numberOfTapes(); ++i)
  {
    os << "Tape " << i + 1 << " : " << static_cast<char>(transition.currentTapeSymbolForTape(i)) << " / "  << static_cast<char>(transition.nextTapeSymbolForTape(i)) << " / " << transition.targetDirectionForTape(i) << std::endl;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, Direction d)
{
	switch(d) {
		case Direction::left:
			os << 'L';
		break;
		case Direction::right:
			os << 'R';
		break;
		case Direction::stationary:
			os << 'S';
	}

	return os;
}

bool operator==(const Transition& t1, const Transition& t2) {
  if (t1.numberOfTapes() != t2.numberOfTapes())
    return false;
  if (t1.sourceStateIndex() != t2.sourceStateIndex())
    return false;
  if (t1.targetStateIndex() != t2.targetStateIndex())
    return false;
  return t1.equalStateTransition(t2) and t1.equalInputRequirements(t2);
}

bool operator!=(const Transition& t1, const Transition& t2) {
  return not (t1 == t2);
}


}
