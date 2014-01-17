/**
 * @name variableset.cpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 11:06:31 leo>
 * 
 * @brief Source code of the VariableSet class
 */

#include "../include/libcnf.hpp"

using namespace cnf;


// !SECTION! Building the variable set
// ===================================

VariableSet::VariableSet()
{
    vars_are_assigned = false;
    subset_cumulated_sizes.push_back(0);
}
        

void VariableSet::add_subset(std::string name,
                             std::initializer_list<unsigned int > dim)
{
    subset_indices[name] = subset_indices.size() - 1;

    std::vector<unsigned int> subset_dim;
    unsigned int
        i = 0,
        total_dim = 1;
    subset_dim.resize(dim.size());
    for (auto d = dim.begin(); d != dim.end(); d++)
    {
        subset_dim[i] = *d;
        total_dim *= subset_dim[i];
        i ++;
    }
    subset_dimensions.push_back(subset_dim);
    subset_cumulated_sizes.push_back(
        total_dim + subset_cumulated_sizes.back());
}


std::string VariableSet::add_subset(std::initializer_list<unsigned int > dim)
{
    std::stringstream name_stream;
    name_stream << "__random__" << subset_indices.size() + 1;
    std::string name = name_stream.str();
    add_subset(name, dim);
    return name;        
}


// !SECTION! Accessing the variables' codes
// ========================================

long int VariableSet::var(std::string name,
                          std::initializer_list<unsigned int > coord)
{
    unsigned int index = subset_indices[name];
    if (coord.size() != subset_dimensions[index].size())
        throw std::domain_error(
            "Too much coordinates for VariableSet.var().");
    unsigned int i = 0;
    long int code = 0;
    for (auto c = coord.begin(); c != coord.end(); c++)
    {
        if (*c >= subset_dimensions[index][i])
        {
            std::stringstream msg;
            msg << "Coordinate " << i
                << " for VariableSet.var() is too large ("
                << (*c) << " >= " << subset_dimensions[index][i] << ").";
            throw std::out_of_range(msg.str());
        }
        if (i == 0)
            code = *c;
        else
            code = *c + subset_dimensions[index][i]*code;
        i++;
    }
    code += subset_cumulated_sizes[index] + 1;

    return code;
}


long int VariableSet::new_code(long int old_code)
{
    if (var_equalities[old_code] == 0)
        return old_code;
    else
        return new_code(var_equalities[old_code]);
}


long int VariableSet::new_code(
    std::string old_name,
    std::initializer_list<unsigned int> old_coord)
{
    return new_code(var(old_name, old_coord));
}


void VariableSet::add_var_equality(long int x1, long int x2)
{
    long int
        to_compare_1 = (x1 > 0) ? x1 : (-1)*x1,
        to_compare_2 = (x2 > 0) ? x2 : (-1)*x2;
    if (to_compare_1 > to_compare_2)
    {
        var_equalities[x1] = x2;
        var_equalities[no(x1)] = no(x2);
    }
    else
    {
        var_equalities[x2] = x1;
        var_equalities[no(x2)] = no(x1);
    }
}

// !SECTION! Assigning the variables and using the result
// ======================================================

bool VariableSet::parse_dimacs(std::istream * input)
{
    if (!(*input))
        throw std::runtime_error("Cannot parse broken input!");
    else
    {
        std::string res;
        (*input) >> res;
        if (res.compare("SAT") != 0)
            return false;
        else
        {
            values.assign(size(), false);
            long int i;
            while ( ((*input) >> i) && (i!=0) )
            {
                if (i>0)
                    values[i-1] = true;
                else
                    values[-(i-1)] = false;
            }
            for (unsigned int k=1; k<=values.size(); k++)
                if (new_code(k) > 0)
                    values[k-1] = values[new_code(k)-1];
                else
                    values[k-1] = !values[(-1)*new_code(k)-1];

            vars_are_assigned = true;
            return true;
        }
    }
}


bool VariableSet::value(
    std::string name,
    std::initializer_list<unsigned int > coord)
{
    if (!vars_are_assigned)
        throw std::logic_error(
            "Cannot return value of un-assigned variable.");
    else
        return values[var(name, coord) - 1];
}


uint32_t VariableSet::little_endian(std::initializer_list<long int> vars)
{
    return little_endian(std::vector<long int>(vars.begin(), vars.end()));
}


uint32_t VariableSet::little_endian(std::vector<long int> vars)
{
    uint32_t res = 0;
    for (auto b = vars.begin(); b != vars.end(); b++)
    {
        res <<= 1;
        if (values[*b - 1])
            res |= 1;
        // else if (*b < 0 && ~values[(-1)*(*b) - 1])
        //     res |= 1;
    }
    return res;
}


// !SECTION! Accessing data about the variable set
// ===============================================

void VariableSet::print_list_subsets()
{
    for (auto key = subset_indices.begin();
         key != subset_indices.end();
         key ++)
    {
        std::cout << key->first << " "
                  << key->second << " {";
        for (unsigned int i=0;
             i<subset_dimensions[key->second].size();
             i++)
            std::cout << " " << subset_dimensions[key->second][i];
        std::cout << "}" << std::endl;

    }
}


unsigned long int VariableSet::size()
{
    return subset_cumulated_sizes.back();
}


unsigned int VariableSet::subset_index_bound(
    std::string name,
    unsigned int n)
{
    if (n >= subset_dimensions[subset_indices[name]].size())
        throw std::domain_error(
            "Index of coordinate too large in VariableSet.subset_index_bound.");
    return subset_dimensions[subset_indices[name]][n];
}

