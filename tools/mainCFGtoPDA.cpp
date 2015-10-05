#include <iostream>
#include <fstream>

#include <UTM/CFG.h>
#include <UTM/CFGParser.h>
#include <UTM/CFG2CNF.h>
#include <UTM/dot.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " [filename].xml [filename].dot" << std::endl
			<< "where [filename].xml is the input CFG and [filename].dot is the name of the file in which the transition function of the PDA will be written."
			<< std::endl;
    return 0;
	}

	CFGParser parser(argv[1]);

	CFG cfg(parser.getVariables(),
		parser.getTerminals(),
		parser.getProductionRules(),
		parser.getStartSymbol(),
		parser.getName());

	FiniteAutomaton pda = cfg.generatePDA();

	// we don't need to see whatever writeDotFile spits out.
	std::ofstream ourStream(argv[2]);
	if (ourStream.is_open()) {
		writeDotFile(&pda, ourStream);
	} else {
		std::cout << "could not write dot file." << std::endl;
	}

  return 0;
}
