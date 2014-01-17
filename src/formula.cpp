/**
 * @name formula.cpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 11:14:25 leo>
 *
 * @brief Source code of the Formula class.
 */

#include "../include/libcnf.hpp"

using namespace cnf;

Formula::Formula(VariableSet * _v)
{
    v = _v;
}


void Formula::add_clause(Clause new_clause)
{
    clauses.push_back(new_clause);
}


void Formula::add_clauses(std::initializer_list<Clause> new_clauses)
{
    unsigned int i = 0, start = clauses.size();
    clauses.resize(start + new_clauses.size());
    for (auto c = new_clauses.begin(); c != new_clauses.end(); c++)
    {
        clauses[start + i] = *c;
        i++;
    }
}


void Formula::add_var_equality(long int v1, long int v2)
{

    v->add_var_equality(v1, v2);

    // add_var_equality_clauses(v1, v2);
    
}


void Formula::add_var_equality_clauses(long int v1, long int v2)
{
    add_clauses({
            Clause{v1, no(v2)},
            Clause{no(v1), v2}});
}


void Formula::add_xor(long int v1, long int v2, long int v3)
{
    add_clauses({
            Clause{no(v1), v2, v3},
            Clause{v1, no(v2), v3},
            Clause{v1, v2, no(v3)},
            Clause{no(v1), no(v2), no(v3)}
        });
}


void Formula::assign_to_integer(std::vector<long int> bits,
                                uint32_t value)
{
    for (unsigned int i=0; i<bits.size(); i++)
        if (((value >> i) & 1) == 1)
            add_clause(cnf::Clause{bits[bits.size() - i - 1]});
        else
            add_clause(cnf::Clause{no(bits[bits.size() - i - 1])});
}


void Formula::assign_to_integer(std::initializer_list<long int> bits,
                                uint32_t value)
{
    assign_to_integer(std::vector<long int>(bits.begin(), bits.end()),
                      value);
}


void Formula::add_xor_with_cstte(
    std::vector<long int> bits1,
    std::vector<long int> bits2,
    uint32_t cstte)
{
    if (bits1.size() != bits2.size())
        throw std::runtime_error(
            "In add_xor_with_cstte(): input literal vectors must "
            "be of the same length!");
    for (unsigned int i=0; i<bits1.size(); i++)
        if (((cstte >> (bits1.size() - i - 1)) & 1) == 1)
            add_var_equality(bits1[i], no(bits2[i]));
        else
            add_var_equality(bits1[i], bits2[i]);
}


void Formula::add_xor_with_cstte(
    std::initializer_list<long int> bits1,
    std::initializer_list<long int> bits2,
    uint32_t cstte)
{
    add_xor_with_cstte(
        std::vector<long int>(bits1.begin(), bits1.end()),
        std::vector<long int>(bits2.begin(), bits2.end()),
        cstte
        );
}


void Formula::to_dimacs(
    std::ostream * out,
    unsigned int card_variables)
{
    (*out) << "p cnf " << card_variables
           << " " << clauses.size()
           << "\n";
    for (unsigned int i=0; i<clauses.size(); i++)
    {
        for (unsigned int j=0; j<clauses[i].size(); j++)
            (*out) << v->new_code(clauses[i][j]) << " ";
        (*out) << "0\n";
    }
    out->flush();
}
