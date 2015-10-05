/*
 * Symbol.h
 *
 *  Created on: Nov 20, 2014
 *      Author: Josse
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <iostream>
#include <vector>
#include <set>
#include <string>

/**
 * @brief Symbol class.
 */
class Symbol
{
  public:
    /**
     * @brief Default constructor.
     */
    Symbol(); // default constructor should not be used to make a representation of a valid symbol

    /**
     * @brief Constructor.
     *
     * @param value The value of the symbol.
     * @param suffix The suffix of the symbol. It is used to differ between symbols with the same value.
     * @param terminal True if the symbol is a terminal.
     */
    Symbol(std::string value, int suffix, bool terminal);

    /**
     * @brief Constructor.
     *
     * @param value The value of the symbol.
     * @param terminal True if the symbol is a terminal.
     */
    Symbol(std::string value, bool terminal);

    /**
     * @brief Copy constructor.
     *
     * @param symbol The symbols to be copy-constructed.
     */
    Symbol(const Symbol& symbol);

    /**
     * @brief Default destructor.
     */
    ~Symbol() =default;

    /**
     * @brief Overload assignment operator.
     */
    Symbol& operator=(Symbol symbol);

    /**
     * @brief Swap 2 symbols.
     *
     * @param first Symbol 1.
     * @param second Symbol 2.
     */
    friend void swap(Symbol& first, Symbol& other);

    /**
     * @brief Get the symbol with a given suffix.
     *
     * @param suffix The suffix of the symbol that will be created.
     *
     * @return Symbol with the same value and given suffix.
     */
    Symbol getSymbolWithSuffix(int) const;

    /**
     * @brief Get the symbol with the next suffix.
     *
     * @return The symbol with the next suffix.
     */
    Symbol getSymbolWithNextSuffix() const;

    /**
     * @brief Get the value of the symbol.
     *
     * @return The value of the symbol.
     */
    std::string getValue() const;

    /**
     * @brief Get the suffix of the symbol.
     *
     * @return The suffix of the symbol.
     */
    int getSuffix() const;

    /**
     * @param Test if the symbol is a terminal.
     *
     * @return True if the symbol is a terminal.
     */
    bool isTerminal() const;

    /**
     * @brief Test if the symbol is variable.
     *
     * @return True if the symbol is a variable.
     */
    bool isVariable() const;

    bool operator==(const Symbol& that) const;
    bool operator!=(const Symbol& that) const;
    bool operator<(const Symbol& that) const;
    bool operator>(const Symbol& that) const;
    bool operator<=(const Symbol& that) const;
    bool operator>=(const Symbol& that) const;
    friend std::ostream& operator<<(std::ostream&, const Symbol&);

  private:
    std::string _value;
    int _suffix;		// denotes 1, 2, 3, ... in the name of the symbol, for example variable C2: value = C, suffix = 2
    bool _terminal;
};

bool operator==(const std::vector<Symbol>& s1, const std::vector<Symbol>& s2);

std::ostream& operator<<(std::ostream&, const Symbol&);
std::ostream& operator<<(std::ostream&, const std::vector<Symbol>& vector);
std::ostream& operator<<(std::ostream& os, const std::set<Symbol>& set);

#endif /* SYMBOL_H_ */
