#include "CFG.h"

#include "CYKTable.h"

bool CFG::isMember(std::string str) const
{
	// convert std::string to vector of Symbols
	std::vector<Symbol> s;
	Symbol symbol;
	std::string substring;
	for (int i = 0; i < str.size(); ++i) {
		substring = str.substr(i, 1);
		symbol = Symbol{substring, true};
		s.push_back({symbol});
	}
	return isMember(s);
}

bool CFG::isMember(const std::vector<Symbol>& s) const
{
	assert(isInCNF());
	// a CFG in CNF cannot contain the empty string
	if (s.empty()) return false;

	CYKTable table(s.size());

	// basis: first row:
	// for every terminal a in the string, add the symbols A where a production rule of the form A -> a exists
	for (int i = 0; i < s.size(); ++i) {
		for (auto head : headsForProductionBody({s.at(i)})) {
			table.insertAt(head, i, i);
		}
	}
	//std::cout << "Table after basis:\n" << table;

	std::set<Symbol> set1, set2;
	// heads stores all the symbols to be put in a table position,
	// currentHeads holds the heads for a particular combination of symbols
	std::set<Symbol> heads, currentHeads;
	// induction
	for (int l = 1; l < s.size(); ++l) {
		// l represents the length of the strings at the row being computed, minus 1
		// (so for the induction case we start at strings with length 2)
		for (int i = 0, j = l; j < s.size(); ++i, ++j) {
			// i is the position where the substring starts,
			// j is the position where the substring ends
			assert(table.exists(i, j));
			// compute X(i, j)
			set1.clear();
			set2.clear();
			for (int k = i; k < j; ++k) {
				// determine the two sets to compare to each other
				assert(table.exists(i, k));
				assert(table.exists(k + 1, j));
				set1 = table.setAt(i, k);
				set2 = table.setAt(k + 1, j);
				// consider all possible combinations of elements in each set
				for (const Symbol& symbol1 : set1) {
					for (const Symbol& symbol2 : set2) {
						// take the union of heads with currentHeads, and store it inside heads
						currentHeads = headsForProductionBody({symbol1, symbol2});
						heads.insert(currentHeads.begin(), currentHeads.end());
					}
				}
			}
			table.setSetAt(heads, i, j);
			heads.clear();
		}
	}

	// if the start symbol is in the upper set, then the string is a member of the CFG
	return !(table.insertAt(_startSymbol, 0, s.size() - 1));
}

std::set<Symbol> CFG::headsForProductionBody(const std::vector<Symbol>& body) const
{
	std::set<Symbol> heads;
	for (const auto& rule : _rules) {
		// comparison between two vector<Symbol>s is defined within Symbol.cpp
		if (rule.second == body) {
			heads.insert(rule.first);
		}
	}
	return heads;
}