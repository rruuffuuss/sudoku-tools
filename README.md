## An experimental sudoku solving algorithm I created to compare against [The fastest sudoku solver](https://codegolf.stackexchange.com/questions/190727/the-fastest-sudoku-solver)

A cell with _n_ candidate values is stored as an integer equal to 

### $` \displaystyle\sum_{i=0}^{n}2^{candidate_{i}}`$

(In other words, if 5, 6 and 7 are the candidate values for a cell, the 5th, 6th and 7th bits will be set to 1)

Hidden subsets and naked subsets can be exposed efficiently with aggregate bitwise operations.\
Eliminated candidates can be efficiently removed with masks produced by exposed subsets.


I'm looking at using constexpr to generate permutations at compile time to speed things up
