[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/mit-pdos/mcqc)

<img src="https://elefthei.github.io/assets/css/images/mcqc.png" alt="MCQC" width="150"/>

Monadic Coq Compiler
--------------------

# Introduction

Alternative Coq compiler, by extraction to C++17. Written in Haskell.
Takes in JSON extraction (available after Coq v.8.5.1) and exports valid, performant and memory safe C++17.
See `test/numeric` and `test/cat` for examples. No GC no RTS, uses `shared_ptr` for reference
counting and a library of base types (in `include/*.hpp`).

# Building

Then with stack, run the following in the source root:
```
stack build
```

# Testing

First, cd into `classes` and run `make`.
This will generate the object files for Coq typeclasses used.

Uses `llvm-lit` for testing, installable from `pip`.
```
lit test
```

To run property-based tests of the C++ library:
```
RC=on lit test/rc
```

# Maintainer
Lef Ioannidis <elefthei@mit.edu>
