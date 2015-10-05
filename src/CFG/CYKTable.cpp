#include "CYKTable.h"

#include <cassert>

CYKTable::CYKTable(int size)
{
	for (int i = 0; i < size; i++) {
		push_back({ std::set<Symbol>() });
		for (int j = 0; j < size - 1 - i; j++) {
			at(i).push_back(std::set<Symbol>());
		}
	}
}

CYKTable::CYKTable(std::string string) :
  CYKTable(string.size())
{}

void CYKTable::translateSecondIndexToVectorIndex(int i, int& j) const
{
	j -= i;
}

bool CYKTable::containsSymbol(Symbol symbol, int i, int j) const
{
	if (!exists(i, j))
    	return false;

  translateSecondIndexToVectorIndex(i, j);
	return setAt(i, j).find(symbol) != setAt(i, j).end();
}

bool CYKTable::exists(int i, int j) const
{

	//std::cout << "CYKTable: original coords " << i << ", " << j << "\t";
	translateSecondIndexToVectorIndex(i, j);
	//std::cout << "translated: " << i << ", " << j << std::endl;
	if (i < size()) {
		if (j < at(i).size()) {
			return true;
		}
	}
	return false;
}

void CYKTable::setSetAt(const std::set<Symbol>& set, int i, int j)
{
	if (!exists(i, j)) return;
	translateSecondIndexToVectorIndex(i, j);

	at(i).at(j) = set;
	return;
}

bool CYKTable::insertAt(Symbol symbol, int i, int j)
{
	if (!exists(i, j)) return false;
	translateSecondIndexToVectorIndex(i, j);

	std::set<Symbol>& set = at(i).at(j);

	return set.insert(symbol).second;
}

const std::set<Symbol>& CYKTable::setAt(int i, int j) const
{
	assert(exists(i, j));
	translateSecondIndexToVectorIndex(i, j);
	return at(i).at(j);
}

std::ostream& operator<<(std::ostream& os, const CYKTable& t)
{
	for (int i = t.size() - 1; i >= 0; i--) {
		for (int j = 0; j < t.at(i).size(); j++) {
			os << t.at(j).at(i) << " ";
		}
		os << std::endl;
	}
	return os;
}

