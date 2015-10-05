/*
 * CFGParser.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: n
 */

#include "CFGParser.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <sstream>
#include "Symbol.h"

CFGParser::CFGParser(const char* source) {
	result = doc.load_file(source);
	if(!result) {
		std::string filename = source;
		throw std::runtime_error("Error parsing CFG " + filename + ": " + result.description());
	}
}

std::string CFGParser::getName() {
	std::string name;
	pugi::xml_attribute nameAttribute = doc.child("CFG").attribute("name");

	if (nameAttribute) {
		name = nameAttribute.value();
	}

	return name;
}

std::set<Symbol> CFGParser::getVariables() {
	std::set<Symbol> variables;
	pugi::xml_node variablesNode = doc.child("CFG").child("variables");
	pugi::xml_attribute value;

	for (pugi::xml_node vNode : variablesNode.children("v")) {
		if ((value = vNode.attribute("value"))) {
			variables.insert({value.value(), false});
		}
	}

	return variables;
}

std::set<Symbol> CFGParser::getTerminals() {
	std::set<Symbol> terminals;
	pugi::xml_node terminalsNode = doc.child("CFG").child("terminals");
	pugi::xml_attribute value;
	pugi::xml_attribute lowerCaseLetters, upperCaseLetters, numbers;

	std::stringstream sstr;

	if ((lowerCaseLetters = terminalsNode.attribute("lowercaseletters"))) {
		if (std::string(lowerCaseLetters.value()) == "true") {
			for (char c = 'a'; c <= 'z'; ++c) {
				sstr.str(""); sstr.clear(); sstr << c;
				terminals.insert(Symbol {sstr.str(), true});
			}
		}
	}

	if ((upperCaseLetters = terminalsNode.attribute("uppercaseletters"))) {
		if (std::string(upperCaseLetters.value()) == "true") {
			for (char c = 'A'; c <= 'Z'; ++c) {
				sstr.str(""); sstr.clear(); sstr << c;
				terminals.insert(Symbol {sstr.str(), true});
			}
		}
	}

	if ((numbers = terminalsNode.attribute("numbers"))) {
		if (std::string(numbers.value()) == "true") {
			for (char c = '0'; c <= '9'; ++c) {
				sstr.str(""); sstr.clear(); sstr << c;
				terminals.insert(Symbol {sstr.str(), true});
			}
		}
	}

	for (pugi::xml_node tNode : terminalsNode.children("t")) {
		if ((value = tNode.attribute("value"))) {
			terminals.insert(Symbol {value.value(), true});
		}
	}

	return terminals;
}

std::vector< std::pair<Symbol, std::vector<Symbol>> > CFGParser::getProductionRules()  {
	std::vector< std::pair<Symbol, std::vector<Symbol> > > rules;
	std::pair<Symbol, std::vector<Symbol>> rule;
	std::stringstream sstr;
	std::string currentToken;

	pugi::xml_node rulesNode = doc.child("CFG").child("productionrules");
	pugi::xml_attribute headAttribute, bodyAttribute;

	for (pugi::xml_node ruleNode : rulesNode.children("rule")) {
		headAttribute = ruleNode.attribute("head");
		bodyAttribute = ruleNode.attribute("body");
		rule.second.clear();
		if (!(headAttribute && bodyAttribute)) continue;
		// head of the rule
		rule.first = {headAttribute.value(), false};

		// special case when body is the empty string, because stringstreams don't want to play along nicely
		// (currentToken wouldn't get updated since sstr was eof (even though the flag says is wasn't))
		// causing the body to consist of the last read variable of the previously read body
		if (std::string(bodyAttribute.value()).empty()) {
			rules.push_back(rule);
			continue;
		}

		// body of the rule
		sstr.clear();
		sstr.str("");
		sstr.str(bodyAttribute.value());
		while (!sstr.eof()) {
			sstr >> currentToken;
			if (currentToken.size() == 1 && currentToken.front() == '\'' && !sstr.eof()) {
				// in case space is a terminal
				sstr >> currentToken;
				if (currentToken.size() == 1 && currentToken.at(0) == '\'') {
					// if the next token is ' too, then it was indeed a space
					rule.second.push_back({" ", true});
				} else { throw std::runtime_error("expected \'"); }
			} else if (currentToken.front() == '\'' && currentToken.back() == '\'') {
				rule.second.push_back({currentToken.substr(1, currentToken.size() - 2), true});
			} else {
				rule.second.push_back({currentToken, false});
			}
		}
		rules.push_back(rule);
	}

	return rules;
}

Symbol CFGParser::getStartSymbol() {
	std::string str;
	str = doc.child("CFG").child("startsymbol").attribute("value").value();

	return Symbol {str, false};
}

//split a string by a delimeter and put it in a vector
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

