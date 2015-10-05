#include <iostream>

#include <UTM/CFGParser.h>
#include <UTM/CFG.h>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "specify filename please" << std::endl;
		return 1;
	}

	CFGParser parser(argv[1]);
	CFG cfg(parser.getVariables(), parser.getTerminals(), parser.getProductionRules(), parser.getStartSymbol(), parser.getName());
	std::cout << cfg;
}
