#include <iostream>
#include <fstream>
#include <cassert>

#include <UTM/TuringMachineParser.h>
#include <UTM/TuringMachineEncodingConverter.h>
/*
tum::Tape stringToTape(std::string str)
{
	tum::Tape tape;
	for (char c : str) {
		tape.push_back(c);
	}
	return tape;
}

*/

int main(int argc, char* argv[])
{
	if (argc < 3 || argc > 4) {
		std::cout << "Usage: " << argv[0] << " utm.xml tm.xml [compact] \n";
		return 0;
	}

	// parse the universal turing machine
	tum::TuringMachineParser utmParser(argv[1]);
	std::shared_ptr<tum::TuringMachine> utm = utmParser.getTuringMachine();

	// parse the turing machine to be simulated
	tum::TuringMachineParser tmParser(argv[2]);
	auto tm = tmParser.getTuringMachine();

	// get the initial tape content for the turing machine to be simulated and encode it, along with
	// its transitions and initial state (placed on the "current state" tape)
	tum::TuringMachineConverter converter(*tm);
	auto tapes = converter.encode(tmParser.getInitialTapes()->at(0));

	// create a turing machine simulator for the universal turing machine and the encoded
	// turing machine to be simulated on the universal's tape
	tum::TuringMachineSimulator utmSimulator(utm.get(), tapes);
	// keep up to 1 entry to be able to go back one step when the universal turing machine dies
	utmSimulator.setNrEntriesToKeep(1);

	if (argc == 4 && std::string(argv[3]) == "compact") {
		utmSimulator.outputNextTransitions(false);
	}


	// write the definition of the turing machine to be simulated
	std::ofstream tmlogfile;
	tmlogfile.open("simulatedtmlog.txt");

	tmlogfile << *tm << std::endl;

	tmlogfile.close();


	// write the definition of the universal turing machine that's been parsed
	std::ofstream utmlogfile;
	utmlogfile.open("utmlog.txt");

	utmlogfile << *utm << std::endl;

	utmlogfile.close();


	// write every step of the simulation to a file
	std::ofstream myfile;
	myfile.open ("simulation.txt");
	myfile << utmSimulator << std::endl;

	while (!utmSimulator.isHalted()) {

		utmSimulator.advance();
		myfile << utmSimulator << std::endl;

	}
	myfile << "Turing machine " << (utmSimulator.isDead() ? "is dead" : "halted") << ", stopping now.\n";
	myfile << "Number of steps executed in simulation: " << utmSimulator.nrStepsExecuted() << std::endl;

	// if turing machine halted, it will still contain the last step, however if
	// it died it will contain no steps, so rewinding is required
	if (!utmSimulator.currentStates().size()) {
		utmSimulator.rewind();
	}

	// write the final state of the simulated turing machine
	myfile << "Decoded tape of simulated turing machine at the end of the simulation:" << std::endl;

	assert(utmSimulator.currentStates().size());
	myfile << converter.decodeSimulatedTape(utmSimulator.currentStates().at(0)->tapes.at(0)) << std::endl;;

	const tum::State& lastState = tm->states().at(converter.decodeCurrentStateTape(utmSimulator.currentStates().at(0)->tapes.at(2)));
	myfile << "State: " << lastState.name() << std::endl;
	if (lastState.isHaltingState()) {
		myfile << "This state is a halting state, so the simulated Turing Machine halted." << std::endl;
	} else {
		myfile << "This state is not a halting state, so the simulated Turing Machine died." << std::endl;
	}

	myfile.close();
}
