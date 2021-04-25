# FiniteAutomata

This project is used for implementing the untilities of finite automata.It restricts only to DFA and NFA and does not deal with epsilon-NFA.

#### I have used subset construction method to convert the input NFA to an equivalent DFA. 
#### Then depth first search (DFS) graph traversal algorithm is used  to find out all reachable (or accessible) states in DFA.
#### All unreachable states from DFA are removed to get an optimized DFA with less states.
#### Collapsing each group of equivalent states in DFA to a single state, and generate the resulting final minimized DFA.

### Input and corresponding output files are provided .
