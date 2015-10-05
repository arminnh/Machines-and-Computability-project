#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <vector>
#include <set>
#include <map>
#include <tuple>
#include <string>
#include <cassert>
#include <iostream>

namespace tum {

class Transition;
class State;

using Character = int;

/**
 * @brief Default Blank symbol.
 */
enum Symbol {
  Blank
};

#ifndef TUM_DIRECTION
#define TUM_DIRECTION

/**
 * @brief Direction enumerator.
 */
enum class Direction {
	left,
	right,
	stationary
};

#endif

/**
 * @brief Turing Machine class.
 *
 * In this Turing machine representation, states are objects (see definition of class State above),
 * but are usually referred to by the index of the vector in which they are stored.
 */
class TuringMachine
{
public:
  /**
   * @brief Default constructor.
   */
	TuringMachine();

  /**
   * @brief Add a new transition.
   * @param t Transition to be added.
   */
	Transition addTransition(Transition t);

  /**
   * @brief Test if transition exists in turing machine.
   * @param t Transition.
   */
	bool transitionExists(const Transition& t) const;

  /**
   * @brief Get all transitions.
   * @return STL vector containing all transitions.
   */
  std::vector<Transition> transitions() const;

  /**
   * @brief Get the transitions from @p sourceIndex to @p targetIndex.
   * @param sourceIndex Index of the source of the transition.
   * @param targetIndex Index of the target of the transition.
   * @return An STL vector containing all the transitions that go from @p sourceIndex to @p targetIndex.
   */
  std::vector<Transition> transitions(int sourceIndex, int targetIndex) const;
  /**
   * @brief Get the transitions that are reachable from @p sourceIndex.
   * @param sourceIndex The starting index of the transitions.
   * @return An STL map mapping the possible destinations with the possible transitions.
   */
	std::map<int, std::vector<Transition>> transitions(int sourceIndex) const;

  /**
   * @brief Get the possible targets starting from @p sourceIndex.
   * @param sourceIndex The (current) sourceIndex.
   * @return An STL vector containing the indices of all possible targets.
   */
	std::vector<int> getTargets(int sourceIndex) const;

  /**
   * @brief Define an accepting state.
   * @param index The index of the accepting state.
   */
	void addAccepting(int index);

  /**
   * @brief Get the initial state of the TM.
   * @return Index of the initial state.
   */
  int initialState() const;
  /**
   * @brief Set the initial state of the TM.
   * @param i Index of the 'initial state'.
   */
  void setInitialState(int i);

  /**
   * @brief Add a state.
   * @param state The state to be added.
   */
 	void addState(const State& state);
  /**
   * @brief Add @p n states to the turing machine.
   * @param n The amount of states to be added.
   */
 	void addNumberOfStates(unsigned n);

  /**
   * @brief Test if @p index is the index of an accepting state.
   * @param index The index of the state.
   * @return True if the state is accepting.
   */
	bool isAcceptingState(int index) const;
  /**
   * @brief Test if @p state is accepting.
   * @param state The state to be tested.
   * @return True if @p state is accepting.
   */
	bool isAcceptingState(const State& state) const;


  void setName(std::string name);
  std::string name() const;
  /**
   * @brief Set the number of tapes.
   * @param number The amount of tapes to be set.
   */
	void setNumberOfTapes(int number);
  /**
   * @brief Get the number of tapes.
   * @return The number of tapes.
   */
	int numberOfTapes() const;
  /**
   * @brief Get the number of symbols.
   * @return The number of symbols.
   */
  int numberOfSymbols() const;

  /**
   * @brief Get all (unique) symbols.
   * @return STL Set of characters.
   */
  std::set<Character> allSymbols() const;

  /**
   * @brief Define the blank symbol.
   * @param symbol The (new) blank symbol.
   */
	void setBlankSymbol(Character symbol);
  /**
   * @brief Get the blank symbol.
   * @return The blank symbol.
   */
	Character blankSymbol() const;
  /**
   * @brief Define the 'AnySymbol' symbol.
   * @param symbol The symbol that will be defined as 'AnySymbol'.
   */
	void setAnySymbolSymbol(Character symbol);
  /**
   * @brief Get the 'AnySymbol' symbol.
   * @return 'AnySymbol' character.
   */
	Character anySymbolSymbol() const;
  /**
   * @brief Test if an 'AnySymbol' symbol is defined.
   * @return True if there is an 'AnySymbol' defined.
   */
  bool anySymbolSymbolSet() const;
  /**
   * @brief Set 'SameSymbol' symbol.
   * @param symbol Newly defined 'SameSymbol'.
   */
	void setSameSymbolSymbol(Character symbol);
  /**
   * @brief Get the 'SameSymbol' symbol.
   * @return 'SameSymbol' character.
   */
	Character sameSymbolSymbol() const;
  /**
   * @brief Test if a 'SameSymbol' symbol is set.
   * @return True if there is a 'SameSymbol' defined.
   */
  bool sameSymbolSymbolSet() const;

  /**
   * @brief Get the states of the machine.
   * @return STL vector containing all states.
   */
  std::vector<State> states() const;

  bool operator==(const TuringMachine& tm) const; //!< Equality operator.

private:
	friend class State;
	friend class Transition;

  std::string _name;

  friend std::ostream& operator << (std::ostream& os, const TuringMachine& tm);

	int _numberOfTapes;
	Character _blankSymbol;
	Character _anySymbolSymbol;
	Character _sameSymbolSymbol;

  bool _anySymbolSymbolSet {false};
  bool _sameSymbolSymbolSet {false};

	std::vector<State> _states;

	// first key is the index of the source state, second key is the index of the target state
	std::map<int, std::map<int, std::vector<Transition>>> _transitions;
	int _initialState;
	std::vector<int> _acceptingStates;
};

  std::ostream& operator << (std::ostream& os, const State& state); //!< Output operator.
  std::ostream& operator << (std::ostream& os, const Transition& transition); //!< Output operator.
  std::ostream& operator << (std::ostream& os, const TuringMachine& tm);  //!< Output operator.
} // namespace tum

#endif
