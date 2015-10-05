#include <iostream>

#include <UTM/TuringMachine.h>
#include <UTM/TMState.h>
#include <UTM/TMTransition.h>
#include <UTM/TuringMachineSimulator.h>

#include <UTM/TuringMachineParser.h>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " tm.xml\n";
		return 0;
	}
	
	tum::TuringMachineParser tmParser(argv[1]);

	auto tm = tmParser.getTuringMachine();
	auto simulator = tmParser.getTuringMachineSimulator();

	std::cout << *tm << std::endl;
	std::cout << *simulator << std::endl;

	while (!simulator->isHalted()) {
		simulator->advance();
		std::cout << *simulator << std::endl;
	}

	std::cout << "Turing machine " << (simulator->isDead() ? "is dead" : "halted") << ", stopping now.\n";
}
