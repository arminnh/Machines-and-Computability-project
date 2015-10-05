/*
 * XMLParse.h
 *
 *  Created on: Oct 9, 2014
 *      Author: n
 */

#ifndef CFGPARSER_H_
#define CFGPARSER_H_

#include <vector>
#include <set>
#include <string>
#include <utility>

#include <UTM/pugixml.hpp>

#include "Symbol.h"

/**
 * @brief CFG parser class.
 *
 * This class parses a XML document and creates a CFG.
 */
class CFGParser {
  public:
    /**
     * @brief Constructor.
     *
     * @param filename The XML file's filename.
     */
    CFGParser(const char* filename);

    /**
     * @brief Default destructor.
     */
    ~CFGParser() =default;

    /**
     * @brief Get the CFG's name.
     *
     * @return The name of the CFG.
     */
    std::string getName();

    /**
     * @brief Get the CFG's variables.
     *
     * @return STL set containing the CFG's variables.
     */
    std::set<Symbol> getVariables();

    /**
     * @brief Get the CFG's terminals.
     *
     * @return STL set containing the CFG's terminals.
     */
    std::set<Symbol> getTerminals();

    /**
     * @brief Get the CFG's production rules.
     *
     * @return STL vector containing the CFG's rules.
     *
     * A rule exists of a pair with a Symbol head and an STL vector of Symbols as body.
     */
    std::vector< std::pair<Symbol, std::vector<Symbol>> > getProductionRules();

    /**
     * @brief Get the CFG's start symbol.
     *
     * @return The CFG's start symbol.
     */
    Symbol getStartSymbol();

  private:
    pugi::xml_document doc;
    pugi::xml_parse_result result;

};

#endif /* CFGPARSER_H_ */

