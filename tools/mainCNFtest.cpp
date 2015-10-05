#include <iostream>

#include <UTM/CFG.h>
#include <UTM/CFGParser.h>
#include <UTM/CFG2CNF.h>

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3) {
		std::cout << "Usage: " << argv[0] << " [filename].xml" << std::endl
			<< "where [filename].xml is the input CFG" << std::endl;
	}

	CFGParser parser(argv[1]);
	CFG cfg(parser.getVariables(),
		parser.getTerminals(),
		parser.getProductionRules(),
		parser.getStartSymbol(),
		parser.getName());

	CFG2CNF converter;
	CFG cfgInCnf = converter(cfg);

	std::cout << "Original:" << std::endl
		<< cfg << std::endl
		<< "Consistent? " << cfg.isConsistent() << std::endl
		<< "New:" << std::endl
		<< cfgInCnf << std::endl
		<< "Consistent? " << cfgInCnf.isConsistent()
		<< std::endl;

  
	if (argc == 3)
		cfgInCnf.generateXML(argv[2]);

	return 0;
}
