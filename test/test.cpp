/**
 * @name test.cpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-08 14:09:27 leo>
 * 
 * @brief Tests for the libcnf library.
 */

#include <iostream>
#include <iomanip>

#include "../include/libcnf.hpp"

void test_VariableSet()
{
        std::cout << "\n---- Testing VariableSet ----" << std::endl;

        cnf::VariableSet v;
        v.add_subset("x", {2, 3});
        v.add_subset("y", {2, 2, 2});
        v.add_subset("z", {8});
        v.print_list_subsets();

        std::cout << "==== x ===" << std::endl;
        for (unsigned int i=0; i<v.subset_index_bound("x", 0); i++)
                for (unsigned int j=0; j<v.subset_index_bound("x", 1); j++)
                        std::cout << v.var("x", {i, j}) << std::endl;

        std::cout << "==== y ===" << std::endl;
        for (unsigned int i=0; i<2; i++)
                for (unsigned int j=0; j<2; j++)
                        for (unsigned int k=0; k<2; k++)
                                std::cout << v.var("y", {i, j, k}) << std::endl;

        std::cout << "==== z ===" << std::endl;
        for (unsigned int i=0; i<v.subset_index_bound("z", 0); i++)
                std::cout << v.var("z", {i}) << std::endl;

        std::cout << "\ntotal #variables = " << v.size() << std::endl;
}


void test_VariableSet_exceptions()
{
        std::cout << "\n---- Exception handling by VariableSet ----"
                  << std::endl;

        cnf::VariableSet v;
        v.add_subset("x", {2, 2});
        try
        {
                v.var("x", {1, 1, 1});
        }
        catch (std::domain_error& e)
        {
                std::cout << "domain_error thrown correctly by var()"
                          << std::endl;
        }
        try
        {
                v.var("x", {1, 3});
        }
        catch (std::out_of_range& e)
        {
                std::cout << "out_of_range thrown correctly by var()"
                          << std::endl;
        }
        try
        {
                v.subset_index_bound("x", 2);
        }
        catch (std::domain_error& e)
        {
                std::cout << "domain_error thrown correctly by subset_index_bound()"
                          << std::endl;
        }
}


void test_Clause()
{
        std::cout << "\n---- Testing Clause ----" << std::endl;

        cnf::Clause c{1, cnf::no(3), 9};
        for (unsigned int i=0; i<c.size(); i++)
                std::cout << c[i] << " ";
        std::cout << std::endl;

        for (unsigned int i=0; i<5; i++)
                if (c.variable_index(i) != -1)
                        std::cout << i << " *is* in the clause." << std::endl;
                else
                        std::cout << i << " is not in the clause." << std::endl;                        
}


void test_Formula()
{
        std::cout << "\n---- Testing Formula ----" << std::endl;
        
		cnf::VariableSet v;
        cnf::Formula f(&v);
        f.add_clause(cnf::Clause{-1});
        f.add_clauses({
                        cnf::Clause{1, -3},
                        cnf::Clause{-2, 3},
                        cnf::Clause{1, 2},
                        cnf::Clause{5, -4}
                });
        f.add_var_equality(10, 11);
        f.add_xor(20, 21, 22);
        f.to_dimacs(&std::cout, 22);
}


void test_Solver()
{
        std::cout << "\n---- Testing Solver ----" << std::endl;
        
        cnf::VariableSet v;
        cnf::Formula unsat(&v), sat(&v);
        cnf::Solver * s = new cnf::Solver("minisat", {});
        v.add_subset("x", {3});
        unsat.add_clauses({
                cnf::Clause{   v.var("x", {1}) ,   v.var("x", {2})},
                cnf::Clause{   v.var("x", {1}) ,cnf::no(v.var("x", {2}))},
                cnf::Clause{cnf::no(v.var("x", {1})),   v.var("x", {0})},
                cnf::Clause{cnf::no(v.var("x", {1})),cnf::no(v.var("x", {0}))}
                });
        sat.add_clauses({
                cnf::Clause{   v.var("x", {1}) ,   v.var("x", {2})},
                cnf::Clause{   v.var("x", {1}) ,cnf::no(v.var("x", {2}))},
                cnf::Clause{cnf::no(v.var("x", {1})),   v.var("x", {0})}
                });
                   
        if (!s->solve(unsat, &v))
                std::cout << "UNSAT correctly identified" << std::endl;
        else
                std::cout << "Problem with UNSAT formula" << std::endl;
        if (s->solve(sat, &v))
                std::cout << "SAT correctly identified" << std::endl;
        else
                std::cout << "Problem with SAT formula" << std::endl;
}


void test_Sbox()
{
        std::cout << "\n---- Testing Sbox ----" << std::endl;
        
        cnf::Sbox sbox(4, 4, {  0x5, 0xb, 0x6, 0xe, 0x8, 0x2, 0x7, 0xa,
                                0x3, 0x4, 0x0, 0xc, 0x1, 0x9, 0xf, 0xd});
        
        for (unsigned int x=0; x<16; x++)
        {
                
                cnf::VariableSet v;
                v.add_subset("in",  {4});
                v.add_subset("out", {4});
                
                cnf::Formula f(&v);
                std::vector<long int> input, output;
                for (unsigned int i=0; i<4; i++)
                {
                        input.push_back(v.var("in", {i}));
                        output.push_back(v.var("out", {i}));
                }
                sbox.add_clauses_image(&f, input, output);
                f.assign_to_integer(input, x);
                if (cnf::Solver("minisat", {}).solve(f, &v))
                {
                        std::cout << std::hex << x << " "
                                  << v.little_endian(output)
                                  << std::endl;
                }
                else
                        std::cout << "Problem with Sbox" << std::endl;
        }

}


int main(int argc, char *argv[])
{
        test_VariableSet();
        test_VariableSet_exceptions();
        test_Clause();
        test_Formula();
        test_Solver();
        test_Sbox();
        
        std::cout << std::endl;
        return 0;
}
