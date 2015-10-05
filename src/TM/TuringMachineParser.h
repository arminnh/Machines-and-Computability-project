#ifndef TUM_TURING_MACHINE_PARSER
#define TUM_TURING_MACHINE_PARSER

#include <string>
#include <vector>
#include <memory>
#include <UTM/pugixml.hpp>

#include "TuringMachine.h"
#include "TMTransition.h"
#include "TMState.h"
#include "TuringMachineSimulator.h"

namespace tum {

/**
 * @brief Turing Machine Parser class.
 *
 * This class parses a turing machine from a given file.
 */
class TuringMachineParser
{
public:
  /**
   * @brief Constructor.
   * @param filename The name of the file to be parsed.
   */
	TuringMachineParser(std::string filename);
  /**
   * @brief Get the parsed turing machine.
   * @param theJosseWay Don't ask... 
   * @return The parsed turing machine.
   */
	std::shared_ptr<TuringMachine> getTuringMachine(bool theJosseWay = false);
  /**
   * @brief Get the TM simulator.
   * @details When calling getTuringMachineSimulator, the initial tapes are already in the simulator.
   * @return The TM Simulator.
   */
	std::shared_ptr<TuringMachineSimulator> getTuringMachineSimulator();
  /**
   * @brief Initialize the Simulators tapes.
   * @return STL vector containing the tapes.
   */
	std::shared_ptr<std::vector<Tape>> getInitialTapes();
private:
	Transition getTransition(const std::vector<State>& states, pugi::xml_node transitionNode);  //!< Parse a transitionNode - The old way.
	Transition getTransitionTheJosseWay(const std::vector<State>& states, pugi::xml_node transitionNode); //!< Parse a transitionNode - The new way.
	Tape getTapeContent(pugi::xml_node tapeNode);   //!< Parse a tapeNode.

	int indexForStateName(const std::vector<State>& states, std::string stateName) const;   //!< Get the index of the transition with name @p stateName.
	Direction directionForCharacter(char c) const;    //!< Get the direction symbolized by a given character @p c.

	std::shared_ptr<TuringMachine> _tm;
	pugi::xml_document _doc;
};

} // namespace tum

#endif

