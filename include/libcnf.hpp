/**
 * @name libcnf.hpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 10:52:21 leo>
 * 
 * @brief Header of the libcnf library
 */

#ifndef _LIBCNF_H_
#define _LIBCNF_H_

#include <cstdint>
#include <ctime>

#include <vector>
#include <string>
#include <map>
#include <initializer_list>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <sstream>

#include "variableset.hpp"
#include "clause.hpp"
#include "formula.hpp"
#include "solver.hpp"
#include "sbox.hpp"

/**
 * This library provides an easy way to build crypto-oriented CNF
 * formulas, solve them with an external SAT-solver and use the result 
 * thus obtained.
 * 
 * Main functionnalities are:
 * + Building CNF formulas in an easy way.
 * + Solving the Formulas using an external SAT-solver and retrieving
 *   the corresponding variable assignment within the program.
 * + Automated construction of parts of the CNF and automatic 
 *   optimizations.
 *
 * All these functionnalities are provided by classes in the cnf
 * namespace.
 */
namespace cnf {

    /** Literals are represented by integers and a negative
     * integer corresponds to its negation, hence no(x) = -x. */
    inline long int no(long int x)
    {
        return (-1) * x;
    }
}


#endif // _LIBCNF_H_

