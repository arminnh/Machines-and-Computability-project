#ifndef TM_SIMULATED_STATE_H
#define TM_SIMULATED_STATE_H

#include <memory>

#include "TMTape.h"
#include "TuringMachine.h"
#include "TMTransition.h"

// simulation of the state of one branch in the computation of a turing machine

namespace tum {

#ifndef TUM_DIRECTION
#define TUM_DIRECTION

// just to be sure
enum class Direction {
	left,
	right,
	stationary
};

#endif

/**
 * @brief Simulated State class.
 */
class SimulatedState : public std::enable_shared_from_this<SimulatedState>
{
public:
  /**
   * @brief Constructor.
   * @param tm The machine this state is part of.
   * @param parent The parent of this state.
   */
	SimulatedState(TuringMachine* tm, std::shared_ptr<SimulatedState> parent = nullptr, int spawnedFromBranch = -1);
  /**
   * @brief Copy constructor sets.
   */
	SimulatedState(std::shared_ptr<SimulatedState> source, int spawnedFromBranch);

  /**
   * @brief Go left.
   * @details The tape will expand automatically if needed.
   * @param tapeIndex The index of the tape.
   */
	void left(std::size_t tapeIndex);
  /**
   * @brief Go right.
   * @details The tape will expand automatically if needed.
   * @param tapeIndex The index of the tape.
   */
	void right(std::size_t tapeIndex);

  /**
   * @brief Get the character at the tapehead.
   * @param tapeIndex The index of the tape.
   * @return The character at the tapehead.
   */
	Character characterAtTapehead(std::size_t tapeIndex) const;
  /**
   * @brief Write @character at the tapehead.
   * @param character The character to be written.
   * @param tapeIndex The index of the tape.
   */
	void writeCharacterAtTapehead(std::size_t tapeIndex, Character character);

  /**
   * @brief Get the current state.
   * @return The index of the current state.
   */
	int currentState() const;
  /**
   * @brief Test if the machine halted at this state.
   * @return True if the state is a halting state.
   */
	bool isHalted() const;

  /*! @brief Test if the simulated state is the result of a branch.
   * @return -1 if it is not spawned from a branch.
   *          The index of the branch otherwise.
   */
  int spawnedFromBranch() const;

  /**
   * @brief Test if the transition is compatible.
   * @details A transition is compatible if it is not an illegal transition.
   * @param transition The transition to be tested.
   * @return True if the transition is compatible.
   */
	bool transitionIsCompatible(const Transition& transition) const;
  /**
   * @brief Execute the transition.
   * @param transition The transition to be executed.
   */
	void performTransition(const Transition& transition);

  /**
   * @brief Clear the blank symbols from the tape.
   * @detail Removes the starting and ending sequences of blank symbols from the tape(s).
   * @note When getting the string representation of a tape after it has been cleaned, the tape head will not be visible.
   *  At this point the tape head is at an index that exceeds the size of the tape.
   */
	void cleanTapes();

  /**
   * @brief Convert the tape to a string.
   * @param tapeIndex The index of the tape that must be converted.
   * @return STL string representing the tape.
   */
	std::string tapeToString(size_t tapeIndex, std::string tapeheadMarker = "*") const;

	std::vector<Tape> tapes;

private:
	TuringMachine* _tm;

  int _spawnedFromBranch {-1};
	std::weak_ptr<SimulatedState> _parent;

	int _currentState;

	std::vector<std::size_t> _tapeheads;
};

}

#endif
