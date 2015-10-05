#ifndef TURING_MACHINE_CONVERTER_H
#define TURING_MACHINE_CONVERTER_H

#include "TuringMachine.h"
#include "TMTransition.h"
#include "TMTape.h"
#include "TMState.h"

namespace tum {

/**
 * @brief Class for converting a given tape to an encoded tape.
 *
 * The original tape is the tape from an arbitrary Turing Machine.
 * There are 4 encoded tapes:
 * Tape 1: The simulation tape.
 * Tape 2: The encodation of the original TM.
 * Tape 3: The current state.
 * Tape 4: The working tape (scrap tape).
 */
class TuringMachineConverter
{
public:
  /**
   * @brief Constructor.
   * @param tm The Turing Machine that will be converted.
   */
	TuringMachineConverter(const TuringMachine& tm);
  /**
   * @brief Encode the turingmachine and input from @p tapeToEncode.
   * @param tapeToEncode The tape with input.
   * @return STL Vector containing the 4 tapes for the UTM.
   */
	std::vector<Tape> encode(const Tape& tapeToEncode);
  /**
   * @brief Decode a tape (writte in binary notation) to it's original form.
   * @param tapeToDecode The encoded tape.
   * @return The decoded tape.
   */
  Tape decodeSimulatedTape(const Tape& tapeToDecode) const;
  int decodeCurrentStateTape(const Tape& tapeToDecode) const;
private:
  bool _encoded {false};
	const TuringMachine* _tm;
	std::map<std::string, Character> _symbolDecodings;
	std::map<std::string, int> _stateDecodings;
};

} // namespace tum

#endif

