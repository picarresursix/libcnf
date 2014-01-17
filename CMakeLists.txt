# Time-stamp: <2014-01-17 10:31:55 leo>
# AUTHOR: Léo Perrin <leoperrin@picarresursix.fr>

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_DEFINITIONS(-Wall -pedantic -std=gnu++11 -g -O4 -mcmodel=medium)

ADD_LIBRARY(LibCNF STATIC
  src/clause.cpp
  src/formula.cpp
  src/variableset.cpp
  src/sbox.cpp
  src/solver.cpp
)

INSTALL(TARGETS LibCNF DESTINATION lib)
INSTALL(FILES
  include/clause.hpp
  include/formula.hpp
  include/variableset.hpp
  include/sbox.hpp
  include/solver.hpp
  include/libcnf.hpp
  DESTINATION include)


#ADD_EXECUTABLE(test_cnf
#  test/test.cpp
#  )
#TARGET_LINK_LIBRARIES(test_cnf
#  LibCNF
#  )

  
