/**
 * @name solver.cpp
 * @author Leo "picarresursix" Perrin <leoperrin@picarresursix.fr>
 * @date Time-stamp: <2014-01-17 10:34:36 leo>
 * 
 * @brief The source code of the Solver class.
 */

#include "../include/libcnf.hpp"

using namespace cnf;


Solver::Solver(std::string solverName,
               std::initializer_list<std::string> options)
{
    std::stringstream input, output;
    input << solverName << "-in-" << time(NULL) << ".dim";
    output << solverName << "-out-" << time(NULL) << ".dim";
    inputName  = input.str();
    outputName = output.str();
    command = solverName;
    for (auto opt = options.begin(); opt != options.end(); opt ++)
        command += " " + (*opt);
}


bool Solver::solve(Formula f, VariableSet * v)
{
    std::ofstream dimacsInput;
    dimacsInput.open(inputName);
    f.to_dimacs(&dimacsInput, v->size());

    std::string fullCommand =
        command + " " + inputName + " " +
        outputName + " 2>/dev/null 1>/dev/null";

    if (system(fullCommand.c_str()) == -1)
        throw std::runtime_error(
            "The SAT-solver did not run correcly.");

    std::ifstream dimacsOutput;
    dimacsOutput.open(outputName);
    return v->parse_dimacs(&dimacsOutput);
}



