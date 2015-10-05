/*
 * CFG.h
 *
 *  Created on: Oct 8, 2014
 *      Author: n
 */

#ifndef CFG_H_
#define CFG_H_

#include <vector>
#include <string>
#include <utility>

#include <UTM/finiteautomaton.h>

#include "Symbol.h"

/**
 * @brief CFG class.
 *
 * This class represents a Context Free Grammar.
 */
class CFG {
  public:
    /**
     * @brief Default constructor.
     */
    CFG() =default;
    /**
     * @brief CFG Constructor.
     *
     * @param var Variables of the CFG.
     * @param ter Terminals of the CFG.
     * @param rules The rules of the CFG.
     * @param start The start symbol of the CFG.
     * @param name The name of the CFG.
     */
    CFG(std::set<Symbol> var, std::set<Symbol> ter, std::vector< std::pair< Symbol, std::vector<Symbol> > > rules, Symbol start, std::string name);

    /**
     * @brief CFG Constructor.
     *
     * This constructor will automatically extract the variables and terminals from the rules.
     *
     * @param rules The rules of the CFG.
     * @param start The start symbol of the CFG.
     * @param name The name of the CFG.
     */
    CFG(std::vector<std::pair<Symbol, std::vector<Symbol>>> rules, Symbol start, std::string name);

    /**
     * @brief Default destructor.
     */
    ~CFG() =default;


    /**
     * @brief Get the variables of the CFG.
     *
     * @return STL vector containing the variables of the CFG.
     */
    std::set<Symbol> getVariables() const;

    /**
     * @brief Get the terminals of the CFG.
     *
     * @return STL vector containing the terminals of the CFG.
     */
    std::set<Symbol> getTerminals() const;

    /**
     * @brief Get the rules of the CFG.
     *
     * @return STL vector of pairs.
     *
     * The first element of each pair is the Head of the rule.
     * The second element of each pair is the Body of the rule.
     */
    std::vector< std::pair< Symbol, std::vector<Symbol> > > getRules() const;

    /**
     * @brief Get the start symbol of the CFG.
     *
     * @return Start symbol.
     */
    Symbol getStartSymbol() const;

    /**
     * @brief Test if the CFG is legitimate.
     *
     * @return True if the CFG is legal.
     */
    bool isConsistent() const;

    /**
     * @brief Test if the CFG is in CNF.
     *
     * @return True if the CFG is in CNF.
     */
    bool isInCNF() const;

    /**
     * @brief Test if the symbol defined in the CFG.
     *
     * @param symbol The symbol.
     *
     * @return True if the symbol is defined.
     */
    bool isDefinedSymbol(const Symbol& symbol) const;

    /**
     * @brief Test if the symbol is defined as a terminal.
     *
     * @param symbol The symbol.
     *
     * @return True symbol is a terminal.
     */
    bool isDefinedTerminal(const Symbol& symbol) const;

    /**
     * @brief Test if the symbol is defined as a variable.
     *
     * @param symbol The symbol.
     *
     * @return True if the symbol is defined as a variable.
     */
    bool isDefinedVariable(const Symbol& symbol) const;

    /**
     * @brief Test if the rules is defined in the CFG.
     *
     * @param rule The rule.
     *
     * @return True if the rule is defined in the CFG.
     */
    bool isDefinedRule(const std::pair<Symbol, std::vector<Symbol> >& rule) const;

    /**
     * @brief Test if a symbol is used in a production.
     *
     * @param symbol The symbol.
     *
     * @return True if the symbol is used.
     */
    bool isUsedWithinProduction(const Symbol&) const;


    /**
     * @brief Use a rule.
     *
     * @param w The rule on which to operate.
     * @param i The index of the rule to be used.
     * @param j The variable in rule @p w on which the rule should be used.
     *
     * @return The new rule where the variabl @p j was substituted by the rule with index @p i.
     */
    std::vector<Symbol> useRule(std::vector<Symbol> w, unsigned int i, unsigned int j) const;

    /**
     * @brief Apply a list of rules to create a string.
     *
     * @param rules The list of rules to be used.
     *
     * @return The resulting string.
     */
    std::string useRules(std::vector < std::pair<int, int> > rules) const;

    /**
     * @brief Generate a PDA that accepts by empty stack.
     *
     * @return A FiniteAutomaton.
     */
    FiniteAutomaton generatePDA() const;

    /**
     * @brief Test if a string is a member of the CFG.
     *
     * @param s The string to be tested.
     *
     * @return True if the string is a member of the CFG.
     */
    bool isMember(std::string s) const; // converts its argument into a vector of Symbols and calls isMember(vector)

    /**
     * @brief Tests if a vector of Symbols is a member of the CFG.
     *
     * This helper method uses the CYK algorithm to test membership.
     *
     * @return True if the vector of symbols are accepted by the CFG.
     */
    bool isMember(const std::vector<Symbol>&) const; // using CYK-algorithm
    // helper function for membership testing function
    /**
     * @brief Get the heads that produce the body.
     *
     * @param body The body for which a head must be found.
     *
     * @return A set of heads.
     */
    std::set<Symbol> headsForProductionBody(const std::vector<Symbol>& body) const;

    /**
     * @brief Overloaded << operator.
     */
    friend std::ostream& operator<< (std::ostream &, CFG const&);

    /**
     * @brief Set the rules for the CFG.
     *
     * @param rules The rules to be set.
     */
    void setRules(std::vector<std::pair<Symbol, std::vector<Symbol> > > rules);

    /**
     * @brief Add a variable to the CFG.
     *
     * @param variable The symbol to be pushed as a variable.
     */
    void pushVariable(Symbol variable);
    
    /**
     * @brief Generate an XML file out of the current CFG.
     *
     * @param name of the new XML file.
     */
	void generateXML(std::string doc);
	
  private:
    std::set<Symbol> _variables, _terminals;
    std::vector< std::pair< Symbol, std::vector<Symbol> > > _rules; //e.g. [(P, [Symbol, Symbol]), (P, [Symbol)]
    Symbol _startSymbol;
    std::string _name;

};

bool operator == (const CFG& cfg1, const CFG& cfg2);

#endif /* CFG_H_ */
