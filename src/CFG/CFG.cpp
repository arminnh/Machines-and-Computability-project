/*
 * CFG.cpp
 *
 *  Created on: Oct 8, 2014
 *      Author: n
 */

#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>

#include <UTM/util.h>
#include "../pugixml-1.4/src/pugixml.hpp"
#include "CFG.h"
#include "CYKTable.h"

CFG::CFG(std::set<Symbol> var, std::set<Symbol> ter,
			std::vector< std::pair< Symbol, std::vector<Symbol> > > rules, Symbol start, std::string name):
				_variables(var), _terminals(ter), _rules(rules), _startSymbol(start), _name(name) { }

// auto-determines all the symbols in the grammar
CFG::CFG(std::vector<std::pair<Symbol, std::vector<Symbol>>> rules, Symbol start, std::string name) :
			_rules(rules), _startSymbol(start), _name(name)
{
	for (const auto& rule : rules) {
		assert(rule.first.isVariable());
		_variables.insert(rule.first);

		for (const Symbol& symbol : rule.second) {
			if (symbol.isTerminal()) {
				_terminals.insert(symbol);
			} else {
				_variables.insert(symbol);
			}
		}
	}
}

std::set<Symbol> CFG::getVariables() const { return _variables; }

std::set<Symbol> CFG::getTerminals() const { return _terminals; }

std::vector< std::pair< Symbol, std::vector<Symbol> > > CFG::getRules() const { return _rules; }

Symbol CFG::getStartSymbol() const { return _startSymbol; }

bool CFG::isConsistent() const {
	// check consistency of production rules
	for (const std::pair<Symbol, std::vector<Symbol>>& rule : _rules) {
		// every head symbol must be defined
		if (!isDefinedVariable(rule.first)) {
			return false;
		}
		for (const Symbol& symbol : rule.second) {
			// every body symbol must be defined
			if (!isDefinedSymbol(symbol)) {
				return false;
			}
		}
	}

	// start symbol must be a defined variable
	if (!isDefinedVariable(_startSymbol)) {
		return false;
	}

	// variables can only contain variables
	for (const Symbol& symbol : _variables) {
		if (symbol.isTerminal()) {
			return false;
		}
	}

	// terminals can only contain terminals
	for (const Symbol& symbol : _terminals) {
		if (symbol.isVariable()) {
			return false;
		}
	}
	return true;
}

bool CFG::isInCNF() const
{
	// do this check within isInCNF or not?
	if (!isConsistent()) return false;

	for (const auto& rule : _rules) {
		if (rule.second.size() == 2) {
			if (rule.second.at(0).isTerminal() || rule.second.at(1).isTerminal()) {
				return false;
			}
		} else if (rule.second.size() == 1) {
			if (rule.second.at(0).isVariable()) {
				return false;
			}
		} else {
			return false;
		}
	}
	return true;
}

bool CFG::isDefinedSymbol(const Symbol& symbol) const {
	return isDefinedVariable(symbol) || isDefinedTerminal(symbol);
}

bool CFG::isDefinedTerminal(const Symbol& symbol) const {
	return _terminals.find(symbol) != _terminals.end();
}

bool CFG::isDefinedVariable(const Symbol& symbol) const {
	return _variables.find(symbol) != _variables.end();
}

bool CFG::isDefinedRule(const std::pair<Symbol, std::vector<Symbol> >& rule) const
{
	bool match;
	for (const auto& cfgRule : _rules)
	{
		if (rule.first != cfgRule.first) continue;
		if (rule.second.size() != cfgRule.second.size()) continue;

		match = true;
		for (int i = 0; i < rule.second.size(); i++) {
			if (rule.second.at(i) != cfgRule.second.at(i)) {
				match = false;
				break;
			}
		}
		if (match) return match;
	}
	return false;
}

bool CFG::isUsedWithinProduction(const Symbol& symbol) const {
	// look for the symbol in productions, if found return true
	for (const auto& rule : _rules) {
		if (symbol == rule.first) {
			return true;
		}
		for (const Symbol& cfgSymbol : rule.second) {
			if (cfgSymbol == symbol) return true;
		}
	}
	return false;
}

//Use rule i on the j-th variable in w, if possible
std::vector<Symbol> CFG::useRule(std::vector<Symbol> w, unsigned int i, unsigned int j) const {
	int counter = 0, position = 0;
	std::vector<Symbol> result;

	// for (auto symbol : w) std::cout << symbol.getValue();
	// std::cout << " -> ";

	//check if the selected rule is valid
	if (i > _rules.size() or i < 1) {
		std::cout << "selected rule does not exist, choose something in range [1, " << _rules.size() << "]" << std::endl;
		return result;
	}

	//check if the selected variable is valid
	for (auto symbol : w) if (symbol.isVariable()) counter++;
	if (j > counter or j < 1) {
		std::cout << "selected variable does not exist, choose something in range [1, " << counter << "]" << std::endl;
		return result;
	}

	//find the position of the j-th variable in w
	counter = 0;
	for (auto it : w) {
		if (counter < j and it.isVariable()) {
			counter++;
			position++;
		} else if (counter < j) {
			position++;
		}
	}

	//if the j-th variable in w does not match the head of the i-th rule, return the (empty) result.
	if (_rules[i-1].first.getValue() != w[position-1].getValue()) {
		std::cout << "wrong input: " << _rules[i-1].first.getValue() << "->";
		for (auto symbol : _rules[i-1].second) std::cout <<  symbol;
		std::cout << " cannot be used on " << w[position-1].getValue() << std::endl;
		return result;
	}

	//replace the variable by its body
	for (int i = 0; i < position-1; i++) result.push_back(w[i]);

	std::vector<Symbol> body_of_rule = _rules[i-1].second;
	for (auto symbol : body_of_rule) result.push_back(symbol);

	for (unsigned int i = position; i < w.size(); i++) result.push_back(w[i]);

	return result;
}

//Generate a string from a list of rules
std::string CFG::useRules(std::vector < std::pair<int, int> > rules) const {
	std::vector<Symbol> w {_startSymbol};	//Vector of symbols represents the string we use the rules on, start with startSymbol
	std::string result;						//String we will return

	//Apply each rule
	for (auto rule : rules) {
		if (!w.empty()) w = useRule(w, rule.first, rule.second);
	}

	//Convert the vector of symbols to a string
	for (auto symbol : w) result += symbol.getValue();

  // std::cout << result << std:endl;
	return result;
}

//Generate a pushdown automaton that accepts by empty stack out of the CFG
FiniteAutomaton CFG::generatePDA() const {
	FiniteAutomaton PDA;
	State state = PDA.addState();

	//for each variable A, take 'epsilon' as input and replace A by B on top of stack for each rule A -> B
	for (auto var : _variables) {
		for (auto rule : _rules) {
			if (var == rule.first) {			//If the selected variable is in the head of the selected rule
				std::vector<int> topOfStack;	//Strings are represented by vectors of integers
				std::vector<int> newTopOfStack;

				for (auto letter : rule.first.getValue()) {
					topOfStack.push_back(letter);
				}
				if (rule.first.getSuffix() != 0)
				{
					topOfStack.push_back(i2c(rule.first.getSuffix()));
				}

				for (auto bodySymbol : rule.second) {
					for (auto letter : bodySymbol.getValue()) {
						newTopOfStack.push_back(letter);
					}
					if (bodySymbol.getSuffix() != 0)
					{
						newTopOfStack.push_back(i2c(bodySymbol.getSuffix()));
					}
				}

				PDA.addArc(state, state).setSymbol(Epsilon).setRule(/*topOfStack*/ rule.first.getValue().at(0) , newTopOfStack);
			}
		}
	}

	//for each terminal a, take 'a' as input and pop the stack
	for (auto ter : _terminals) {
		PDA.addArc(state, state).setSymbol(ter.getValue().at(0)).setRule(ter.getValue().at(0), {Epsilon});
	}

	return PDA;
}

std::ostream& operator<<(std::ostream& os, CFG const& cfg) {
	os << "Context-Free Grammar: " << cfg._name << std::endl;
	os << "\tVariables: ";
	for (std::set<Symbol>::const_iterator it = cfg._variables.begin(); it != cfg._variables.end() ; it++) {
		auto itAdvanced = it;
		std::advance(itAdvanced, 1);
		if (itAdvanced == cfg._variables.end()) os  << *it << std::endl;
		else os << *it << ", ";
	}

	os << "\tTerminals: ";
	for (std::set<Symbol>::const_iterator it = cfg._terminals.begin(); it != cfg._terminals.end() ; it++) {
		auto itAdvanced = it;
		std::advance(itAdvanced, 1);
		if (itAdvanced == cfg._terminals.end()) os  << *it << std::endl;
		else os << *it << ", ";
	}

	os << "\tRules:\n";
	for (std::vector< std::pair<Symbol, std::vector<Symbol>> >::const_iterator it = cfg._rules.begin(); it != cfg._rules.end() ; it++) {
		if (it + 1 == cfg._rules.end()) {
			os << "\t\t" << it->first << " -> ";
			for (auto it2 : it->second) os << it2 << " ";
			os << std::endl;
		} else {
			os << "\t\t" << it->first << " -> ";
			for (auto it2 : it->second) os << it2 << " ";
			os << std::endl;
		}
/*
		os << it->first << " -> ";
		for (auto it2 : it->second) os << it2;

		if (it + 1 != cfg._rules.end()) os << ", ";
		else os << std::endl;
*/
	}

	os << "\tStart Symbol: " << cfg._startSymbol << std::endl;

	return os;
}

// NEW CODE
void CFG::setRules(std::vector<std::pair<Symbol, std::vector<Symbol> > > new_rules) {
	this->_rules = new_rules;
}


void CFG::pushVariable(Symbol variable) {
	this->_variables.insert(variable);
}

void CFG::generateXML(std::string document) {
	std::ofstream myfile;
	myfile.open(document);

	pugi::xml_document doc;
	pugi::xml_node node = doc.append_child("CFG");
	node.append_attribute("name") = this->_name.c_str();

	pugi::xml_node terminals = node.append_child("terminals");
	for (auto terminal : this->_terminals) {
		pugi::xml_node newNode = terminals.append_child("t");
		std::string value = terminal.getValue();
		if (terminal.getSuffix() != 0) {
			value += std::to_string(terminal.getSuffix());
		}
		newNode.append_attribute("value") = value.c_str();
	}

	pugi::xml_node variables = node.append_child("variables");
	for (auto variable : this->_variables) {
		pugi::xml_node newNode = variables.append_child("v");
		std::string value = variable.getValue();
		if (variable.getSuffix() != 0) {
			value += std::to_string(variable.getSuffix());
		}
		newNode.append_attribute("value") = value.c_str();
	}

	pugi::xml_node productionrules = node.append_child("productionrules");
	for (auto production : this->_rules) {
		pugi::xml_node newNode = productionrules.append_child("rule");
		std::string head = production.first.getValue();
		if (production.first.getSuffix() != 0) {
			head += std::to_string(production.first.getSuffix());
		}
		newNode.append_attribute("head") = head.c_str();

		std::string body;
		for (auto symbol : production.second) {
			if (symbol.isTerminal()) body += "'";
			body += symbol.getValue();
			if (symbol.getSuffix() != 0) {
				body += std::to_string(symbol.getSuffix());
			}
			if (symbol.isTerminal()) body += "'";
			body += " ";
		}
    if(body.size() > 0)
      body.pop_back(); //remove the " " at the end
		newNode.append_attribute("body") = body.c_str();
	}

	pugi::xml_node startsymbol = node.append_child("startsymbol");
	startsymbol.append_attribute("value") = this->_startSymbol.getValue().c_str();

	// doc.print(std::cout);
	doc.print(myfile);
  myfile.close();
}

bool operator == (const CFG& cfg1, const CFG& cfg2) {
  if (cfg1.getTerminals() != cfg2.getTerminals()) return false;
  if (cfg1.getRules() != cfg2.getRules()) return false;
  if (cfg1.getVariables() != cfg2.getVariables()) return false;
  if (cfg1.getStartSymbol() != cfg2.getStartSymbol()) return false;
  return true;
}
