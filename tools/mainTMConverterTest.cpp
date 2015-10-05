#include <iostream>

#include <cassert>

#include <UTM/TuringMachineParser.h>
#include <UTM/TuringMachineEncodingConverter.h>

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 3) {
		std::cout << "Usage: " << argv[0] << " tm.xml\n";
		return 0;
	}

	tum::TuringMachineParser parser(argv[1]);
	std::shared_ptr<tum::TuringMachine> tm = parser.getTuringMachine(argc == 3 ? (std::string(argv[2]) == "josse" ? true : false) : false);

	auto tapesToEncode = parser.getInitialTapes();

	assert(tapesToEncode->size() == 1);

	tum::TuringMachineConverter encoder(*tm);

	std::vector<tum::Tape> encodedTapes = encoder.encode(tapesToEncode->at(0));

	for (const auto& tape : encodedTapes) {
		std::cout << "Tape: " << tape << std::endl;;
	}
}
