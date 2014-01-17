/**
 * @name clause.cpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 11:11:30 leo>
 * 
 * @brief Source code of the Clause class.
 */

#include "../include/libcnf.hpp"

using namespace cnf;


Clause::Clause()
{
}


Clause::Clause(std::initializer_list<long int> lit_codes)
{
    unsigned int i = 0;
    literals.resize(lit_codes.size());
    for (auto lit = lit_codes.begin(); lit != lit_codes.end(); lit++)
    {
        literals[i] = *lit;
        i ++;
    }
}

const long int& Clause::operator[](const unsigned int i) const
{
    return literals[i];
}


int Clause::variable_index(long int x)
{
    for (unsigned int i=0; i<literals.size(); i++)
        if (literals[i] == x || literals[i] == -x)
            return i;
    return -1;
}


unsigned int Clause::size()
{
    return literals.size();
}


void Clause::push_back(long int x)
{
    literals.push_back(x);
}
