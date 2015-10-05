#ifndef DOT_H
#define DOT_H

#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>

#include <UTM/Symbol.h>
#include <UTM/finiteautomaton.h>


/**
  * @brief Write out Graphviz dot file.
  *
  * @param pda The automaton.
  * @param os The output stream to write to.
  */
void writeDotFile(const FiniteAutomaton *pda, std::ostream &os) {
  os << "digraph machine {" << std::endl;
  os << "    rankdir=LR;" << std::endl;
  os << "    node [ shape = point ];" << std::endl;
  os << "    ENTRY;" << std::endl;
  os << "    node [ shape = circle];" << std::endl;
  os << "    node [ fixedsize = true, height = 0.65, shape = doublecircle ];" << std::endl;

  for (auto accept : pda->accepting()) {
    std::cout << "Added accept" << std::endl;
    os << "    " << accept.index() + 1 << ";" << std::endl;
  }
  os << "    node [ shape = circle ];" << std::endl;
  std::stringstream ss;
  for (std::size_t i = 0; i < pda->numArcs(); ++i) {
    switch (pda->arc(i).symbol()) {
      case Epsilon:
        ss << "\u03B5";
        break;
      case Any:
        ss << "any";
        break;
      default:
        ss << static_cast<char>(pda->arc(i).symbol());
        break;
    }
    ss << ",";
    switch(pda->arc(i).rule().first) {
      case StartOfStack:
        ss << "Z0";
        break;
      case OtherStartOfStack:
        ss << "X0";
        break;
      case Epsilon:
        ss << "\u03B5";
        break;
      case Any:
        ss << "any";
        break;
        default:
          ss << static_cast<char>(pda->arc(i).rule().first);
          break;
    }
    ss << "/";
    for (auto newItem : pda->arc(i).rule().second) {
      switch(newItem) {
        case StartOfStack:
          ss << "Z0";
          break;
        case OtherStartOfStack:
          ss << "X0";
          break;
        case Epsilon:
          ss << "\u03B5";
          break;
        case Any:
          ss << "any";
          break;
        default:
          ss << static_cast<char>(newItem);
          break;
      }
    }
    if ((i < pda->numArcs()-1) && pda->arc(i).source() == pda->arc(i+1).source() && pda->arc(i).target() == pda->arc(i+1).target()) {
      ss << "\\n ";
    }
    else {
      os << "    " << pda->arc(i).source().index() + 1 << " -> " << pda->arc(i).target().index() + 1 << " [ label = \"" << ss.str() << "\" ];" << std::endl;
      ss.str("");
    }
  }
  os << "    ENTRY -> " << pda->start().index() + 1 << " [ label = \"IN\" ];" << std::endl;
  os << "}" << std::endl;
}


/**
  * @brief Write out Graphviz dot file.
  *
  * @param fa The automaton.
  * @param filename The output filename to write to.
  */
void writeDotFile(const FiniteAutomaton *fa, const std::string &filename) {
  std::ofstream ofs(filename.c_str());
  writeDotFile(fa, ofs);
}

#endif
