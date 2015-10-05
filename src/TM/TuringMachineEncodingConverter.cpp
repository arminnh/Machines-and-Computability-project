#include "TuringMachineEncodingConverter.h"

#include <cassert>
#include <stdexcept>
#include <string>

namespace tum {

TuringMachineConverter::TuringMachineConverter(const TuringMachine& tm) : _tm(&tm) {}


std::vector<Tape> TuringMachineConverter::encode(const Tape& tapeToEncode)
{
	std::vector<Tape> tapes;

	_symbolDecodings.clear();
	_stateDecodings.clear();

	std::set<Character> symbols = _tm->allSymbols();
	for (auto symbol : tapeToEncode) {
		symbols.insert(symbol);
	}

	std::map<Character, std::string> symbolEncodings;

	if (symbols.size() == 1) {
		std::string encodedSymbol;
		encodedSymbol = ".";
		symbolEncodings[*symbols.begin()] = encodedSymbol;
		_symbolDecodings[encodedSymbol] = *symbols.begin();
	} else
	// compute encoded versions of all the symbols
	for (std::set<Character>::iterator i = symbols.begin(); i != symbols.end(); ++i) {
		std::string encodedSymbol;
		int j = 0;

		int length = std::distance(symbols.begin(), i);

		for (; j < length; ++j) {
			encodedSymbol += "1";
		}
		for (; j < symbols.size() - 1; ++j) {
			encodedSymbol += ".";
		}
		symbolEncodings[*i] = encodedSymbol;
		_symbolDecodings[encodedSymbol] = *i;
	}

	/*
	std::cout << "printing symbolEncodings" << std::endl;
	for (auto s : symbolEncodings) {
		std::cout << s.first << " " << s.second << std::endl;
	}
	std::cout << "finished" << std::endl;
	*/

	// compute encoded versions of all the states
	std::vector<State> states = _tm->states();
	std::map<int, std::string> stateEncodings;

	for (int i = 0; i < states.size(); ++i) {
		std::string encodedState;
		int j = 0;
		for (; j < i; ++j) {
			encodedState += "1";
		}
		for (; j < states.size() - 1; ++j) {
			encodedState += ".";
		}
		stateEncodings[i] = encodedState;
		_stateDecodings[encodedState] = i;
	}

	/*
	std::cout << "printing stateEncodings" << std::endl;
	for (auto s : stateEncodings) {
		std::cout << s.first << " " << s.second << std::endl;
	}
	std::cout << "finished" << std::endl;
	*/

	Tape simulatedTapeTape;

	// construct the simulated tape tape
	for (const Character& symbol : tapeToEncode) {
		assert(symbolEncodings.count(symbol));
		for (char c : symbolEncodings.find(symbol)->second) {
			simulatedTapeTape.push_back(c);
		}
		if (&symbol != &tapeToEncode.back()) {
			simulatedTapeTape.push_back('0');
		}
	}

	Tape transitionsTape;

	// construct the transitions tape
	// first add blank symbol to transitions tape

	for (char c : symbolEncodings[_tm->blankSymbol()]) {
		transitionsTape.push_back(c);
	}

	transitionsTape.push_back('B');

	auto transitions = _tm->transitions();
	for (const Transition& transition : transitions) {
		// current state
		// assert(stateEncodings.count(transition.sourceStateIndex()));
		for (char c : stateEncodings[transition.sourceStateIndex()]) {
			transitionsTape.push_back(c);
		}
		transitionsTape.push_back(';');

		// current symbol
		for (char c : symbolEncodings[transition.currentTapeSymbolForTape(0)]) {
			transitionsTape.push_back(c);
		}
		transitionsTape.push_back(';');

		// new state
		for (char c : stateEncodings[transition.targetStateIndex()]) {
			transitionsTape.push_back(c);
		}
		transitionsTape.push_back(';');

		// new symbol
		for (char c : symbolEncodings[transition.nextTapeSymbolForTape(0)]) {
			transitionsTape.push_back(c);
		}
		transitionsTape.push_back(';');

		// direction
		int k = 0;
		switch(transition.targetDirectionForTape(0)) {
			case Direction::right:
				k = 1;
			break;
			case Direction::left:
				k = 2;
			break;
			case Direction::stationary:
			default:
				k = 3;
		}
		for (int i = 0; i < k; ++i) {
			transitionsTape.push_back('1');
		}

		if (&transition != &transitions.back()) {
			transitionsTape.push_back('0');
		}
	}

	// construct current state tape (initially contains the initial state)

	Tape currentStateTape;

	assert(stateEncodings.count(_tm->initialState()));

	for (char c : stateEncodings[_tm->initialState()]) {
		currentStateTape.push_back(c);
	}

	Tape scratchTape;
	scratchTape.push_back('B');

	tapes.push_back(simulatedTapeTape);
	tapes.push_back(transitionsTape);
	tapes.push_back(currentStateTape);
	tapes.push_back(scratchTape);

	_encoded = true;

	return tapes;
}

Tape TuringMachineConverter::decodeSimulatedTape(const Tape& tapeToDecode) const
{
	if (!_encoded) {
		throw std::runtime_error("TuringMachineConverter::decodeSimulatedTape: encode before decoding!");
	}

	Tape decodedTape;
	int currentIndex = 0;
	int startOfSymbolIndex;

	std::string encodedSymbol;

	while (currentIndex < tapeToDecode.size() && tapeToDecode.at(currentIndex) == 'B') {
		++currentIndex;
	}

	while (currentIndex < tapeToDecode.size() && tapeToDecode.at(currentIndex) != 'B') {
		startOfSymbolIndex = currentIndex;

		while (currentIndex < tapeToDecode.size()
			&& tapeToDecode.at(currentIndex) != '0'
			&& tapeToDecode.at(currentIndex) != 'B') {
			++currentIndex;
		}

		encodedSymbol.clear();

		for (int i {startOfSymbolIndex}; i < currentIndex; ++i) {
			encodedSymbol += tapeToDecode.at(i);
		}

		if (!_symbolDecodings.count(encodedSymbol)) {
			throw std::runtime_error("TuringMachineConverter::decodeSimulatedTape: no symbol decoding found for " + encodedSymbol);
		}

		decodedTape.push_back(_symbolDecodings.find(encodedSymbol)->second);

		++currentIndex;
	}

	return decodedTape;
}

int TuringMachineConverter::decodeCurrentStateTape(const Tape& tapeToDecode) const
{
	if (!_encoded) {
		throw std::runtime_error("TuringMachineConverter::decodeCurrentStateTape: encode before decoding!");
	}
	int currentIndex = 0;

	std::string encodedState;

	while (currentIndex < tapeToDecode.size() && tapeToDecode.at(currentIndex) == 'B') {
		++currentIndex;
	}

	while (currentIndex < tapeToDecode.size() && tapeToDecode.at(currentIndex) != 'B') {
		encodedState += tapeToDecode.at(currentIndex);
		++currentIndex;
	}

	if (!_stateDecodings.count(encodedState)) {
		throw std::runtime_error("TuringMachineConverter::decodeCurrentStateTape: no symbol decoding found for " + encodedState);
	}

	return _stateDecodings.find(encodedState)->second;
}

} // namespace tum

