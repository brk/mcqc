(**
    RUN: %coqc %s
    RUN: %clean
    RUN: %mcqc H1.json -o %t.cpp
    RUN: %FC %s -check-prefix=CPP < %t.cpp
    RUN: %clang -c %t.cpp

    CPP: #include "nat.hpp"
    CPP: template<{{typename|class}} [[TT:.?]]>
    CPP: nat onEvens([[TT]] f, nat n)
*)

Fixpoint isEven(n: nat) :=
  match n with
    | 0 => true
    | 1 => false
    | S(S m as sm) => isEven m
  end.

Definition onEvens (f : nat -> nat) (n: nat) :=
  match (isEven n) with
    | true => f n
    | false => n
  end.

Require Extraction.
Extraction Language JSON.
Separate Extraction onEvens.
