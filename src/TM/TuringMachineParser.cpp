#include "TuringMachineParser.h"

#include <cstdlib>
#include <stdexcept>

namespace tum {

TuringMachineParser::TuringMachineParser(std::string filename) : _tm(nullptr)
{
	pugi::xml_parse_result result = _doc.load_file(filename.c_str());
	if (!result) {
		throw std::runtime_error("Error parsing Turing Machine " + filename + ": " + result.description());
	}
}

std::shared_ptr<TuringMachine> TuringMachineParser::getTuringMachine(bool theJosseWay)
{
	if (_tm) {
		return _tm;
	}

	pugi::xml_node tmNode = _doc.child("turingmachine");
	auto tm = std::make_shared<TuringMachine>();
	_tm = tm;

	// set Turing Machine name
	tm->setName(tmNode.attribute("name").value());

	// set number of tapes
	tm->setNumberOfTapes(atoi(tmNode.attribute("nrtapes").value()));

	// set blank symbol
	tm->setBlankSymbol(tmNode.child("blanksymbol").text().get()[0]);

	// set any symbol symbol, if defined
	std::string anySymbolSymbol = tmNode.child("anysymbol").text().get();
	if (!anySymbolSymbol.empty()) {
		tm->setAnySymbolSymbol(anySymbolSymbol.at(0));
	}

	// set same symbol symbol, if defined
	std::string sameSymbolSymbol = tmNode.child("samesymbol").text().get();
	if (!sameSymbolSymbol.empty()) {
		tm->setSameSymbolSymbol(sameSymbolSymbol.at(0));
	}

	// get states

	pugi::xml_node statesNode = tmNode.child("states");
	std::vector<State> states;

	for (pugi::xml_node stateNode : statesNode.children("st")) {
		State newState(tm.get());
		newState.setName(stateNode.attribute("name").value());

		if (std::string(stateNode.attribute("halting").value()) == "true") {
			newState.setHaltingState(true);
		}
		if (std::string(stateNode.attribute("initial").value()) == "true") {
			tm->setInitialState(states.size());
		}
		states.push_back(newState);
	}

	for (auto& state : states) {
		tm->addState(state);
	}

	// get transitions

	pugi::xml_node transitionsNode = tmNode.child("transitions");
	std::vector<Transition> transitions;

	for (pugi::xml_node transitionNode : transitionsNode.children("tr")) {
		transitions.push_back(theJosseWay ? getTransitionTheJosseWay(states, transitionNode) : getTransition(states, transitionNode));
	}

	for (auto& transition : transitions) {
		tm->addTransition(transition);
	}

	// get subroutines the turing machine depends on

	// subroutines: to be done.
/*
	pugi::xml_text subroutinesFilenameNode = tmNode.child("subroutines");
	std::string subroutineFilename;

	for (pugi::xml_text subroutinesFilenameNode : tmNode.child("subroutines").children("sr").text()) {
		subroutineFilename = subroutinesFilenameNode.get();

		TuringMachineParser parser(subroutineFilename);
		tm->addSubroutine()
	}
*/

	return tm;
}

std::shared_ptr<TuringMachineSimulator> TuringMachineParser::getTuringMachineSimulator()
{
	if (!_tm) {
		getTuringMachine();
		if (_tm) {
			throw std::runtime_error("TuringMachineParser::getTuringMachineSimulator: something went wrong parsing the turing machine.");
		}
		throw std::runtime_error("TuringMachineParser::getTuringMachineSimulator: run getTuringMachine before getTuringMachineSimulator!");
	}

	auto tapes = getInitialTapes();

	auto tmsim = std::make_shared<TuringMachineSimulator>(_tm.get(), *tapes);

	return tmsim;
}

std::shared_ptr<std::vector<Tape>> TuringMachineParser::getInitialTapes()
{
	if (!_tm) {
		throw std::runtime_error("TuringMachineParser::TuringMachineSimulator: run getInitialTapes before getTuringMachineSimulator!");
	}
	// get initial tape content

	pugi::xml_node tmNode = _doc.child("turingmachine");

	auto tapes = std::make_shared<std::vector<Tape>>();

	for (pugi::xml_node tapeNode : tmNode.child("initialtapes").children("tape")) {
		tapes->push_back(getTapeContent(tapeNode));
	}
	return tapes;
}

Transition TuringMachineParser::getTransition(const std::vector<State>& states, pugi::xml_node transitionNode)
{
	Transition transition;
	transition.setTuringMachine(_tm.get());

	pugi::xml_node currentStateNode = transitionNode.child("current");
	pugi::xml_node nextStateNode = transitionNode.child("next");
	transition.setSourceState(indexForStateName(states, currentStateNode.attribute("state").value()));
	transition.setTargetState(indexForStateName(states, nextStateNode.attribute("state").value()));

	pugi::xml_node currentTapeNode = transitionNode.child("current").child("tape");
	int i = 0;
	for (pugi::xml_node nextTapeNode : transitionNode.child("next").children("tape")) {

		Character currentTapeSymbol = currentTapeNode.attribute("symbol").value()[0];
		Character nextTapeSymbol = nextTapeNode.attribute("symbol").value()[0];
		Direction direction = directionForCharacter(nextTapeNode.attribute("direction").value()[0]);

		transition.setTape(i, currentTapeSymbol, nextTapeSymbol, direction);
		currentTapeNode = currentTapeNode.next_sibling("tape");
		++i;
	}

	return transition;
}

Transition TuringMachineParser::getTransitionTheJosseWay(const std::vector<State>& states, pugi::xml_node transitionNode)
{
	Transition transition;
	transition.setTuringMachine(_tm.get());

	pugi::xml_node stateNode = transitionNode.child("state");
	transition.setSourceState(indexForStateName(states, stateNode.attribute("source").value()));
	transition.setTargetState(indexForStateName(states, stateNode.attribute("target").value()));

	int i = 0;
	for (pugi::xml_node tapeNode : transitionNode.children("tape")) {

		Character currentTapeSymbol = tapeNode.attribute("current").value()[0];
		Character nextTapeSymbol = tapeNode.attribute("next").value()[0];
		Direction direction = directionForCharacter(tapeNode.attribute("direction").value()[0]);

		transition.setTape(i, currentTapeSymbol, nextTapeSymbol, direction);
		++i;
	}

	return transition;
}

Tape TuringMachineParser::getTapeContent(pugi::xml_node tapeNode)
{
	Tape tape;
	pugi::xml_text tapeTextNode = tapeNode.text();
	std::string tapeText = tapeTextNode.get();
	for (auto c : tapeText) {
		tape.push_back(c);
	}

	return tape;
}

int TuringMachineParser::indexForStateName(const std::vector<State>& states, std::string stateName) const
{
	for (int i {0}; i < states.size(); ++i) {
		if (states.at(i).name() == stateName) {
			return i;
		}
	}

	throw std::runtime_error("TuringMachineParser::indexForStateName: state name not found (" + stateName + ")");

	return 0;
}

Direction TuringMachineParser::directionForCharacter(char c) const
{
	switch(c) {
		case 'L':
			return Direction::left;
		case 'R':
			return Direction::right;
		case 'S':
			return Direction::stationary;
		default:
			throw std::runtime_error("TuringMachineParser::directionForCharacter: argument must be L, R or S, is: " + std::to_string(c));
	}

	return Direction::stationary;
}

} // namespace tum

