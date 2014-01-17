# Time-stamp: <2014-01-17 10:31:55 leo>
# AUTHOR: Léo Perrin <leoperrin@picarresursix.fr>

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_DEFINITIONS(-Wall -pedantic -std=gnu++11 -g -O4 -mcmodel=medium)

ADD_LIBRARY(LibCNF
  src/clause.cpp
  src/formula.cpp
  src/variableset.cpp
  src/sbox.cpp
  src/solver.cpp
)

ADD_EXECUTABLE(test_cnf
  test/test.cpp
  )
TARGET_LINK_LIBRARIES(test_cnf
  LibCNF
  )

  
