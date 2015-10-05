#include "CFG2CNF.h"
#include <set>

void CFG2CNF::deleteNonGenerating(CFG& cfg) {
	// (1) First delete all the rules with variables that have no body
	std::vector<std::pair<Symbol, std::vector<Symbol> > > new_rules = cfg.getRules();
	// get all the variables that have a body
	std::set<Symbol> symbolsWithBody;
	for(auto it : new_rules) {
		symbolsWithBody.insert(it.first);
	}
	// delete all the rules that have a Variable without body
	for(unsigned int i=0; i<new_rules.size(); i++) {
		for(auto it_symbols : new_rules.at(i).second) {
			if(symbolsWithBody.count(it_symbols)==0 && it_symbols.isVariable()) {
				new_rules.erase(new_rules.begin()+i);
				i--;
				break;
			}
		}
	}
	// (2) Second we delete all the rules that are not reachable
	// first collect all the reachable symbols
	std::set<Symbol> reachableSymbols;
	reachableSymbols.insert(cfg.getStartSymbol());
	unsigned int count(0);
	while(count != reachableSymbols.size()) {
		count = reachableSymbols.size();
		for(auto variable : reachableSymbols) {
			for(auto rule : new_rules) {
				if(rule.first == variable) {
					for(auto variable2 : rule.second) {
						reachableSymbols.insert(variable2);
					}
				}
			}
		}
	}
	// delete non reachable rules
	for(unsigned int i=0; i<new_rules.size(); i++) {
		if(reachableSymbols.count(new_rules.at(i).first) == 0) {
			new_rules.erase(new_rules.begin()+i);
		}
	}
	cfg.setRules(new_rules);
}



void CFG2CNF::deleteEpsilon(CFG& cfg) {
	Symbol startsymbol = cfg.getStartSymbol();
	std::vector<std::pair<Symbol,std::vector<Symbol> > > rules = cfg.getRules();
	unsigned int counter(0);
	unsigned int counter2(rules.size());
	// loop through all the rules and delete those with EpsilonBody
	for(unsigned int x=0; x<counter2; x++) { 
		// if rule has Epsilonbody
		if(rules.at(x).second.size() == 0) {
			// delete that rule
			Symbol deletedVariable = rules.at(x).first;
			rules.erase(rules.begin()+counter);
			counter2--;
			counter--;
			x--;
			// replace all the rules that had the deleted rule in them
			// by its subset rules
			for(unsigned int i=0; i<counter2; i++) { 
				for(unsigned int j=0; j<rules.at(i).second.size(); j++) {
					if(rules.at(i).second.at(j) == deletedVariable) { 
						if(rules.at(i).second.size() >1) {
							std::pair<Symbol,std::vector<Symbol> > new_rule = rules.at(i);
							new_rule.second.erase(new_rule.second.begin()+j);
							rules.push_back(new_rule);
							counter2++;
						}
					}
				}
			}
		}
		counter++;
	}
	// now insert all the rules in a set to eliminate doubles
	std::set<std::pair<Symbol,std::vector<Symbol> > > rules_in_set_vorm;
	for(auto iter : rules) {
		rules_in_set_vorm.insert(iter);
	}
	// put the filtered rules back in the vector
	std::vector<std::pair<Symbol,std::vector<Symbol> > > rules_without_duplicates;
	for(auto iter2 : rules_in_set_vorm) {
		rules_without_duplicates.push_back(iter2);
	}
	// assign the rules back to the cfg
	cfg.setRules(rules_without_duplicates);
}

// Deletes unit productions
void CFG2CNF::deleteUnit(CFG& cfg) {
	std::vector<std::pair<Symbol,std::vector<Symbol> > > rules = cfg.getRules();
	// loop through all the rules
	for(unsigned int i=0; i<rules.size(); i++) {
		// if we have only one item in the body && if the item is a variable
		if( (rules.at(i).second.size() == 1) && (rules.at(i).second.at(0).isVariable()) ) {
			Symbol tempVariable = rules.at(i).first;
			Symbol tempVariable2 = rules.at(i).second.at(0);
			rules.erase(rules.begin()+i);
			i--;
			// if we DON'T have a rule of the form A -> A, but rather A->B
			// we can continue and replace the rule by the rules of the variable B
			if(tempVariable != tempVariable2) {
				for(unsigned int x=0; x<rules.size(); x++) { 
					if(rules.at(x).first == tempVariable2) {
						std::pair<Symbol, std::vector<Symbol> > new_rule(tempVariable, rules.at(x).second);
						rules.push_back(new_rule);
					}
				}
			}
		}
	}
	// assign the new rules to cfg
	cfg.setRules(rules);
}

// Changes all the terminal in the rules to Variables
void CFG2CNF::renameTerminals(CFG& cfg) {
	std::vector<std::pair<Symbol,std::vector<Symbol> > > rules = cfg.getRules();
	std::vector<std::pair<Symbol,std::vector<Symbol> > > temp_deleted_rules;
	// loop through all the rules
	// (1) Here we will replace all the rules with terminals by rules of the form A->a that already exist  
	for(unsigned int i=0; i<rules.size(); i++) {
		// if the rule has only one element in its body and it is a terminal
		if(rules.at(i).second.size()==1 && rules.at(i).second.at(0).isTerminal()) {
			Symbol new_symbol = cfg.getStartSymbol().getSymbolWithNextSuffix();
			// find a new Variable name for this terminal
			while(cfg.isDefinedVariable(new_symbol)) {
				new_symbol = new_symbol.getSymbolWithNextSuffix();
			}
			cfg.pushVariable(new_symbol);
			std::vector<Symbol> body_with_terminal;
			body_with_terminal.push_back(rules.at(i).second.at(0));
			std::pair<Symbol, std::vector<Symbol> > new_rule(new_symbol, body_with_terminal);
			std::pair<Symbol, std::vector<Symbol> > temp_deleted_rule = rules.at(i);
			rules.erase(rules.begin()+i);
			rules.insert(rules.begin()+i, new_rule);
			for(unsigned int j=0; j<rules.size(); j++) {
				if(rules.at(j).second.size()>1) {
					for(unsigned int k=0; k<rules.at(j).second.size(); k++) {
						if(rules.at(j).second.at(k) == rules.at(i).first) {
							std::pair<Symbol, std::vector<Symbol> > new_rule2 = rules.at(j);
							new_rule2.second.erase(new_rule2.second.begin()+k);
							new_rule2.second.insert(new_rule2.second.begin()+k, new_symbol);
							rules.push_back(new_rule2);
						}
					}
				}
			}
			temp_deleted_rules.push_back(temp_deleted_rule);
		}
	}
	rules.insert(rules.end(), temp_deleted_rules.begin(), temp_deleted_rules.end());
	// (2) Here we will replace all the rules with terminals by rules of the form A->a that DON'T exist,
	// but we will make those rules
	for(unsigned int i=0; i<rules.size(); i++) {
		if(rules.at(i).second.size() >1) {
			// check each item of the body if it is a terminal
			for(unsigned int j=0; j<rules.at(i).second.size(); j++) {
				// check if item is terminal
				if(rules.at(i).second.at(j).isTerminal()) {
					bool foundReplacement(false);
					// search if there already exist an rule of the form A->a to replace the terminal
					for(auto it : rules) {
						if(it.second.size() == 1 && it.second.at(0) == rules.at(i).second.at(j) && !foundReplacement) {
							foundReplacement = true;
							rules.at(i).second.erase(rules.at(i).second.begin()+j);
							rules.at(i).second.insert(rules.at(i).second.begin()+j, it.first);
						}
					}
					// if we didn't found one we simply make a new one
					if(!foundReplacement) {
						Symbol new_symbol = cfg.getStartSymbol().getSymbolWithNextSuffix();
						// search for a new variable name
						while(cfg.isDefinedVariable(new_symbol)) {
							new_symbol = new_symbol.getSymbolWithNextSuffix();
						}
						cfg.pushVariable(new_symbol);
						std::vector<Symbol> body_with_terminal;
						body_with_terminal.push_back(rules.at(i).second.at(j));
						std::pair<Symbol, std::vector<Symbol> > new_rule(new_symbol, body_with_terminal);
						rules.push_back(new_rule);
						rules.at(i).second.erase(rules.at(i).second.begin()+j);
						rules.at(i).second.insert(rules.at(i).second.begin()+j, new_symbol);
					}
				}
			}
		}
	}
	cfg.setRules(rules);
}

// Shrinks all the bodies of the rule to two variables
void CFG2CNF::shrinkBodysToTwoVariables(CFG& cfg) {
	std::vector<std::pair<Symbol,std::vector<Symbol> > > rules = cfg.getRules();
	// loop through all the rules
	for(unsigned int i=0; i<rules.size(); i++) {
		// if the rule has more than 2 variables
		if(rules.at(i).second.size() > 2) {
			Symbol new_symbol = rules.at(i).second.at(1).getSymbolWithNextSuffix();
			// we now have to find a new variable name to replace the rule from A->BCDE to A->BF
			while(cfg.isDefinedVariable(new_symbol)) {
				new_symbol = new_symbol.getSymbolWithNextSuffix();
			}
			cfg.pushVariable(new_symbol);
			std::vector<Symbol> new_body(rules.at(i).second.begin()+1, rules.at(i).second.end());
			std::pair<Symbol, std::vector<Symbol>> new_rule(new_symbol, new_body);
			rules.at(i).second.erase(rules.at(i).second.begin()+1, rules.at(i).second.end());
			rules.at(i).second.push_back(new_symbol);
			rules.push_back(new_rule);
		}
	}
	// filter the vector by pushing content to set
	std::set<std::pair<Symbol,std::vector<Symbol> > > rules_in_set_vorm;
	for(auto iter : rules) {
		rules_in_set_vorm.insert(iter);
	}
	// return all the rules to the vector
	std::vector<std::pair<Symbol,std::vector<Symbol> > > rules_without_duplicates;
	for(auto iter2 : rules_in_set_vorm) {
		rules_without_duplicates.push_back(iter2);
	}

	cfg.setRules(rules_without_duplicates);
}

// Functor operator; mainfunction
CFG CFG2CNF::operator() (CFG cfg) {
	if (cfg.isInCNF()) return cfg;
	deleteEpsilon(cfg);
	deleteUnit(cfg);
	deleteNonGenerating(cfg);
	renameTerminals(cfg);
	shrinkBodysToTwoVariables(cfg);
	return cfg;
}

