#include "TuringMachine.h"
#include <UTM/util.h>

#include <stdexcept>

#include "TMTransition.h"
#include "TMState.h"

namespace tum {

TuringMachine::TuringMachine() {}

Transition TuringMachine::addTransition(Transition t)
{
	int sourceStateIndex = t.sourceStateIndex();
	int targetStateIndex = t.targetStateIndex();

  if(t.turingMachine() == nullptr) {
    t.setTuringMachine(this);
  }
	_transitions[sourceStateIndex][targetStateIndex].push_back(t);

  bool sourcePresent = false;
  bool targetPresent = false;
  for (const auto& state : _states)
  {
    if (state.index() == sourceStateIndex) sourcePresent = true;
    if (state.index() == targetStateIndex) targetPresent = true;
  }

  if (!sourcePresent)
    _states.push_back(State(this));
  if (!targetPresent)
    _states.push_back(State(this));

  return t;
}

bool TuringMachine::transitionExists(const Transition& t) const
{
	if (_transitions.find(t.sourceStateIndex()) == _transitions.end())
		return false;
	const auto& innerMap = _transitions.find(t.sourceStateIndex())->second;
	if (innerMap.find(t.targetStateIndex()) == innerMap.end())
		return false;

	for (const Transition& transition : innerMap.find(t.targetStateIndex())->second)
	{
		if (transition.sourceStateIndex() == t.sourceStateIndex()) {

		}
	}
	return true;
}

std::vector<Transition> TuringMachine::transitions() const
{
	std::vector<Transition> transitions;
	for (const auto& e1 : _transitions) {
		const auto& e2 = e1.second;
		for (const auto& e3 : e2) {
			const auto& e4 = e3.second;
			for (const auto& e5 : e4)
				transitions.push_back(e5);
		}
	}
	return transitions;
}

std::vector<Transition> TuringMachine::transitions(int sourceIndex, int targetIndex) const
{
	assert(_transitions.count(sourceIndex));
	assert(_transitions.find(sourceIndex)->second.count(targetIndex));
  return (_transitions.find(sourceIndex)->second).find(targetIndex)->second;
}

std::map<int, std::vector<Transition>> TuringMachine::transitions(int sourceIndex) const
{
	if (_transitions.count(sourceIndex))
		return _transitions.find(sourceIndex)->second;

	return std::map<int, std::vector<Transition>>();
}

int TuringMachine::initialState() const
{
	return _initialState;
}

void TuringMachine::setInitialState(int i)
{
	_initialState = i;
}

void TuringMachine::addState(const State& state)
{
	_states.push_back(state);
	_states.back().setTuringMachine(this);
}

void TuringMachine::addNumberOfStates(unsigned number)
{
	for (unsigned i = 0; i < number; ++i) {
		_states.push_back(State(this));
	}
}

std::vector<int> TuringMachine::getTargets(int sourceIndex) const
{
  std::map<int, std::vector<Transition>> targets{_transitions.find(sourceIndex)->second};

  std::vector<int> result{};
  for(auto index : targets)
    result.push_back(index.first);
  return result;
}

void TuringMachine::addAccepting(int index)
{
  bool valid = false;
  for(const auto& state : _states) {
    if (index == state.index())
      valid = true;
  }
  if(!valid)
    throw std::runtime_error("Added accepting state (index: " + std::to_string(index) + ") is not a valid state!\n Thrown by TuringMachine::addAccepting(int index)");
  for (auto& state : _acceptingStates)
    if (state == index)
      return;
  _acceptingStates.push_back(index);
}

bool TuringMachine::isAcceptingState(int stateIndex) const
{
	for (int i = 0; i < _acceptingStates.size(); ++i) {
		if (stateIndex == _acceptingStates.at(i)) {
			return true;
		}
	}
	return false;
}

void TuringMachine::setName(std::string name)
{
	_name = name;
}

std::string TuringMachine::name() const
{
	return _name;
}

void TuringMachine::setNumberOfTapes(int number)
{
	_numberOfTapes = number;
}

int TuringMachine::numberOfTapes() const
{
	return _numberOfTapes;
}

int TuringMachine::numberOfSymbols() const
{
	return allSymbols().size();
}

std::set<Character> TuringMachine::allSymbols() const
{
	std::set<Character> allSymbols;

	std::vector<Transition> transitions = TuringMachine::transitions();

	for (const auto& transition : transitions) {
		for (int i = 0; i < transition.numberOfTapes(); ++i) {
			allSymbols.insert(transition.currentTapeSymbolForTape(i));
			allSymbols.insert(transition.nextTapeSymbolForTape(i));
		}
	}
	return allSymbols;
}

// important note about Transition arguments in addSubroutine: sourceState of jumpTo transiton points to a state of this;
// targetState of jumpTo points to a state of tm;
// sourceState of returnFrom points to a state of tm;
// targetState of returnFrom points to a state of this.

void TuringMachine::setBlankSymbol(Character symbol)
{
	_blankSymbol = symbol;
}

Character TuringMachine::blankSymbol() const
{
	return _blankSymbol;
}

void TuringMachine::setAnySymbolSymbol(Character symbol)
{
	_anySymbolSymbol = symbol;
	_anySymbolSymbolSet = true;
}

Character TuringMachine::anySymbolSymbol() const
{
	return _anySymbolSymbol;
}

bool TuringMachine::anySymbolSymbolSet() const
{
	return _anySymbolSymbolSet;
}

void TuringMachine::setSameSymbolSymbol(Character symbol)
{
	_sameSymbolSymbol = symbol;
	_sameSymbolSymbolSet = true;
}

Character TuringMachine::sameSymbolSymbol() const
{
	return _sameSymbolSymbol;
}

bool TuringMachine::sameSymbolSymbolSet() const
{
	return _sameSymbolSymbolSet;
}

std::vector<State> TuringMachine::states() const
{
  return _states;
}


bool TuringMachine::operator==(const TuringMachine& tm) const
{
  if (numberOfTapes() != tm.numberOfTapes())
    return false;
  if (states() != tm.states())
    return false;
  return true;
}

std::ostream& operator << (std::ostream& os, const TuringMachine& tm)
{
	os << "Turing machine";
	if (!tm.name().empty()) {
		os << " " << tm.name();
	}

	os << ":\nStates:\n";
  for (size_t i = 0; i < tm._states.size(); ++i) {
  	if (tm._states.at(i).name().empty()) {
  		os << i;
  	} else {
  		os << tm._states.at(i).name();
  	}
  	if (i == tm._initialState) {
  		os << " (initial)";
  	}
  	if (tm._states.at(i).isHaltingState()) {
  		os << " (halting)";
  	}
  	os << "\t";
  }
  os << std::endl << "Transitions:\n";

  for (const auto& transitionEl : tm._transitions) {
  	const auto& transitionEl2 = transitionEl.second;
  	for (const auto& transitionEl3 : transitionEl2) {
  		const auto& transitionEl4 = transitionEl3.second;
  		for (const auto& transition : transitionEl4) {
  			os << transition;
  		}
  		
  	}
  }

  return os;
}

} // namespace tum
