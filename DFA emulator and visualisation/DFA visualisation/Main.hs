import DFA    -- defines our DFA type as in the previous problems
import VisDFA -- provides a function to generate an image of your DFA

-- define some DFA here, or copy over one from
-- a previous problem

dfa :: DFA
dfa
  = ([0,1,2], "ab", ts, 0, [1])
    where
      ts = [ ((0, 'a'), 1)
           , ((0, 'b'), 2)
           , ((1, 'a'), 1)
           , ((1, 'b'), 1)
           , ((2, 'a'), 2)
           , ((2, 'b'), 2)
           ]


-- Then press 'Run' and run the command 'visDFA dfa' to see
-- a drawing of your DFA!