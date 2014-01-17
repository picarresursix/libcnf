/**
 * @name solver.hpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 10:26:43 leo>
 *
 * @brief The header of the Solver class.
 */

#ifndef _CNF_SOLVER_H_
#define _CNF_SOLVER_H_

#include "libcnf.hpp"

namespace cnf {

/**
 * A class providing an interface to use common SAT-solver
 * (they all have the same CLI interface in theory).
 *
 * To solve a formula F, its content is flushed in a DIMACS file
 * called inputName in current directory. Then, the cnf is solved and
 * the corresponding DIMACS assignment is stored in file outputName
 * from where it is retrieved and used to assign the variables in the
 * set associated to the formula.
 */
    class Solver
    {
    protected:
        /** The name of the DIMACS file in which the cnf is stored. */
        std::string inputName;

        /** The name of the DIMACS file in which the assignment
         * of the variables is stored. */
        std::string outputName;

        /** The command to call to launch the SAT-solver. */
        std::string command;
    public:
        /** Initializes this instance. */     
        Solver(
            std::string solverName,
            std::initializer_list<std::string> options);

        /** Solves the given Formula f. If it is satisfiable, returns
         * true and assigns the variables in the VariableSet v
         * accordingly. Otherwise, returns False. */
        bool solve(Formula f, VariableSet * v);
    };
} // closing namespace

#endif
