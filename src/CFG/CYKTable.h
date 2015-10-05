#ifndef CYK_TABLE_H
#define CYK_TABLE_H

#include <vector>
#include <set>
#include <string>
#include "Symbol.h"

#include <iostream>

/**
 * @brief CYK Table class.
 */
class CYKTable : protected std::vector<std::vector<std::set<Symbol> > >
{
  public:
    /**
     * @brief Constructor.
     *
     * @param str The string for which to create a CYK table.
     */
    CYKTable(std::string str);

    /**
     * @brief Constructor.
     *
     * @param size The size of the CYK table.
     */
    CYKTable(int size);

    /**
     * @brief Test if CYK table contains a symbol.
     *
     * @param symbol Test if the symbol is in the CYK table.
     * @param i The index of the row where the symbol might be.
     * @param j The index of the column where the symbol might be.
     *
     * @return True if the symbol was found in the CYK table.
     */
    bool containsSymbol(Symbol symbol, int i, int j) const;

    /**
     * @brief Test if location (i,j) is legitimate.
     *
     * @param i The index of the row in the CYK table.
     * @param j The index of the column in the CYK table.
     *
     * @return True if the location is legitimate.
     */
    bool exists(int i, int j) const;

    /**
     * @brief Put a set of symbols on a location in the CYK table.
     *
     * @param set The set of symbols to be placed.
     * @param i The index of the row where the symbols must be placed.
     * @param j The index of the column where the symbols must be placed.
     */
    void setSetAt(const std::set<Symbol>& set, int i, int j);

    /**
     * @brief Insert a symbol in the CYK table.
     *
     * @param symbol The symbol to be inserted.
     * @param i The row where the symbol needs to be inserted.
     * @param j The column where the symbol needs to be inserted.
     *
     * @return True if the symbol was succesfully inserted or already present.
     */
    bool insertAt(Symbol symbol, int i, int j);

    /**
     * @brief Get the set of symbols in a location in the CYK table.
     *
     * @param i The index of the row in the CYK table.
     * @param j The index of the column in the CYK table.
     *
     * @return STL set of symbols at that location.
     */
    const std::set<Symbol>& setAt(int i, int j) const;

    /**
     * @brief Overload of << operator.
     */
    friend std::ostream& operator<<(std::ostream& os, const CYKTable& t);

  private:
    void translateSecondIndexToVectorIndex(int i, int& j) const;
};

#endif
