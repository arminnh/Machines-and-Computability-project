/*
*
* File Name :
*
* Creation Date : 21-01-2015
* Last Modified : do 22 jan 12:34:27 2015
* Created By : Bruno De Deken 
*
*/

#include "comparefiles.h"

#include <string>
#include <set>

#include <gtest/gtest.h>

#include <UTM/util.h>
#include <UTM/dot.h>

#include <UTM/finiteautomaton.h>

#include <UTM/CFG.h>
#include <UTM/CFGParser.h>
#include <UTM/CFG2CNF.h>

/** CFG TESTS **/
class CFGTest : public ::testing::Test {
  protected:
    CFG cfg;
    virtual void SetUp() {
      CFGParser parser("./data/cfg1Test.xml");
      cfg = CFG(parser.getVariables(), parser.getTerminals(), parser.getProductionRules(), parser.getStartSymbol(), parser.getName());
    }
};
TEST_F(CFGTest, parser) {
  std::set<Symbol> vars{cfg.getVariables()};
  EXPECT_EQ(vars.size(), 1);
  EXPECT_EQ(vars.begin()->getValue(), "P");

  std::set<Symbol> ters{cfg.getTerminals()};
  EXPECT_EQ(ters.size(), 2);

  auto it = ters.begin();
  EXPECT_EQ(it->getValue(), "0");
  ++it;
  EXPECT_EQ(it->getValue(), "1");

  EXPECT_EQ(cfg.getRules().at(0).first.getValue(), "P");
  EXPECT_EQ(cfg.getRules().at(0).second.size(), 3);
  EXPECT_EQ(cfg.getRules().at(0).second.at(0).getValue(), "0");
  EXPECT_EQ(cfg.getRules().at(0).second.at(1).getValue(), "P");
  EXPECT_EQ(cfg.getRules().at(0).second.at(2).getValue(), "0");

  EXPECT_EQ(cfg.getRules().at(1).first.getValue(), "P");
  EXPECT_EQ(cfg.getRules().at(1).second.size(), 3);
  EXPECT_EQ(cfg.getRules().at(1).second.at(0).getValue(), "1");
  EXPECT_EQ(cfg.getRules().at(1).second.at(1).getValue(), "P");
  EXPECT_EQ(cfg.getRules().at(1).second.at(2).getValue(), "1");
}
TEST_F(CFGTest, checks) {
  EXPECT_TRUE(cfg.isConsistent());
  EXPECT_FALSE(cfg.isDefinedSymbol(Symbol{"X", true}));
  EXPECT_FALSE(cfg.isDefinedSymbol(Symbol{"P", true}));
  EXPECT_TRUE(cfg.isDefinedSymbol(Symbol{"P", false}));
  EXPECT_FALSE(cfg.isDefinedSymbol(Symbol{"0", false}));
  EXPECT_TRUE(cfg.isDefinedSymbol(Symbol{"0", true}));
  EXPECT_TRUE(cfg.isDefinedSymbol(Symbol{"1", true}));

  EXPECT_FALSE(cfg.isDefinedVariable(Symbol{"T", false}));
  EXPECT_FALSE(cfg.isDefinedVariable(Symbol{"P", true}));
  EXPECT_TRUE(cfg.isDefinedVariable(Symbol{"P", false}));

  EXPECT_FALSE(cfg.isDefinedTerminal(Symbol{"P", false}));
  EXPECT_FALSE(cfg.isDefinedTerminal(Symbol{"0", false}));
  EXPECT_TRUE(cfg.isDefinedTerminal(Symbol{"0", true}));

  auto rule = *cfg.getRules().begin();
  EXPECT_TRUE(cfg.isDefinedRule(rule));
  rule.first = Symbol{"X", true};
  EXPECT_FALSE(cfg.isDefinedRule(rule));

  EXPECT_FALSE(cfg.isUsedWithinProduction(Symbol{"P", true}));
  EXPECT_FALSE(cfg.isUsedWithinProduction(Symbol{"0", false}));
  EXPECT_TRUE(cfg.isUsedWithinProduction(Symbol{"P", false}));
  EXPECT_TRUE(cfg.isUsedWithinProduction(Symbol{"1", true}));
}
TEST_F(CFGTest, methodBasics) {
  auto r = *cfg.getRules().begin();
  auto r2 = cfg.useRule(r.second, 1, 1);
  EXPECT_EQ(r.second, (std::vector<Symbol>{Symbol{"0", true}, Symbol{"P", false}, Symbol{"0", true}}));
  EXPECT_EQ(r2, (std::vector<Symbol>{Symbol{"0", true}, Symbol{"0", true}, Symbol{"P", false}, Symbol{"0", true}, Symbol{"0", true}}));

	std::vector< std::pair <int, int > > rulesToUse {
    std::make_pair(1, 1),
    std::make_pair(1, 1),
    std::make_pair(2, 1)};
  EXPECT_EQ(cfg.useRules(rulesToUse), "001P100");
}
TEST_F(CFGTest, automaton) {
  FiniteAutomaton* fa = new FiniteAutomaton(cfg.generatePDA());
  writeDotFile(fa, "result.dot");

  EXPECT_TRUE(compareFiles("result.dot", "./data/expected.dot"));

  delete fa;
}
TEST_F(CFGTest, result){
  CFG2CNF converter;
  cfg = converter(cfg);
  EXPECT_TRUE(cfg.isMember(std::string{"00010001000"}));
  EXPECT_TRUE(cfg.isMember(std::string{"00"}));
  EXPECT_FALSE(cfg.isMember(std::string{""}));  //Empty string never alowed.
  EXPECT_FALSE(cfg.isMember(std::string{"0010"}));
}
TEST_F(CFGTest, xmlWriter) {
  CFGParser parser("./data/cfg1Test.xml");
  CFG cfg1 = CFG(parser.getVariables(), parser.getTerminals(), parser.getProductionRules(), parser.getStartSymbol(), parser.getName());

  cfg1.generateXML("result.xml");

  CFGParser _parser("result.xml");
  CFG cfg2 = CFG(_parser.getVariables(), _parser.getTerminals(), _parser.getProductionRules(), _parser.getStartSymbol(), _parser.getName());

  EXPECT_EQ(cfg2, cfg1);
}

