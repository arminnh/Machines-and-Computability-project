/*
*
* File Name :
*
* Creation Date : 21-01-2015
* Last Modified : wo 21 jan 15:16:38 2015
* Created By : Bruno De Deken 
*
*/

#include "comparefiles.h"

#include <gtest/gtest.h>

#include <UTM/util.h>
#include <UTM/dot.h>

#include <UTM/finiteautomaton.h>

/** PDA TESTS **/
class PDATest: public ::testing::Test {
  protected:
    FiniteAutomaton pda;
};
TEST_F(PDATest, Construction) {
  EXPECT_EQ(0, pda.numStates());
  EXPECT_EQ(0, pda.numArcs());
}
TEST_F(PDATest, States1) {
  State state1 = pda.addState();

  EXPECT_EQ(1, pda.numStates());
  EXPECT_EQ(&pda, state1.automaton());
  EXPECT_EQ(0, state1.index());
  EXPECT_EQ(0, state1.arcs().size());
}
TEST_F(PDATest, Arcs1) {
  State state1 = pda.addState();
  Arc arc1 = pda.addArc(state1, state1);

  EXPECT_EQ(1, pda.numArcs());
  EXPECT_EQ(&pda, arc1.automaton());
  EXPECT_EQ(0, arc1.index());
  EXPECT_EQ(0, arc1.source().index());
  EXPECT_EQ(0, arc1.target().index());
}
TEST_F(PDATest, States2) {
  pda.addState();
  State state2 = pda.addState();

  EXPECT_EQ(&pda, state2.automaton());
  EXPECT_EQ(1, state2.index());
  EXPECT_EQ(0, state2.arcs().size());

  EXPECT_EQ(0, pda.numArcs());
}
TEST_F(PDATest, Arcs2) {
  State state1 = pda.addState();
  State state2 = pda.addState();
  pda.addArc(state1, state1);
  Arc arc2 = pda.addArc(state1, state2);

  EXPECT_EQ(2, pda.numArcs());
  EXPECT_EQ(&pda, arc2.automaton());
  EXPECT_EQ(1, arc2.index());
  EXPECT_EQ(0, arc2.source().index());
  EXPECT_EQ(1, arc2.target().index());
}
TEST_F(PDATest, int2char) {
  EXPECT_EQ('1', i2c(1));
  EXPECT_EQ('9', i2c(9));
}
TEST_F(PDATest, stack) {
  State s1 = pda.addState();
  State s2 = pda.addState();
  pda.addArc(s1, s2).setSymbol(i2c(3)).setRule(StartOfStack, {'A',StartOfStack});

  EXPECT_EQ('3', pda.arc(0).symbol());
}

