/*
*
* File Name : main.cpp
*
* Creation Date : 20-10-2014
* Last Modified : do 22 jan 13:39:05 2015
* Created By : Bruno De Deken
*
*/

#include "comparefiles.h"

#include <string>
#include <set>

#include <gtest/gtest.h>

#include <UTM/util.h>
#include <UTM/dot.h>

// #include <UTM/finiteautomaton.h>
//
// #include <UTM/cfg.h>
// #include <UTM/cfgparser.h>
// #include <UTM/cfg2cnf.h>

#include <UTM/TuringMachine.h>
#include <UTM/TMState.h>
#include <UTM/TMTransition.h>
#include <UTM/TuringMachineSimulator.h>
#include <UTM/TuringMachineParser.h>


using tum::TuringMachine;
using tum::SimulatedState;
using tum::Transition;
using tum::Direction;

using Rule = std::tuple<int, int, Direction>;
/** TM TESTS **/
class TMTest : public ::testing::Test {
  protected:
    std::shared_ptr<tum::TuringMachine> tm;
    std::shared_ptr<tum::TuringMachineSimulator> simulator;
    virtual void SetUp() {
      tum::TuringMachineParser tmParser("./data/tm1Test.xml"); // TuringMachine + tape wordt hierin gedefinieerd.
      tm = tmParser.getTuringMachine();
      simulator = tmParser.getTuringMachineSimulator();
    }
};
TEST_F(TMTest, simState1) {
  tum::SimulatedState state1{&*tm};

  EXPECT_EQ(state1.tapes.at(0).size(), 1);

  state1.left(0);

  EXPECT_EQ(state1.tapes.at(0).size(), 2);

  state1.right(0);
  state1.right(0);

  EXPECT_EQ(state1.tapes.at(0).size(), 3);
}
TEST_F(TMTest, simState2) {
  SimulatedState state{&*tm};                                       //<(*)B>
  EXPECT_EQ(state.tapes.at(0).size(), 1);
  state.right(0);
  state.writeCharacterAtTapehead(0, ' ');                           //<B(*) >
  state.right(0);
  state.writeCharacterAtTapehead(0, 'U');                           //<B (*)U>
  EXPECT_EQ(static_cast<char>(state.characterAtTapehead(0)), 'U');
  state.right(0);
  state.writeCharacterAtTapehead(0, 'T');                           //<B U(*)T>
  EXPECT_EQ(static_cast<char>(state.characterAtTapehead(0)), 'T');
  state.right(0);
  state.writeCharacterAtTapehead(0, 'M');                           //<B UT(*)M>
  EXPECT_EQ(static_cast<char>(state.characterAtTapehead(0)), 'M');
  state.right(0);
  state.writeCharacterAtTapehead(0, ' ');                           //<B UTM(*) >
  state.right(0);                                                   //<B UTM (*)B>

  EXPECT_EQ(state.tapeToString(0), "B UTM (*)B");

  state.cleanTapes();
  EXPECT_EQ(state.tapeToString(0), " UTM ");
}

TEST_F(TMTest, simulator) {
  // current status = (*)000111
  EXPECT_EQ(static_cast<char>(simulator->currentStates().at(0)->characterAtTapehead(0)), '0');
  simulator->advance();
  simulator->advance();
  simulator->advance();
  // current status = X00(*)111
  EXPECT_EQ(static_cast<char>(simulator->currentStates().at(0)->characterAtTapehead(0)), '1');

  simulator->advance();
  simulator->advance();
  simulator->advance();
  // current status = (*)X00Y11
  EXPECT_EQ(static_cast<char>(simulator->currentStates().at(0)->characterAtTapehead(0)), 'X');

  simulator->advance();
  simulator->advance();
  simulator->advance();
  // current status = X00(*)Y11
  EXPECT_EQ(static_cast<char>(simulator->currentStates().at(0)->characterAtTapehead(0)), 'Y');
}

TEST_F(TMTest, transition) {
  Transition trans = tm->transitions().at(0);

  EXPECT_EQ(trans.turingMachine()->transitions(), tm->transitions());
  EXPECT_EQ(trans.isDefinedTape(0), true);
  EXPECT_EQ(trans.isDefinedTape(1), false);
  EXPECT_EQ(trans.isDefinedTape(-1), false);

  EXPECT_EQ(trans.currentTapeSymbolForTape(0), 'Y');
  EXPECT_EQ(trans.nextTapeSymbolForTape(0), 'Y');
  EXPECT_EQ(trans.targetDirectionForTape(0), tum::Direction::right);

  Transition trans2 = tm->transitions().at(0);
  Transition trans3 = tm->transitions().at(1);

  EXPECT_EQ(trans == trans2, true);
  EXPECT_EQ(trans == trans3, false);

  EXPECT_EQ(trans.sourceStateIndex(), 0);
  EXPECT_EQ(trans.targetStateIndex(), 0);
  EXPECT_EQ(trans3.sourceStateIndex(), 0);
  EXPECT_EQ(trans3.targetStateIndex(), 1);

  EXPECT_EQ(trans.numberOfTapes(), 1);
  trans.setTape(0, 'X', 'Y', tum::Direction::left);
  EXPECT_EQ(trans.numberOfTapes(), 1);
  trans.setTape(1, 'A', 'B', tum::Direction::left);
  EXPECT_EQ(trans.numberOfTapes(), 2);
  trans.setNextTape('0', '3', tum::Direction::right);
  EXPECT_EQ(trans.numberOfTapes(), 3);

  EXPECT_EQ(trans.isDefinedTape(0), true);
  EXPECT_EQ(trans.isDefinedTape(1), true);
  EXPECT_EQ(trans.isDefinedTape(2), true);
  EXPECT_EQ(trans.isDefinedTape(3), false);
}

TEST_F(TMTest, basicTM1) {
  TuringMachine tm;
  Transition tr1(&tm, 1, 2);
  Transition tr2(0, 0);
  tr2.setTape(0, 0, 2, Direction::right);
  tm.addTransition(tr1);
  tr2 = tm.addTransition(tr2);
  EXPECT_EQ(*(tr1.turingMachine()), tm);
  EXPECT_EQ(*(tr2.turingMachine()), tm);
}

TEST_F(TMTest, basicTM2) {
  TuringMachine M;
  Transition t;
  EXPECT_EQ(M.transitionExists(t), false);
  t = M.addTransition({0, 1, {Rule{0, 2, Direction::right}}});
  EXPECT_EQ(M.transitionExists(t), true);
  M.addTransition({0, 3, {Rule{3, 3, Direction::right}}});
  M.addTransition({1, 1, {Rule{0, 0, Direction::right}}});
  M.addTransition({1, 1, {Rule{3, 3, Direction::right}}});
  M.addTransition({1, 2, {Rule{1, 3, Direction::left}}});
  M.addTransition({2, 2, {Rule{0, 0, Direction::left}}});
  M.addTransition({2, 2, {Rule{3, 3, Direction::left}}});
  M.addTransition({2, 0, {Rule{2, 2, Direction::right}}});
  M.addTransition({3, 3, {Rule{2, 2, Direction::right}}});
  M.addTransition({3, 4, {Rule{tum::Blank, tum::Blank, Direction::right}}});
  M.addAccepting(4);

  EXPECT_EQ(M.states().size(), 5);

  std::vector<int> test = {1, 3};
  EXPECT_EQ(M.getTargets(0), test);
  test = {1, 2};
  EXPECT_EQ(M.getTargets(1), test);
  test = {0, 2};
  EXPECT_EQ(M.getTargets(2), test);
  test = {3, 4};
  EXPECT_EQ(M.getTargets(3), test);
}


