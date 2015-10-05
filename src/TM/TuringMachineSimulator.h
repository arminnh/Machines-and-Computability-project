#ifndef TUM_TURINGMACHINE_SIMULATOR_H
#define TUM_TURINGMACHINE_SIMULATOR_H

#include <iostream>
#include <memory>

#include "TuringMachine.h"
// #include "TMTransition.h"
#include "TMSimulatedState.h"

namespace tum {

/**
 * @brief This class runs the simulation for a turing machine.
 * @details The tapes are found in the simulator, not in the Turing Machine itself.
 */
class TuringMachineSimulator
{
public:
  /**
   * @brief Constructor.
   * @param tm The turing machine to be simulated.
   * @param tapes The tapes on which the turing machine will be simulated.
   */
	TuringMachineSimulator(TuringMachine* tm, const std::vector<Tape>& tapes = {});

  /**
   * @brief Get the turing machine that is being simulated.
   * @return the utm::TuringMachine that is being simulated.
   */
	const TuringMachine& turingMachine() const;

  /**
   * @brief Advance a step in the simulation.
   * @return True if the advancement was succesful.
   */
	bool advance();

  /**
   * @brief Rewind the tape.
   * @return True if rewinding was successful.
   */
	bool rewind();
  /**
   * @brief Test if it's possible to rewind the tape.
   * @return True if rewinding is possible (true if history has been kept).
   */
	bool canRewind() const;

  int nrStepsExecuted() const;

  /**
   * @brief Set the number of entries to be kept.
   * @param nrEntriesToKeep is Amount of entries to keep.
   */
	void setNrEntriesToKeep(size_t nrEntriesToKeep);
  void outputNextTransitions(bool boolean);
  /**
   * @brief Remove the old entries (that must not be kept).
   */
	void removeOldEntries();

  /**
   * @brief Test if the simulation has halted.
   * @return True if the simulation has stopped.
   */
	bool isHalted() const;
  /**
   * @brief Test if the simulation has halted.
   * @return True if the simulation has halted.
   */
	bool isDead() const;

  /**
   * @brief Get the current states.
   * @return STL vector containing (pointers to) all current simulated states.
   */
  std::vector<std::shared_ptr<const SimulatedState>> currentStates() const;

	friend std::ostream& operator<<(std::ostream&, const TuringMachineSimulator&);  //!< Output operator.

private:
	TuringMachine& turingMachine();

	std::vector<std::shared_ptr<SimulatedState>> computeNextSimulatedStates();


	std::vector<Transition> nextMoves(const SimulatedState& state) const;   //!< finds and returns all the possible next moves given the current state (combination of tapehead symbols)

	size_t _nrHistoryEntriesToKeep {0};

  int _nrStepsExecuted {0};
  bool _outputNextTransitions {true};

	// where the index with the greatest value contains the states closest to current (most recent)
	std::vector<std::vector<std::shared_ptr<SimulatedState>>> _pastStates;

	std::vector<std::shared_ptr<SimulatedState>> _currentStates;

	// where the index with the greatest value contains the states closest to current (earliest)
	std::vector<std::vector<std::shared_ptr<SimulatedState>>> _futureStates;

	TuringMachine* _tm;
};

std::ostream& operator<<(std::ostream&, const TuringMachineSimulator&);

std::ostream& operator<<(std::ostream& os, Direction);

} // namespace tum

#endif
