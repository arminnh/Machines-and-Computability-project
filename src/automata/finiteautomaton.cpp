#include <iostream>

#include "finiteautomaton.h"


State FiniteAutomaton::addState() {
  std::size_t index = m_stateArcs.size();
  m_stateArcs.resize(index + 1);
  return State(this, index);
}

void FiniteAutomaton::removeState(const State &state) {
  m_stateArcs.erase(m_stateArcs.begin() + state.index());
  // remove all arcs involving this state
  std::vector<std::size_t> arcs;
  for (std::size_t i = 0; i < m_arcs.size(); ++i)
    if (m_arcs[i].first == state.index() || m_arcs[i].second == state.index())
      arcs.push_back(i);
  for (auto i = arcs.rbegin(); i != arcs.rend(); ++i)
    m_arcs.erase(m_arcs.begin() + *i);
}

Arc FiniteAutomaton::addArc(const State &source, const State &target) {
  assert(source.index() < numStates());
  assert(target.index() < numStates());

  std::size_t index = m_arcs.size();
  m_arcs.push_back(std::make_pair(source.index(), target.index()));
  m_stateArcs[source.index()].push_back(Arc(this, index));
  m_symbols.push_back(Epsilon);
  m_rules.push_back(std::make_pair(Any, std::vector<int> {Any}));

  return Arc(this, index);
}

void FiniteAutomaton::removeArc(const Arc &arc) {
  m_arcs.erase(m_arcs.begin() + arc.index());
}

void FiniteAutomaton::setAccepting(const State &state, bool accept) {
  if (accept)
    m_accepting.insert(state);
  else if (m_accepting.find(state) != m_accepting.end())
    m_accepting.erase(m_accepting.find(state));
  std::cout << "added accept with index: " << state.index() << std::endl;
}

std::set<int> FiniteAutomaton::alphabet() const {
  std::set<int> result;
  for (auto symbol = m_symbols.begin(); symbol != m_symbols.end(); ++symbol)
    if (*symbol != Epsilon)
      result.insert(*symbol);
  return result;
}

std::set<State> FiniteAutomaton::eclose(const State &state) const {
  std::set<State> result;
  result.insert(state);
  eclose(result);
  return result;
}

void FiniteAutomaton::eclose(std::set<State> &result) const {
  std::size_t size = result.size();
  for (auto state = result.begin(); state != result.end(); ++state) {
    std::vector<Arc> arcs = state->arcs();
    for (auto arc = arcs.begin(); arc != arcs.end(); ++arc)
      if (arc->symbol() == Epsilon)
        result.insert(arc->target());
  }

  if (size < result.size())
    eclose(result);
}

bool FiniteAutomaton::equivalent(const State &state1, const State &state2) const {
  if((accepting().find(state1) == accepting().end() && accepting().find(state2) != accepting().end()) ||
      (accepting().find(state1) != accepting().end() && accepting().find(state2) == accepting().end()))
    return false;

  if(state1 == state2)
    return true;

  std::map<unsigned int, std::size_t> trans1;
  std::map<unsigned int, std::size_t> trans2;

  std::vector<Arc> arcs1 = state1.arcs();
  std::vector<Arc> arcs2 = state2.arcs();

  /* fill maps */
  for(auto arc1 = arcs1.begin(); arc1 != arcs1.end(); ++arc1)
    trans1[arc1->symbol()] = arc1->target().index();

  for(auto arc2 = arcs2.begin(); arc2 != arcs2.end(); ++arc2)
    trans2[arc2->symbol()] = arc2->target().index();

  if (trans1.size() != trans2.size())
    return false;

  for(auto a : alphabet()) {
    if (trans1.find(a) == trans1.end())
      continue;
    if (trans2.find(a) == trans2.end())
      return false;
    if(!equivalent(state(trans1.find(a)->second), state(trans2.find(a)->second)))
      return false;
  }

  return true;
}

void FiniteAutomaton::final_state() {
  // (accepting.empty() <=> (empty_stack)
  if (m_accepting.empty()) {
    /** WORKFLOW:
     * DONE -- Add new final state
     * DONE -- Add new start state
     * DONE -- Add new first item on stack
     * DONE -- enter old PDA
     * DONE -- If top of stack == new first item on stack
     *  DONE -- Go to Accept state
     */
    p_addFinalAcceptingState();
    p_addStartState();
  }
}

void FiniteAutomaton::empty_stack() {
  // (accepting.empty() <=> (empty_stack)
  if (!m_accepting.empty()) {
    /** WORKFLOW:
     * Add new final state
     * Add new start state
     * Add new first item on stack
     * Enter old PDA
     * If reaching an accepting state
     *  Clear the stack
     */
    p_addFinalClearingState();
    p_addStartState();
  }
}

void FiniteAutomaton::p_addFinalClearingState() {
  // Add new state
  State newState = addState();

  // Add Arcs to new final state + rule
  for (auto& state : m_accepting) {
    addArc(state, newState).setRule(Any, std::vector<int>{Epsilon});
  }
  addArc(State(this, numStates()-1), State(this, numStates()-1)).setRule(Any, std::vector<int> {Epsilon});
  m_accepting.clear();
}

void FiniteAutomaton::p_addFinalAcceptingState() {
  // Add new Accept state
  addState();
  setAccepting(State(this, numStates()));

  // Add Arcs to new final state + rule
  for (std::size_t i = 0; i < numStates()-1; ++i) {
    addArc(State(this, i), State(this, numStates()-1)).setRule(OtherStartOfStack, std::vector<int> {Epsilon});
  }
}

void FiniteAutomaton::p_addStartState() {
  // Increment indices for all source and target indices
  for(auto it = m_arcs.begin(); it != m_arcs.end(); ++it) {
    it->first++;
    it->second++;
  }
  // Add new (starting) source + target for arc
  m_arcs.emplace(m_arcs.begin(), std::make_pair<std::size_t, std::size_t> (0,1));

  // Add new symbol (Epsilon)
  m_symbols.emplace(m_symbols.begin(), Epsilon);

  // Increment indices for all arcs
  for (auto it = m_stateArcs.begin(); it != m_stateArcs.end(); ++it) {
    for(auto it2 = it->begin(); it2 != it->end(); ++it2) {
      it2->setIndex(it2->index()+1);
    }
  }
  // Add new (starting) arc.
  m_stateArcs.emplace(m_stateArcs.begin(), std::vector<Arc> {Arc(this, 0)});

  // Add start state
  m_start = 0;

  // Add replacement rule for start state.
  m_rules.emplace(m_rules.begin(), std::make_pair(OtherStartOfStack, std::vector<int> {StartOfStack, OtherStartOfStack}));
}










