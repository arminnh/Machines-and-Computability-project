#ifndef TM_TAPE_H
#define TM_TAPE_H

#include <iostream>
#include <vector>
#include <deque>

namespace tum {

using Character = int;

/**
 * @brief Tape class for running simulations.
 */
class Tape : public std::deque<Character>
{
public:
	Tape();
	Tape(std::string contents);
};

std::ostream& operator<<(std::ostream& os, const Tape& t);  //!< operator overload.

} // namespace tum

#endif
