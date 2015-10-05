/*
 *
 * File Name :
 *
 * Creation Date : 21-01-2015
 * Last Modified : do 22 jan 12:34:42 2015
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
class CNFTest : public ::testing::Test {
protected:
  CFG cfg;
  CFG2CNF converter;
  virtual void SetUp() {
    CFGParser _parser("./data/cfg2Test.xml");
    cfg = CFG(_parser.getVariables(), _parser.getTerminals(), _parser.getProductionRules(), _parser.getStartSymbol(), _parser.getName());
  }
};

TEST_F(CNFTest, generating) {
  EXPECT_TRUE(cfg.isUsedWithinProduction(Symbol{"G", false}));
  converter.deleteNonGenerating(cfg);
  EXPECT_FALSE(cfg.isUsedWithinProduction(Symbol{"G", false}));
}
TEST_F(CNFTest, epsilon) {
  EXPECT_TRUE(cfg.isDefinedRule(std::make_pair(Symbol{"P", false}, std::vector<Symbol>{})));  // P -> "" exists
  converter.deleteEpsilon(cfg);
  EXPECT_FALSE(cfg.isDefinedRule(std::make_pair(Symbol{"P", false}, std::vector<Symbol>{}))); // P -> "" removed
}
TEST_F(CNFTest, units) {
  EXPECT_TRUE(cfg.isDefinedRule(std::make_pair(Symbol{"R", false}, std::vector<Symbol>{Symbol{"P", false}})));
  converter.deleteUnit(cfg);
  EXPECT_FALSE(cfg.isDefinedRule(std::make_pair(Symbol{"R", false}, std::vector<Symbol>{Symbol{"P", false}})));
}

TEST_F(CNFTest, renameVariables) {
  EXPECT_TRUE(cfg.isDefinedRule(std::make_pair(Symbol{"P", false}, std::vector<Symbol>{Symbol{"1", true}, Symbol{"P", false}, Symbol{"1", true}})));
  converter.shrinkBodysToTwoVariables(cfg);
  EXPECT_FALSE(cfg.isDefinedRule(std::make_pair(Symbol{"P", false}, std::vector<Symbol>{Symbol{"1", true}, Symbol{"P", false}, Symbol{"1", true}})));
}
