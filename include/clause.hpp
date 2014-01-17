/**
 * @name clause.hpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-15 13:51:52 leo>
 * 
 * @brief Header of the Clause class.
 */

#ifndef _CNF_CLAUSE_H_
#define _CNF_CLAUSE_H_

#include "libcnf.hpp"

namespace cnf {

/**
 * The Clause class models a disjunctive clause, i.e. the logic "or"
 * of plenty of literals. A literal can be a variable or its
 * negation. Literals are represented by the code of the corresponding
 * variable (an integer) or its opposite if the literal is the
 * negation of the variable.
 */
    class Clause
    {
    private:
        /** Stores the codes of the literals in this clause. */
        std::vector<long int> literals;
    public:
        /** Builds an empty clause. */
        Clause();
        
        /** Builds a clause containing the given literals. */
        Clause(std::initializer_list<long int> lit_codes);

        /** Returns the i-th literal of the clause. */
        const long int& operator[](const unsigned int i) const;

        /** If the given (possibly negated) variable is in the clause,
         * returns its index within the clause. Otherwise, returns
         * -1. */
        int variable_index(long int x);

        /** Returns the number of literals in the clause. */
        unsigned int size();

        /** Adds a new literal to the clause. */
        void push_back(long int x);
    };

} // end namespace

#endif // _CLAUSE_H_
