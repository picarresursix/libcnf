/**
 * @name formula.hpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 10:52:35 leo>
 * 
 * @brief Header of the Formula class.
 */

#ifndef _CNF_FORMULA_H_
#define _CNF_FORMULA_H_

#include "libcnf.hpp"

namespace cnf {

/** Models a CNF formula, i.e. the conjunction of an arbitrary number
 * of disjunctive clauses.
 *
 * Clauses are represented using instances of the Clause class. */
    class Formula
    {
    private:
        /** The set in which the variables used for this formula live. */
        VariableSet * v;
        
        /** Stores the clauses corresponding to this CNF. */
        std::vector<Clause> clauses;
    public:
        /** Creates an empty formula and initializes the v attribute. */
        Formula(VariableSet * _v);

        /** Adds the given clause at the end of the CNF formula. */
        void add_clause(Clause new_clause);

        /** Adds the given clauses at the end of the CNF formula. */
        void add_clauses(std::initializer_list<Clause> new_clauses);

        /** Enforces the use of a unique code for both variables, i.e. calls  */
        void add_var_equality(long int v1, long int v2);

        /** Adds clauses enforcing the the literals with the given
         * codes are equal, namely:
         * 
         * (v1 or no(v2)) and (no(v1) or v2).
         * */
        void add_var_equality_clauses(long int v1, long int v2);

        /** Adds closes enforcing that the xor of the three literals
         * given is equal to zero where true is 1 and false is 0:
         *
         * (no(v1) or v2 or v3) and (v1 or no(v2) or v3) ...
         * and (v1 or v2 or no(v3)) and (no(v1) or no(v2) or no(v3))
         * */
        void add_xor(long int v1, long int v2, long int v3);

        /** Adds one-variable clauses modeling that the variable with
         * codes given in `bits` are the little-endian representation
         * of the integer `value`. */
        void assign_to_integer(
            std::vector<long int> bits,
            uint32_t value);

        /** Adds one-variable clauses modeling that the variable with
         * codes given in `bits` are the little-endian representation
         * of the integer `value`. */
        void assign_to_integer(
            std::initializer_list<long int> bits,
            uint32_t value);

        /** Adds clauses modeling that xoring memberwise the literals
         * in `bits1` and `bits2` yields the binary expansion of
         * `cstte`.
         *
         * @throw std::runtime_error if `bits1` and `bits2` are not of
         * the same length. */
        void add_xor_with_cstte(
            std::vector<long int> bits1,
            std::vector<long int> bits2,
            uint32_t cstte);

        /** Adds clauses modeling that xoring memberwise the literals
         * in `bits1` and `bits2` yields the binary expansion of
         * `cstte`.
         *
         * @throw std::runtime_error if `bits1` and `bits2` are not of
         * the same length. */                
        void add_xor_with_cstte(
            std::initializer_list<long int> bits1,
            std::initializer_list<long int> bits2,
            uint32_t cstte);
        
        /** Prints on the given output stream a DIMACS formatted
         * representation of this CNF formula, the number of variables
         * being used in the first line of file.
         *
         * The first line (p cnf <#variables> <#clauses>) is omitted
         * since most SAT solvers don't care anyway (at least minisat
         * and glucose don't).*/
        void to_dimacs(std::ostream * out,
                       unsigned int card_variables);
    };

} // end namespace

#endif // _FORMULA_H_
