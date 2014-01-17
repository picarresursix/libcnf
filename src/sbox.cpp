/**
 * @name sbox.cpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 11:17:20 leo>
 * 
 * @brief Source code of the Sbox class.
 */

#include "../include/libcnf.hpp"

using namespace cnf;

Sbox::Sbox(unsigned int _n_input_bits,
           unsigned int _n_output_bits,
           std::initializer_list<unsigned int> output)
{
    init(_n_input_bits, _n_output_bits,
         std::vector<unsigned int>(output.begin(), output.end()));
}


Sbox::Sbox(unsigned int _n_input_bits,
           unsigned int _n_output_bits,
           std::vector<unsigned int> output)
{
    init(_n_input_bits, _n_output_bits, output);
}


void Sbox::init(unsigned int _n_input_bits,
                unsigned int _n_output_bits,
                std::vector<unsigned int> output)
{
    n_input_bits = _n_input_bits;
    input_space_size = 1;
    for (unsigned int i=0; i<n_input_bits; i++)
        input_space_size *= 2;

    n_output_bits = _n_output_bits;
    output_space_size = 1;
    for (unsigned int i=0; i<n_output_bits; i++)
        output_space_size *= 2;

    values.assign(output.begin(), output.end());

    if (values.size() != input_space_size)
        throw std::logic_error("In Sbox.init(): the lookup table's size"
                               "does not match the input size.");

    build_cnf_template();
}


void Sbox::build_cnf_template()
{
    // Building naive CNF
    for (unsigned int x=0; x<input_space_size; x++)
    {
        for (unsigned int out_bit=0; out_bit<n_output_bits; out_bit++)
        {
            std::vector<int> clause;
            for (unsigned int in_bit=0; in_bit<n_input_bits; in_bit++)
            {
                if (((x >> (n_input_bits - in_bit - 1)) & 1) == 1)
                    clause.push_back(-1);
                else
                    clause.push_back(1);
            }
            if (((values[x] >> (n_output_bits - out_bit - 1)) & 1) == 1)
                clause.push_back(out_bit+1);
            else
                clause.push_back(-out_bit-1);
            cnf_template.push_back(clause);
        }
    }

    bool finished = false;
    while (!finished)
        finished = simplify();
    
    // for (unsigned int index=0; index<cnf_template.size(); index++)
    // {
    //     for (unsigned int lit=0; lit<cnf_template[index].size(); lit++)
    //         std::cout << cnf_template[index][lit] << " ";
    //     std::cout << std::endl;
    // }

}


bool Sbox::simplify()
{
    std::vector<std::vector<int> > new_cnf;
    std::vector<bool> treated(cnf_template.size(), false);
    for (unsigned int i=0; i<cnf_template.size() - 1; i++)
        if (!treated[i])
            for (unsigned int j=i+1; j<cnf_template.size(); j++)
                if (!treated[j]
                    && cnf_template[i].back() == cnf_template[j].back())
                {
                    unsigned int n_differences = 0;
                    for (unsigned int k=0; k<n_input_bits; k++)
                        if (cnf_template[i][k] != cnf_template[j][k])
                            n_differences ++;
                    if (n_differences == 1)
                    {
                        std::vector<int> clause(n_input_bits+1, 0);
                        for (unsigned int k=0; k<n_input_bits; k++)
                            if (cnf_template[i][k] == cnf_template[j][k])
                                clause[k] = cnf_template[i][k];
                        clause[n_input_bits] = cnf_template[i].back();
                        treated[i] = true;
                        treated[j] = true;
                        new_cnf.push_back(clause);
                    }
                }

    bool has_been_modified = false;
    for (unsigned int i=0; i<cnf_template.size(); i++)
        if (!treated[i])
            new_cnf.push_back(cnf_template[i]);
        else
            has_been_modified = true;
    
    cnf_template = new_cnf;

    return has_been_modified;
}


void Sbox::add_clauses_image(
    Formula * f,
    std::vector<long int> input_bits,
    std::vector<long int> output_bits)
{
    if (input_bits.size() != n_input_bits)
        throw std::runtime_error("In Sbox.add_clauses_image: the input bit"
                                 " vector is not of the correct size.");
    if (output_bits.size() != n_output_bits)
        throw std::runtime_error("In Sbox.add_clauses_image: the output bit"
                                 " vector is not of the correct size.");

    for (unsigned int index=0; index<cnf_template.size(); index++)
    {
        Clause c;
        // adding input bits
        for (unsigned int lit=0; lit<cnf_template[index].size() - 1; lit++)
            if (cnf_template[index][lit] != 0)
                c.push_back(input_bits[lit] * cnf_template[index][lit]);

        // adding the unique output bit
        unsigned int out_bit = (cnf_template[index].back() > 0) ?
            cnf_template[index].back() - 1 :
            (-1) * cnf_template[index].back() - 1;

        if (cnf_template[index].back() > 0)
            c.push_back(output_bits[out_bit]);
        else
            c.push_back(no(output_bits[out_bit]));

        // for (unsigned int k=0; k<c.size(); k++)
        //     std::cout << c[k] << " ";
        // std::cout << std::endl;


        f->add_clause(c);
    }
    
    // exit(0); // !TO_DITCH!
    
}


void Sbox::add_clauses_image(
    Formula * f,
    std::initializer_list<long int> _input_bits,
    std::initializer_list<long int> _output_bits)
{
    add_clauses_image(
        f,
        std::vector<long int>(_input_bits.begin(), _input_bits.end()),
        std::vector<long int>(_output_bits.begin(), _output_bits.end())
        );
}

