LibCNF (the Conjunctive Normal Form formula solving LIBrary)        {#mainpage}
============================================================


## Install ##

On linux, run the following command from the install directory:

    mkdir build
    cmake ..
    make
    sudo make install

## Usage ##

Simply include the header in your code and link the library during the compilation. **Warning!** This library uses the `std::initializer_list ` template so it requires the C++11 standard. Simply add `-lLibCNF -std=gnu++11` to your compilation line to take care of everything.


## Test ##

Compile the `test.cpp` in the `test` directory with `g++ test.cpp -o test -std=gnu++11 -lLibCNF` and then run the test with `./test`.

<!-- !CONTINUE! Write README. -->
