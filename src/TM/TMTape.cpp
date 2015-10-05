#include "TMTape.h"

namespace tum {

Tape::Tape() {}

Tape::Tape(std::string contents)
{
	for (char c : contents) {
		push_back(c);
	}
}

std::ostream& operator<<(std::ostream& os, const Tape& t)
{
	for (int i = 0; i < t.size(); ++i) {
		os << (char) t.at(i);
	}
	return os;
}
}
