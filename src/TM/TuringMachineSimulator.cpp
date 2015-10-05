#include "TuringMachineSimulator.h"

#include "TMState.h"
#include <stdexcept>

namespace tum {

TuringMachineSimulator::TuringMachineSimulator(TuringMachine* tm, const std::vector<Tape>& tapes) : _tm(tm)
{
	_currentStates.push_back(std::make_shared<SimulatedState>(tm));
	if (tapes.size()) {
		if (tapes.size() != tm->numberOfTapes()) {
			throw std::runtime_error("TuringMachineSimulator::TuringMachineSimulator: number of tapes of turing machine should be equal to number of tapes specified in vector");
		} else {
			_currentStates.back()->tapes = tapes;
		}
	}
}

const TuringMachine& TuringMachineSimulator::turingMachine() const
{
	if (!_tm)
		throw std::runtime_error("TuringMachineSimulator::turingMachine: _tm cannot be nullptr!");

	return *_tm;
}

bool TuringMachineSimulator::advance()
{
	_pastStates.push_back(_currentStates);
/*
	if (_futureStates.size()) {
		_currentStates = _futureStates.back();
		_futureStates.pop_back();

		removeOldEntries();
		return true;
	}
*/
	if (isHalted()) {
		return false;
	}

	if (!_currentStates.size()) {
		return false;
	}

	_pastStates.push_back(_currentStates);
	_currentStates = computeNextSimulatedStates();
	++_nrStepsExecuted;

	for (auto state : _currentStates) {
		// std::cout << "and again, still works: " << state->currentState() << std::endl;
	}

	removeOldEntries();


	return true;
}

bool TuringMachineSimulator::rewind()
{
	if (!_pastStates.size()){
		return false;
	}

	_futureStates.push_back(_currentStates);
	_currentStates = _pastStates.back();
	_pastStates.pop_back();

	--_nrStepsExecuted;

	return true;
}

bool TuringMachineSimulator::canRewind() const
{
	return _pastStates.size();
}

int TuringMachineSimulator::nrStepsExecuted() const
{
	return _nrStepsExecuted;
}

void TuringMachineSimulator::setNrEntriesToKeep(size_t nrEntriesToKeep)
{
	_nrHistoryEntriesToKeep = nrEntriesToKeep;
}

void TuringMachineSimulator::outputNextTransitions(bool boolean)
{
	_outputNextTransitions = boolean;
}

void TuringMachineSimulator::removeOldEntries()
{
	while (_pastStates.size() > _nrHistoryEntriesToKeep) {
		_pastStates.erase(_pastStates.begin());
	}
}

bool TuringMachineSimulator::isHalted() const
{
	if (isDead()) {
		return true;
	}

	for (const auto& state : _currentStates) {
		if (!state) {
			throw std::runtime_error("state is nullptr!");
		}
		if (!state->isHalted()) {
			return false;
		}
	}

	return true;
}

bool TuringMachineSimulator::isDead() const
{
	return !_currentStates.size();
}

std::vector<std::shared_ptr<const SimulatedState>> TuringMachineSimulator::currentStates() const
{
	std::vector<std::shared_ptr<const SimulatedState>> simulatedStates;
	for (auto state : _currentStates) {
		simulatedStates.push_back(state);
	}

	return simulatedStates;
}

std::vector<std::shared_ptr<SimulatedState>> TuringMachineSimulator::computeNextSimulatedStates()
{
	std::vector<std::shared_ptr<SimulatedState>> newSimulatedStates;

	std::vector<Transition> nextMovesForCurrentSimulatedState;

	// for every currently simuated state, find its next moves
	for (size_t i = 0; i < _currentStates.size(); ++i) {
		nextMovesForCurrentSimulatedState = nextMoves(*_currentStates.at(i));

		// for every move found, perform it on a copy of the currently simulated state,
		// and add it to the simulated states of the next step
		for (int j = 0; j < nextMovesForCurrentSimulatedState.size(); ++j) {
			auto newSimulatedState = std::make_shared<SimulatedState>(_currentStates.at(i), i);

			newSimulatedState->performTransition(nextMovesForCurrentSimulatedState.at(j));
			newSimulatedStates.push_back(newSimulatedState);
		}
	}

	return newSimulatedStates;
}

std::vector<Transition> TuringMachineSimulator::nextMoves(const SimulatedState& state) const
{
	int currentState = state.currentState();

	std::map<int, std::vector<Transition>> candidateMoves = _tm->transitions(currentState);

	std::vector<Transition> actualMoves;

	// for every transition starting in the current state, test for compatibility
	// with the current situation (current state + tape contents) and add it to
	// the next moves vector
	for (auto it = candidateMoves.begin(); it != candidateMoves.end(); ++it) {
		std::vector<Transition>& transitions = it->second;
		for (int j = 0; j < transitions.size(); ++j) {
			Transition& transition = transitions.at(j);

			if (state.transitionIsCompatible(transition)) {
				actualMoves.push_back(transition);
			}
		}
	}

	return actualMoves;
}

std::ostream& operator<<(std::ostream& os, const TuringMachineSimulator& tms)
{

	if (tms._outputNextTransitions) {
		os << "Current State:\n";
	}

	if (tms.isDead()) {
		os << "Dead\n";
	} else if (tms.isHalted()) {
		os << "Accepted\n";
	}

	for (int i = 0; i < tms._currentStates.size(); ++i) {
		const auto& currentState = *tms._currentStates.at(i);

		// currentState.currentState()... seems legit
		std::string stateName = tms._tm->states().at(currentState.currentState()).name();
		if (stateName.empty()) {
			stateName = std::to_string(currentState.currentState());
		}

		os << "Branch " << i + 1 << " of " << tms._currentStates.size();
		if (currentState.spawnedFromBranch() != -1) {
			os << ", spawned from branch " << currentState.spawnedFromBranch() + 1;
		}
		if (tms._tm->numberOfTapes() != 1) {
			os << ", state: " << stateName << std::endl;
		} else {
			os << ": ";
		}

		for (int j = 0; j < currentState.tapes.size(); ++j) {
			if (tms._tm->numberOfTapes() != 1) {
				os << "Tape " << j + 1 << ": " << currentState.tapeToString(j) << std::endl;;
			} else {
				os << currentState.tapeToString(j, stateName) << std::endl;
			}
			
		}

		if (tms._outputNextTransitions) {
			auto nextMoves = tms.nextMoves(*tms._currentStates.at(i));
			if (!nextMoves.empty() && !tms.isHalted()) {
				if (nextMoves.size() != 1) {
					os << "Will perform these transitions to advance to the next state:" << std::endl;
				} else {
					os << "Will perform this transition to advance to the next state:" << std::endl;
				}

				for (const auto& move : nextMoves) {
					os << move;
					if (&move != &nextMoves.back()) {
						os << ";" << std::endl;
					}
				}
			} else if (!tms.isHalted()) {
				os << "This branch will die in the next step." << std::endl;
			}
		}
	}

	return os;
}

} // namespace tum

