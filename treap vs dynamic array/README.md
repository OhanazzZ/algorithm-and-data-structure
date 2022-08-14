# An Experimental Study on Treap

This repository documents an experimental comparison between Treap and Dynamic Array and their performance under operation sequences of different lengths and operation types.  

View the .html file for convenience, use .ipynb for reproduction

The notebook implements:
* Data generator
* Sequence generator
* Randomised Treap
* Dynamic Array as a competitor 

The notebook conducts experiments under the following settings:
* Experiment 1: Time v.s. Number of Insertions
    * Sequence length $\in$ {0.1M, 0.2M, 0.5M, 0.8M, 1M}, where $M$ = one million

* Experiment 2: Time v.s. Deletion Percentage.
    * Sequence length = $1M$  
    * $prob_{delete}$ $\in$ {0.1%, 0.5%, 1%, 5%, 10%}, $prob_{insert}$ = 1 - $prob_{delete}$

* Experiment 3: Time v.s. Search Percentage.
    * Sequence length = $1M$
    * $prob_{search} \in$ {0.1%, 0.5%, 1%, 5%, 10%}, $prob_{insert} = 1 - prob_{search}$

* Experiment 4: Time v.s. Length of Mixed Operation Sequence
    * Sequence length $\in$ {0.1M, 0.2M, 0.5M, 0.8M, 1M}, where $M$ = one million
    * $prob_{delete}=5\%, prob_{search}=5\%, prob_{insert}=90\%$
