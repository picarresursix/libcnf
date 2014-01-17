/**
 * @name sbox.hpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 10:40:45 leo>
 * 
 * @brief The header of the Sbox class.
 */

#ifndef _CNF_SBOX_H_
#define _CNF_SBOX_H_

#include "libcnf.hpp"

namespace cnf {

    /** Provides an easy way to state that a set of boolean variables
     * correspond to the image of another set of boolean variables
     * by a Sbox.
     *
     * It is assumed that the sbox maps a set [0, 2^m-1] to [0,
     * 2^n-1].
     *
     * The complexity of this implementation is exponential in the
     * size of the input of the S-box so you might want to stay
     * away from it for larger S-boxes.
     *
     * Simplifications in the CNF modeling that some bits are the
     * image of some others are performed during the construction of
     * this instance.
     */
    class Sbox
    {
    private:
        /** Stores the outputs of the Sbox. */
        std::vector<uint16_t> values;

        /** The number of input bits. */
        unsigned int n_input_bits;

        /** The number of output bits. */
        unsigned int n_output_bits;

        /** The size of the input space. */
        unsigned int input_space_size;
        
        /** The size of the output space. */
        unsigned int output_space_size;

        /** Stores the structure of the CNF modeling this  */
        std::vector<std::vector<int> > cnf_template;
        

    public:
        /** Calls the init method with the given args. */
        Sbox(
            unsigned int _n_input_bits,
            unsigned int _n_output_bits,
            std::initializer_list<unsigned int> output);

        /** Calls the init method with the given args. */
        Sbox(
            unsigned int _n_input_bits,
            unsigned int _n_output_bits,
            std::vector<unsigned int> output);

        /** Initializes the Sbox instance from the number of input and output
         * bits and the look-up table representation of the Sbox.
         *
         * @throw std::logic_error if the size of the output does not
         * match the input size.*/
        void init(
            unsigned int _n_input_bits,
            unsigned int _n_output_bits,
            std::vector<unsigned int> output);

        void build_cnf_template();
        bool simplify();
                
        /** Adds to the given formula the clauses modelling that a set
         * of bit is the image of an other set of bit by this S-box.
         *
         * @throw std::runtime_error if the size of the input or the
         * output vector don't match the expected ones. */
        void add_clauses_image(
            Formula * f,
            std::vector<long int> input_bits,
            std::vector<long int> output_bits);
                
        /** Calls add_clauses_image() with the std::initializer_list
         * turned into vectors. */
        void add_clauses_image(
            Formula * f,
            std::initializer_list<long int> input_bits,
            std::initializer_list<long int> output_bits);
    };
        
} // end namespace

#endif // _SBOX_H_ */
