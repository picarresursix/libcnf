/**
 * @name variableset.hpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 11:06:43 leo>
 * 
 * @brief Header of the VariableSet class.
 */

#ifndef _CNF_VARIABLESET_H_
#define _CNF_VARIABLESET_H_

#include "libcnf.hpp"

namespace cnf {

/**
 * This class provides an easy way to give every literal a unique
 * code, assign values to them from the DIMACS output of a SAT-solver;
 * all while using precisely as much numbers as necessary.
 *
 * Suppose you have modelled your problem in such a way that you have
 * to sets of variables: x_i,j and y_l,m,n where i,l are in [0,6]
 * (thus taking 7 different values) and j,m,n are in [0, 2] (thus
 * taking 3 values). Then you should use the following code to create
 * the corresponsing set of variable:
 *
 * ```
 * VariableSet v;
 * v.add_subset("x", {7, 3});
 * v.add_subset("y", {7, 3, 3});
 * ```
 *
 * In order to retrieve the index of x_0,0 and y_5,2,1, you then
 * simply need:
 *
 * ```
 * code_x_0_0 = v.var("x", {0, 0});
 * code_y_5_2_1 = v.var("y", {5, 2, 1});
 * ```
 *
 * Each variable subset (x and y in the previous example) is given an
 * a number when add_subset is called; here 0 for x and 1 for y. These
 * numbers are stored in the map `subset_indices`. The dimensions of
 * the subsets are stored as vectors of unsigned int ({7,3} for x,
 * {7,3,3} for y) which are themselves stored in the
 * `subset_dimensions` vector.
 *
 * Each variable subset is allocated an interval of strictly positive
 * integers. Here, x has [1, 21] (7*3 = 21) and y has [22, 84]
 * (7*3*3+21 = 84). A hypothetical third subset would then start at
 * 84. To allow a fast computation of the index of each variable, the
 * `subset_cumulated_sizes` vector stores at position `i` the sum of
 * the sizes of the subsets of indices smaller than `i`. Here, it
 * would contain: [1, 22, 85].
 *
 * The 0 index is not allowed for two reasons. First, -0=0 so it
 * cannot be "negated". Second, in the DIMACS format, 0 corresponds to
 * the end of a clause so it is not a valid index either.
 *
 * Finally, variables value can be read from a DIMACS file. Their
 * values are stored in the `values` vector of bool and the value of
 * each variable can be retrieved using the `value` method.
 */
    class VariableSet
    {
    private:
        /** Used to store the correspondance between a subset name and
         * its index*/
        std::map<std::string, unsigned int> subset_indices;

        /** Used to store the dimensions of the different variable
         * subsets. */
        std::vector<std::vector<unsigned int> > subset_dimensions;

        /** Used to store the total size of the union of the subsets
         * having an index smaller than a constant. */
        std::vector<unsigned int> subset_cumulated_sizes;

        /** The values of the variables (known). */
        std::vector<bool> values;
        
        /** Is true if and only if the variables have received an
         * assignment. */
        bool vars_are_assigned;

        /** Stores the correspondance between variables known to be
         * equal. */
        std::map<long int, long int> var_equalities;
        
    public:
        /** Methods to build a variable set.*/
        ///@{
        
        /** Builds an empty variable set. */
        VariableSet();

        /** Adds a subset of variable with the given name and
         * dimensions. If dim = {3, 5} then the possible indices are
         * in [0,2]x[0,4] (3 possible values for the first, 5 for the
         * second).*/
        void add_subset(
            std::string name,
            std::initializer_list<unsigned int > dim);

        /** Adds a new subset of variables using a new name chosen at
         * random and having the given dimensions.
         *
         * @return The name of the subset just added. */
        std::string add_subset(std::initializer_list<unsigned int > dim);

        ///@}
        /** Accessing the variables' codes. */
        ///@{
        
        /** Returns the code of the variable wanted, i.e. with the
         * specified name and coordinates/indices.
         *
         * Raises an exception if the coordinates are not valid.
         * 
         * @throw std::domain_error if there are too many coordinates.
         * @throw std::out_of_range if one of the coordinate is too
         * large */
        long int var(
            std::string name,
            std::initializer_list<unsigned int > coord);

        /** Returns the code to be used for this variable knowing that
         * it may be equal to other variables. */
        long int new_code(long int old_code);

        /** Returns the code of the variable with given name and
         * coordinates by taking into account that it may be equal to
         * other variables.  */
        long int new_code(
            std::string old_name,
            std::initializer_list<unsigned int> old_coord);

        /** Tells the variable set that the two variables given are
         * equal so that a unique code should be used for both of
         * them. */
        void add_var_equality(long int x1, long int x2);
        
        ///@}
        /** Assigning the variables and using the result. */
        ///@{
             
        /** Parses a DIMACS file and Returns true if the given stream
         * corresponds to a satisfying assignment and assigns the
         * variables accordingly, i.e. sets values[i] to true if i is
         * in the DIMACS. The `vars_are_assigned` is also set to true.
         *
         * If the file does not correspond to a satisfying assignment,
         * returns false.
         *
         * Note that the VariableSet instance's must be set within the
         * program. For instance, the following fails and throws a
         * std::logic_error exception:
         *
         * ```
         * VariableSet v;
         * v.parse_dimacs(&some_stream);
         * ```
         * 
         * but this works:
         * ```
         * VariableSet v;
         * v.add_subset("x", {10, 10});
         * v.parse_dimacs(&some_stream);
         * ```
         *
         * @throw std::logic_error if the VariableSet instance
         * contains no subset.
         */
        bool parse_dimacs(std::istream * input);

        /**
         * Returns the value of the variable with the given names
         * and coordinates.
         *
         * @throw std::logic_error if the variables have not been
         * assigned yet. */
        bool value(
            std::string name,
            std::initializer_list<unsigned int > coord);

        /** Returns the little endian representation of the brace
         * enclosed list of bits corresponding to the assignment of
         * the variables given as the input.
         *
         * @throw std::logic_error if the variables have not been
         * assigned yet. */
        uint32_t little_endian(std::initializer_list<long int> vars);
        
        /** Returns the little endian representation of the vector of
         * bits corresponding to the assignment of the variables given
         * as the input.
         *
         * @throw std::logic_error if the variables have not been
         * assigned yet. */
        uint32_t little_endian(std::vector<long int> vars);
        
        ///@}
        /** Accessing data about the variable set */
        ///@{
                    
        /** Prints on stdout the list of the subsets, their indices
         * and their dimensions.
         *
         * Sample output:
         * y 1 { 7 3 3}
         * x 0 { 7 3}
         */
        void print_list_subsets();

        /** Return the total size of the variable set, i.e. the sum of
         * the sizes of the subsets. */
        unsigned long int size();

        /** Returns the upper-bound of the n-th index of the variable
         * with the given name.
         *
         * Example:
         * ```
         * v.add_subset("x", 7, 3);
         * i = v.subset_index_bound(0)
         * j = v.subset_index_bound(1)
         * ```
         *
         * Here, i=7, j=3.
         *
         * @throw std::domain_error if n is too large. */
        unsigned int subset_index_bound(
            std::string name,
            unsigned int n);

        ///@}
    };


} // Closing namespace

#endif // _VARIABLESET_H_
