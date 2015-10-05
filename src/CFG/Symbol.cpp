/*
 * Symbol.cpp
 *
 *  Created on: Nov 20, 2014
 *      Author: Josse
 */

#include "Symbol.h"

Symbol::Symbol() : _value(""), _suffix(0), _terminal(false) { }

Symbol::Symbol(std::string value, int suffix, bool terminal) : _value(value), _suffix(suffix), _terminal(terminal) { }

Symbol::Symbol(std::string value, bool terminal) : _value(value), _suffix(0), _terminal(terminal) { }

Symbol::Symbol(const Symbol& source) : Symbol::Symbol(source._value, source._suffix, source._terminal) { }

Symbol& Symbol::operator=(Symbol other)
{
	swap(*this, other);
	return *this;
}

void swap(Symbol& first, Symbol& second)
{
	using std::swap;

	swap(first._value, second._value);
	swap(first._suffix, second._suffix);
	swap(first._terminal, second._terminal);
}

Symbol Symbol::getSymbolWithSuffix(int suffix) const {
	return Symbol(_value, suffix, _terminal);
}

// returns symbol with same value, but suffix + 1
Symbol Symbol::getSymbolWithNextSuffix() const {
	return getSymbolWithSuffix(_suffix + 1);
}

std::string Symbol::getValue() const {
	return _value;
}

int Symbol::getSuffix() const {
	return _suffix;
}

bool Symbol::isTerminal() const { return _terminal; }

bool Symbol::isVariable() const { return !_terminal; }

bool Symbol::operator==(const Symbol& that) const {
	return _value == that._value && _suffix == that._suffix && _terminal == that._terminal;
}

bool Symbol::operator!=(const Symbol& that) const {
	return !(*this == that);
}

bool Symbol::operator<(const Symbol& that) const {
	// all variables are sorted before any terminals
	if (_terminal != that._terminal) {
		return _terminal < that._terminal;
	}
	// symbols that match on their value are compared against their suffixes
	if (_value == that._value) {
		return _suffix < that._suffix;
	}
	// others are compared against their values
	return _value < that._value;
}

bool Symbol::operator>(const Symbol& that) const {
	return (!(*this < that)) && *this != that;
}

bool Symbol::operator<=(const Symbol& that) const {
	return *this < that || *this == that;
}

bool Symbol::operator>=(const Symbol& that) const {
	return !(*this < that);
}

bool operator==(const std::vector<Symbol>& s1, const std::vector<Symbol>& s2)
{
	if (&s1 == &s2) return true;
	if (s1.size() != s2.size()) return false;
	for (int i = 0; i < s1.size(); ++i) {
		if (s1.at(i) != s2.at(i)) return false;
	}
	return true;
}

std::ostream& operator<<(std::ostream& os, const Symbol& symbol) {
	os << symbol._value;
	if (symbol._suffix and symbol._suffix != 0) {
		os << symbol._suffix;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Symbol>& vector)
{
	os << "(";
	for (std::vector<Symbol>::const_iterator it = vector.begin(); it != vector.end(); std::advance(it, 1)) {
		os << *it;
		if (std::next(it, 1) != vector.end()) os << ", ";
	}
	os << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::set<Symbol>& set)
{
	os << "{";
	for (std::set<Symbol>::const_iterator it = set.begin(); it != set.end(); std::advance(it, 1)) {
		os << *it;
		if (std::next(it, 1) != set.end()) os << ", ";
	}
	os << "}";
	return os;
}
