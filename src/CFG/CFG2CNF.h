#ifndef _CFG2CNF_
#define _CFG2CNF_

#include "CFG.h"

/**
 * @brief CFG to CNF converter class.
 *
 * This class is a converter from a CFG into CNF.
 */
class CFG2CNF {
	public:
    /**
     * @brief Default constructor.
     */
		CFG2CNF() =default;

    /**
     * @brief Delete non generating variables.
     *
     * The removal of non generating variables happens in 2 steps:
     * 1) Delete all the rules with variables without bodies.
     * 2) Delete all the rules that are not reachable.
     *
     * @param cfg The CFG of which the non generating variables must be removed.
     */
		void deleteNonGenerating(CFG& cfg);

    /**
     * @brief Delete all the epsilons.
     *
     * @param cfg The CFG from which all the epsilons must be removed.
     */
		void deleteEpsilon(CFG& cfg);

    /**
     * @brief Delete all the unit productions.
     *
     * @param cfg The CFG from which all the unit productions must be removed.
     */
		void deleteUnit(CFG& cfg);

    /**
     * @brief Rename the terminals.
     *
     * Rename the terminals so that all rules containing terminals are of the form: A -> a
     *
     * @param cfg The CFG form which the terminals have to be renamed.
     */
		void renameTerminals(CFG& cfg);

    /**
     * @brief Shrink rule with variables to rules with exactly 2 variables.
     *
     * so that the rules containing variables are of the form: A -> BC.
     *
     * @param cfg The CFG from which all the rules with variables have to be checked and adjusted.
     */
		void shrinkBodysToTwoVariables(CFG& cfg);

    /**
     * @brief Overload operator ().
     *
     * @return CFG in CNF.
     */
		CFG operator() (CFG);
};








#endif //_CFG2CNF_
