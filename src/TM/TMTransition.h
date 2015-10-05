#ifndef TM_TRANSITION_H
#define TM_TRANSITION_H

#include <iostream>
#include <vector>
#include <tuple>

namespace tum {

class TuringMachine;
class State;

using Character = int;

#ifndef TUM_DIRECTION
#define TUM_DIRECTION

enum class Direction {
	left,
	right,
	stationary
};

#endif

/**
 * @brief Transition class for a Turing Machine.
 */
class Transition
{
  public:
    /**
     * @brief Default constructor.
     */
    Transition();
    /**
     * @brief Constructor.
     * @param tm The turingmachine the transition is part of.
     */
    Transition(TuringMachine* tm);
    /**
     * @brief Constructor.
     * @param sourceStateIndex The source of the transition.
     * @param targetStateIndex The target of the transition.
     */
    Transition(int sourceStateIndex, int targetStateIndex);
    /**
     * @brief Constructor.
     * @param tm The turingmachine the transition is part of.
     * @param sourceStateIndex The source of the transition.
     * @param targetStateIndex The target of the transition.
     */
    Transition(TuringMachine*, int sourceStateIndex, int targetStateIndex);
    /**
     * @brief Constructor.
     * @param sourceStateIndex The source of the transition.
     * @param targetStateIndex The target of the transition.
     * @param symbolsAndDirections A vector where each element defines the current character, new character, direction head
     *        should move.
     *        The vector contains the tuples for each tape of the turing machine (In case of single tape TM, vector size = 1).
     */
    Transition(int sourceStateIndex, int targetStateIndex, std::vector<std::tuple<Character, Character, Direction>> symbolsAndDirections);

    /**
     * @brief Get the turingmachine this transition is part of.
     */
    const TuringMachine* turingMachine() const;
    /**
     * @brief Set the turingmachine this transition is part of.
     * @param tm The turingmachine.
     */
    Transition& setTuringMachine(TuringMachine* tm);

    /**
     * @brief Set the source state.
     * @param stateIndex Index of the source state.
     * @return This transition (to enable chaining commands).
     */
    Transition& setSourceState(int stateIndex);
    /**
     * @brief Set the target state.
     * @param stateIndex Index of the target state.
     * @return This transition (to enable chaining commands).
     */
    Transition& setTargetState(int stateIndex);
    /**
     * @brief Change a states index.
     * @details This is used in the case of subroutining.
     * @param from The original index.
     * @param to The new index.
     * @return This transition (to enable chaining commands).
     */
    Transition& stateIndexChangedFromTo(int from, int to); // used when subroutining

    /**
     * @brief Get the source state.
     * @return The source state.
     */
    const State& sourceState() const;
    /**
     * @brief Get the target state.
     * @return the target state.
     */
    const State& targetState() const;
    /**
     * @brief Get the source states index.
     * @return The source states index.
     */
    int sourceStateIndex() const;
    /**
     * @brief Get the target states index.
     * @return The target states index.
     */
    int targetStateIndex() const;

    // operations on the tapes of the transition
    /**
     * @brief Define a new set of instructions for this transition on tape @p tapeIndex.
     * @param tapeIndex Index of the tape.
     * @param currentTapeSymbol Symbol that will be read.
     * @param newTapeSymbol Symbol that will be written on the tape.
     * @param direction The direction the head will move in.
     * @note If the tapeIndex already exists, the current values for this transition, on the given tape will be overwritten.
     */
    Transition& setTape(int tapeIndex, Character currentTapeSymbol, Character newTapeSymbol, Direction direction);
    /**
     * @brief Set a new transition on the next tape.
     * @param currentTapeSymbol Symbol that will be read.
     * @param newTapeSymbol Symbol that will be written on the tape.
     * @param direction The direction the head will move in.
     */
    Transition& setNextTape(Character currentTapeSymbol, Character newTapeSymbol, Direction direction);
    /**
     * @brief Get the number of tapes.
     * @return The amount of tapes (== index of potential next new tape).
     */
    int numberOfTapes() const;
    /**
     * @brief Test if tape exists.
     * @param tapeIndex The index of the tape to be tested.
     * @return True if the tape exists.
     */
    bool isDefinedTape(int tapeIndex) const;

    // operations on the tape symbols of the transition and the direction
    /**
     * @brief Test if given symbol is current symbol on the tape.
     * @param tapeIndex The index of the tape.
     * @param symbol The symbol to be tested.
     * @return True if the symbol matches the Character under the head.
     */
    bool isCurrentTapeSymbolForTape(int tapeIndex, Character symbol) const;
    /**
     * @brief Test if given symbol is the next symbol on the tape.
     * @param tapeIndex The index of the tape.
     * @param symbol The symbol to be tested.
     * @return True if the symbol matches the next Character on the tape.
     */
    bool isNextTapeSymbolForTape(int tapeIndex, Character symbol) const;
    /**
     * @brief Get the current symbol on the tape.
     * @param tapeIndex The index of the tape.
     * @return The requested Character.
     */
    Character currentTapeSymbolForTape(int tapeIndex) const;
    /**
     * @brief Get the next symbol on the tape.
     * @param tapeIndex The index of the tape.
     * @return The requested Character.
     */
    Character nextTapeSymbolForTape(int tapeIndex) const;

    /**
     * @brief Test if given direction is direction tape will move in.
     * @param tapeIndex The index of the tape.
     * @param direction The direction to be tested.
     * @return True if the direction is correct.
     */
    bool isDirectionForTape(int tapeIndex, Direction direction) const;
    /**
     * @brief Get the direction the tape will move in.
     * @param tapeIndex The index of the tape.
     * @return Requested direction.
     */
    Direction targetDirectionForTape(int tapeIndex) const;

    // comparisons to other transitions
    /**
     * @brief Test if 2 transitions have the same states.
     * @param other The other transition.
     * @return True if both transitions have the same source and target states.
     */
    bool equalStateTransition(const Transition& other) const;
    /**
     * @brief Test if 2 transitions have the same input requirements.
     * @details Two transitions have the same input requirements if they have the same source state and the same
     * amount of tapes.
     * @return True if both transitions have the same input requirements.
     */
    bool equalInputRequirements(const Transition& other) const;

    // ...

  private:
    friend std::ostream& operator<<(std::ostream&, const Transition&);

    TuringMachine* _tm;
    int _sourceState;
    int _targetState;

    // index in vector is tape number, first tuple element is the character upon which the turing machine will follow
    // the transition for the given tape, second tuple element is the character that will be written on the tape,
    // and the third tuple element is the direction the tapehead will move in on the next step.

    std::vector<std::tuple<Character, Character, Direction>> _symbolsAndDirectionPerTape;

    const std::tuple<Character, Character, Direction>* symbolsAndDirection(int tapeIndex) const;
};

std::ostream& operator<<(std::ostream& os, Direction d);
bool operator==(const Transition& t1, const Transition& t2);
bool operator!=(const Transition& t1, const Transition& t2);

}

#endif
