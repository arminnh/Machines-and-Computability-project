#include "TMSimulatedState.h"

#include "TMState.h"
#include <stdexcept>
#include <cassert>

namespace tum {

SimulatedState::SimulatedState(TuringMachine* tm, std::shared_ptr<SimulatedState> parent, int spawnedFromBranch) : _tm(tm), _spawnedFromBranch(spawnedFromBranch), _parent(parent)
{
	if (!tm) {
		throw std::runtime_error("SimulatedState::SimulatedState: tm cannot be nullptr.");
	}

	for (int i = 0; i < tm->numberOfTapes(); ++i) {
		tapes.push_back({});
		tapes.back().push_back(tm->blankSymbol());

		_tapeheads.push_back(0);
	}

	_currentState = tm->initialState();
}

SimulatedState::SimulatedState(std::shared_ptr<SimulatedState> source, int spawnedFromBranch = -1) : tapes(source->tapes), _tm(source->_tm), _spawnedFromBranch(spawnedFromBranch), _parent(source), _currentState(source->_currentState), _tapeheads(source->_tapeheads)
{

}

void SimulatedState::left(std::size_t tapeIndex)
{
	std::size_t& tapehead = _tapeheads.at(tapeIndex);
	Tape& tape = tapes.at(tapeIndex);

	if (tapehead == 0) {
		tape.push_front(_tm->blankSymbol());
	} else {
		--tapehead;
	}
	return;
}

void SimulatedState::right(std::size_t tapeIndex)
{
	std::size_t& tapehead = _tapeheads.at(tapeIndex);
	Tape& tape = tapes.at(tapeIndex);

	if (tapehead == tape.size() - 1) {
		tape.push_back(_tm->blankSymbol());
	}
	++tapehead;
	return;
}

Character SimulatedState::characterAtTapehead(std::size_t tapeIndex) const
{
	assert(tapeIndex < tapes.size());
	assert(!tapes.at(tapeIndex).empty());
	return tapes.at(tapeIndex).at(_tapeheads.at(tapeIndex));
}

void SimulatedState::writeCharacterAtTapehead(std::size_t tapeIndex, Character c)
{
	tapes.at(tapeIndex).at(_tapeheads.at(tapeIndex)) = c;
	return;
}

int SimulatedState::currentState() const
{
	return _currentState;
}

bool SimulatedState::isHalted() const
{
	if (!_tm) {
		throw std::runtime_error("tum::SimulatedTape::isHalted: _tm is nullptr");
	}
	return _tm->states().at(_currentState).isHaltingState();
}

int SimulatedState::spawnedFromBranch() const
{
	return _spawnedFromBranch;
}

bool SimulatedState::transitionIsCompatible(const Transition& transition) const
{
	if (_currentState != transition.sourceStateIndex()) {
		return false;
	}

	if (tapes.size() != transition.numberOfTapes()) {
		return false;
	}

	for (int i = 0; i < transition.numberOfTapes(); ++i) {
		if (!transition.isCurrentTapeSymbolForTape(i, characterAtTapehead(i)) && (!_tm->anySymbolSymbolSet() || transition.currentTapeSymbolForTape(i) != _tm->anySymbolSymbol())) {
			return false;
		}
	}

	return true;
}

void SimulatedState::performTransition(const Transition& transition)
{
	if (!transitionIsCompatible(transition)) {
		throw std::runtime_error("tum::SimulatedState::performTransition: transition needs to be compatible!");
		return;
	}

	// simple as that ?!

	_currentState = transition.targetStateIndex();

	for (int i = 0; i < transition.numberOfTapes(); ++i) {
		if ((!_tm->sameSymbolSymbolSet()) || transition.nextTapeSymbolForTape(i) != _tm->sameSymbolSymbol())
			writeCharacterAtTapehead(i, transition.nextTapeSymbolForTape(i));
		switch(transition.targetDirectionForTape(i)) {
			case Direction::right:
				right(i);
			break;
			case Direction::left:
				left(i);
			break;
			case Direction::stationary:
			break;
		}
	}

	// too dangerous: what if a tapehead points to a blank that gets removed?
	// also: tapehead values should be adjusted when cleaning the left side of the tape,
	// and so forth...
	// cleanTapes();
}

void SimulatedState::cleanTapes()
{
	for (Tape& tape : tapes) {
		while (tape.back() == _tm->blankSymbol() && tape.size() > 1) {
			tape.pop_back();
		}
		while (tape.front() == _tm->blankSymbol() && tape.size() > 1) {
			tape.pop_front();
		}
	}

	return;
}

std::string SimulatedState::tapeToString(size_t tapeIndex, std::string tapeheadMarker) const
{
	std::string string;
	const Tape& tape = tapes.at(tapeIndex);
	for (int i = 0; i < tape.size(); ++i) {
		if (i == _tapeheads.at(tapeIndex)) {
			string += "(" + tapeheadMarker + ")";
		}
		string += tape.at(i);
	}

	return string;
}

}
