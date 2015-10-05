#ifndef FINITEAUTOMATON_H
#define FINITEAUTOMATON_H

#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <stack>
#include <utility>
#include <memory>

#include <UTM/Symbol.h>

static const std::size_t NullIndex = -1;

/**
  * @brief Special symbols for internal use.
  *
  * The values are chosen in on of the unicode private use areas (0xE000...0xF8FF).
  */
enum SpecialSymbols {
  Epsilon = 0xE000,
  StartOfStack,
  OtherStartOfStack,
  Any,
  Error,
  Concatenate,
  Union,
  ZeroOrMore,
  OneOrMore,
  ZeroOrOne,
  LeftParen,
  RightParen,
  LastSymbol = 0xF8FF
};

// Forward declarations.
class FiniteAutomaton;
class State;

/**
  * @brief Class representing an arc in a finite automaton.
  */
class Arc {
  public:
    /**
      * Constructor.
      *
      * @param automaton The automaton this arc is part of.
      * @param index The index for this arc.
      */
    Arc(const FiniteAutomaton *automaton, std::size_t index) : m_automaton(automaton), m_index(index) {
    }

    /**
      * @brief Get the automaton this arc is part of.
      *
      * @return The automaton this arc is part of.
      */
    const FiniteAutomaton* automaton() const {
      return m_automaton;
    }

    /**
      * @brief Get the index for this automaton.
      *
      * Indexing starts at 0.
      *
      * @return The index for this automaton.
      */
    std::size_t index() const {
      return m_index;
    }

    /**
      * @brief Get the state from which this arc originates.
      *
      * @return The state from which this arc originates.
      */
    inline State source() const;

    /**
      * @brief Get the state to which this arc goes.
      *
      * @return The state to which this arc goes.
      */
    inline State target() const;

    /**
      * @brief Get the symbol associated with this arc.
      *
      * @return The symbol associated with this arc.
      */
    inline int symbol() const;

    /**
     * @brief Get the rule assiciated with this arc.
     *
     * @return The rule assiociated with this arc.
     */
    inline std::pair<int, std::vector<int> > rule() const;

    /**
      * @brief Set the symbol associated with this arc.
      *
      * @param symbol The new symbol.
      *
      * @return This arc.
      */
    inline Arc& setSymbol(int symbol);

    /**
     * @brief Set the rule associated with this arc.
     *
     * @param The new rule.
     *
     * @return This arc.
     */
    inline Arc& setRule(int topOfStack, std::vector<int> newTopOfStack);

    inline std::size_t& setIndex(std::size_t index);

  private:
    const FiniteAutomaton *m_automaton; //!< The automaton this arc is part of.
    std::size_t m_index; //!< The arc's index.
};

/**
  * @brief Class representing a state in a finite automaton.
  */
class State {
  public:
    /**
      * @brief Constructor.
      *
      * @param automaton The automaton this state is part of.
      * @param index The index for this state.
      */
    State(const FiniteAutomaton *automaton, std::size_t index) : m_automaton(automaton), m_index(index) {
    }

    /**
      * @brief Get the automaton this arc is part of.
      *
      * @return The automaton this arc is part of.
      */
    const FiniteAutomaton* automaton() const {
      return m_automaton;
    }

    /**
      * @brief Get the index for this automaton.
      *
      * Indexing starts at 0.
      *
      * @return The index for this automaton.
      */
    std::size_t index() const {
      return m_index;
    }

    /**
      * @brief Get the list of arcs that go out of this state.
      *
      * @return The list of arcs.
      */
    inline const std::vector<Arc>& arcs() const;

    /**
      * @brief Equality comparison operator.
      *
      * @param other The state to compare against.
      *
      * @return True if this state is the same as state @p other.
      */
    bool operator==(const State &other) const {
      return m_automaton == other.m_automaton && m_index == other.m_index;
    }

    /**
      * @brief Less than comparison operator.
      *
      * @param other The state to compare against.
      *
      * @return True if this state has an index that is less than that of state @p other.
      */
    bool operator<(const State &other) const {
      return m_index < other.m_index;
    }

  private:
    const FiniteAutomaton *m_automaton; //!< The automaton this state is part of.
    std::size_t m_index; //!< The state's index.
};

/**
  * @brief Class representing a finite state automaton.
  *
  * This class can be used to represent DFA, NFA and epsilon-NFA automata.
  */
class FiniteAutomaton : public std::enable_shared_from_this<FiniteAutomaton>
{
  public:
    /**
      * @brief Constructor.
      *
      * Creates an empty finite state automaton.
      */
    FiniteAutomaton() : m_start(0), m_stack({StartOfStack}) {
    }

    //! @name States
    //@{

    /**
      * @brief Get the number of states in this automaton.
      *
      * @return The number of states in this automaton.
      */
    std::size_t numStates() const {
      return m_stateArcs.size();
    }

    /**
      * @brief Get the state with the specified @p index.
      *
      * Indexing starts at 0.
      *
      * @param index The index of the state to get.
      *
      * @return The state with the specified @p index.
      */
    State state(std::size_t index) const {
      assert(index < numStates());
      return State(this, index);
    }

    /**
      * @brief Add a state to this automaton.
      *
      * @return The newly added state.
      */
    State addState();

    /**
      * @brief Remove a state from this automaton.
      *
      * @param state The state to remove.
      */
    void removeState(const State &state);

    //@}

    //! @name Arcs
    //@{

    /**
      * @brief Get the number of arcs in this automaton.
      *
      * @return The number of arcs in this automaton.
      */
    std::size_t numArcs() const {
      return m_arcs.size();
    }

    /**
      * @brief Get the arc with the specified @p index.
      *
      * Indexing starts at 0.
      *
      * @param index The index of the arc to get.
      *
      * @return The arc with the specified @p index.
      */
    Arc arc(std::size_t index) const {
      assert(index < numArcs());
      return Arc(this, index);
    }

    /**
      * @brief Add an arc to this automaton.
      *
      * @param source The source state from which the arc leaves.
      * @param target The target state where the arc arrives.
      *
      * @return The newly added arc.
      */
    Arc addArc(const State &source, const State &target);

    /**
      * @brief Remove an arc from this automaton.
      *
      * @param arc The arc to remove.
      */
    void removeArc(const Arc &arc);

    //@}

    // inline const std::vector<std::pair<int, std::vector<int> > >& rules() const;

    /**
      * @brief Get the start state for this automaton.
      *
      * @return The start state for this automaton.
      */
    State start() const {
      return State(this, m_start);
    }

    /**
      * @brief Set the start state for this automaton.
      *
      * @param state The new start state.
      */
    void setStart(const State &state) {
      m_start = state.index();
    }

    /**
      * @brief Get the set of accepting states.
      *
      * @return The set of accepting states.
      */
    const std::set<State>& accepting() const {
      return m_accepting;
    }

    /**
      * @brief Add or remove a state from the set of accepting states.
      *
      * @param state The state to add or remove from the set of accepting states.
      * @param accept If true, the state will be added to the set of
      *               accepting states. If false, the state will be removed from
      *               the set of accepting states.
      */
    void setAccepting(const State &state, bool accept = true);

    /**
      * @brief Get the alphabet for this automaton.
      *
      * @return The alphabet.
      */
    std::set<int> alphabet() const;

    /**
      * @brief Get the ECLOSE set for the specified state.
      *
      * @param state The state for which to get the ECLOSE set.
      *
      * @return The ECLOSE set for the specified state.
      */
    std::set<State> eclose(const State &state) const;

    /**
      * @brief Get the ECLOSE set for the specified set of states.
      *
      * @param states The states for which to get the ECLOSE set. This
      *               parameter will also be used as output by adding the
      *               ECLOSE states to this set.
      */
    void eclose(std::set<State> &states) const;

    /**
      * @brief Check if two states are equivalent.
      *
      * @param state1 The first state.
      * @param state2 The second state.
      *
      * @return True is @p state1 and @p state2 are equivalent.
      */
    bool equivalent(const State &state1, const State &state2) const;

    /**
     * @brief Convert the finite automaton to a PDA using acceptance by final state.
     */
    void final_state();

    /**
     * @brief Convert the finite automaton to a PDA using acceptance by empty stack.
     */
    void empty_stack();

  private:
    friend class State;
    friend class Arc;

    void p_addStartState(); // Add a first state and push new starting symbol on stack
    void p_addFinalAcceptingState();  // Add final accepting state
    void p_addFinalClearingState();   // Add final state that clears the stack


    std::vector<std::pair<std::size_t, std::size_t> > m_arcs; //!< Source and target indices for the arcs
    std::vector<int> m_symbols; //!< Symbols for the arcs.
    std::vector<std::vector<Arc> > m_stateArcs; //!< Arcs for the states
    std::set<State> m_accepting; //!< The set of accepting states.
    std::size_t m_start; //!< Index of the start state.
    std::stack<int>  m_stack; //!< Stack of Push down automaton
    std::vector<std::pair<int, std::vector<int> > > m_rules; // !< vector of rules: rules = pair<CurTopOfStack, NewTopOfStack>
};


State Arc::source() const {
  return State(m_automaton, m_automaton->m_arcs[m_index].first);
}

int Arc::symbol() const {
  return m_automaton->m_symbols[m_index];
}

std::pair<int, std::vector<int> > Arc::rule() const {
  return m_automaton->m_rules[m_index];
}

Arc& Arc::setSymbol(int symbol) {
  const_cast<FiniteAutomaton*>(m_automaton)->m_symbols[m_index] = symbol;
  return (*this);
}

Arc& Arc::setRule(int topOfStack, std::vector<int> newTopOfStack) {
  std::pair<int, std::vector<int> > rule = std::make_pair(topOfStack, newTopOfStack);
  const_cast<FiniteAutomaton*>(m_automaton)->m_rules[m_index] = rule;
  return (*this);
}

State Arc::target() const {
  return State(m_automaton, m_automaton->m_arcs[m_index].second);
}

std::size_t& Arc::setIndex(std::size_t index) {
  m_index = index;
  return m_index;
}

const std::vector<Arc>& State::arcs() const {
  return m_automaton->m_stateArcs[m_index];
}

#endif
