#ifndef TUM_STATE_H
#define TUM_STATE_H

#include <string>

namespace tum {

class TuringMachine;

/**
 * @brief State class.
 * @details A state object for a tum::TuringMachine.
 */
class State
{
  public:
    /**
     * @brief Constructor.
     * @param TM The machine this state is part of.
     */
    State(TuringMachine* TM);

    /**
     * @brief Get the turing machine this state is part of.
     * @return tum::TuringMachine pointer.
     */
    TuringMachine* turingMachine() const;

    /**
     * @brief Get the index of this state.
     * @return The index.
     */
    int index() const;

    /**
     * @brief Get the name of this state.
     * @return The name as an STL string.
     */
    std::string name() const;
    /**
     * @brief Set the name of the state.
     * @param name The name of the state.
     */
    void setName(std::string name);

    /**
     * @brief Define this state as a halting state or not.
     * @param halting True if the state should be halting.
     */
    void setHaltingState(bool halting);
    /**
     * @brief Test if this is a halting state.
     * @return True if the state is halting.
     */
    bool isHaltingState() const;

    /**
     * @brief Set the turing machine this state is part of.
     * @param tm The turing machine this state is part of.
     * @return The state object.
     */
    State& setTuringMachine(TuringMachine* tm);

    bool operator==(const State& other) const;  //!< Equality operator.
  private:
    TuringMachine* _tm;
    std::string _name;
    bool _isHaltingState;
};

}

#endif

