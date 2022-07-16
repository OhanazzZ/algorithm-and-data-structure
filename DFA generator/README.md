# DFA generator
a Haskell function 
```Haskell
multiples :: Int -> DFA
```
so that
```Haskell
multiples n (n > 0)
```
produces a DFA (deterministic final-state automata) with alphabet {0, 1} that recognises the language $M_n$ of binary strings representing natural numbers that are multiples of n