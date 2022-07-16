import DFA    -- hidden module, defines DFA types as in worksheet 3
import RunDFA -- hidden module, contains a completed DFA emulator
import VisDFA -- hidden module, contains DFA visualisation tools
import EqDFA  -- hidden module, required for testing purposes

-- here's a general solution that does not produce minimal DFAs:

-- multiples' n
-- General algorithm to build a DFA for the language
-- M_n for any n, where:
-- 
--   M_n = { w | w is a binary number divisible by n }
-- 
-- Idea: Use states 0, 1, ..., n-1. If the input-so-far
-- is a binary number B, then we should be in state
-- B `mod` n.
-- Appending a zero to a binary number multiplies this
-- remainder by 2, and appending a one multiplies it by
-- 2 and then adds 1. So, we should go from state i to
-- state 2i (mod n) on a '0' and to state 2i+1 (mod n)
-- on a '1'.
multiples' :: Int -> DFA
multiples' n
  = (states, "01", delta, 0, [0])
  where
    states
      = [0..n-1]
    delta
      =  [ ((i, '0'), (2*i)   `mod` n) | i <- states ]
      ++ [ ((i, '1'), (2*i+1) `mod` n) | i <- states ]

-- here's the same solution, implemented with a neat trick:
-- if you view the states in a circle, the transitions just
-- walk around the circle, so we can make delta using zip and
-- cycle:
multiples'' :: Int -> DFA
multiples'' n
  = (states, symbols, delta, 0, [0])
  where
    states  = [0..n-1]
    symbols = "01"
    inputs  = [(s, x) | s <- states, x <- symbols]
    delta   = zip inputs (cycle states)


-- and here's a general solution that does produce minimal DFAs:

-- multiples n
-- Construct a minimal DFA for the language M_n:
-- 
--   M_n = { w | w is a binary number divisible by n }
-- 
-- Idea: We can recognise such a number in two parts.
-- First, a prefix that is a binary number divisible by
-- an odd number `b`, and then a suffix consisting of
-- `a` zeros, where
-- 
--   n = b * 2^a
--
-- The resulting DFA will be minimal.
multiples :: Int -> DFA
multiples n
  | a == 0    = (states, "01", deltaOdd,  0,   [0])
  | b == 1    = (states, "01", deltaTwo,  end, [end])
  | otherwise = (states, "01", deltaBoth, end, [end])
  where
    -- determine `a` and `b` (defined above) by counting how many
    -- factors of 2 fit into n (see helper function `howEven`).
    (a, b) = howEven n
    -- in any case, we'll need precisely a+b states (start from 0)
    end = a+b-1
    states = [0..end]
    -- in the case where n is odd (a=0) we'll only have a prefix
    -- loop:
    deltaOdd
      =  [ ((i, '0'), (2*i) `mod` n)   | i <- [0..end] ]
      ++ [ ((i, '1'), (2*i+1) `mod` n) | i <- [0..end] ]
    -- in the case where n is a power of two (b=1) we'll only
    -- have a suffix chain:
    deltaTwo
      =  [ ((i, '0'), i+1) | i <- [0..end-1] ] ++ [ ((end, '0'), end) ]
      ++ [ ((i, '1'), 0)   | i <- [0..end] ]
    -- in the general case, n contains both a non-zero power of
    -- two and a non-one odd factor; we'll need both a prefix loop
    -- and a suffix chain! allocate states as follows:
    -- * use states 0..b-1 to form a loop for recognising valid
    --   prefixes (binary strings divisible by `b`) using the
    --   simple technique from the `multiples'` function above.
    -- * use states 0 (again) and another `a` states (b..a+b-1)
    --   in a chain to recognise a suffix of `a` zeros
    --   note that the chain starts when the prefix is exactly
    --   divisible by `b`, from state 0, and that all states in
    --   the chain will lead back into the prefix loop on a one
    --   (in particular, to state 1).
    -- * the state at the end of this chain, `a+b-1`, is the
    --   start state and the accept state.
    deltaBoth
      -- construct the prefix loop just like in a normal machine
      =  [ ((i, '0'), (2*i) `mod` b)   | i <- [1..b-1] ]
      ++ [ ((i, '1'), (2*i+1) `mod` b) | i <- [1..b-1] ]
      -- from the zero-remainder point, proceed to count zeros in
      -- the suffix chain, or return to state 1 in the prefix loop
      -- on a one
      ++ [ ((0, '0'), b), ((0, '1'), 1) ]
      ++ [ ((i, '0'), i+1) | i <- [b..end-1] ] ++ [ ((end, '0'), end) ]
      ++ [ ((i, '1'), 1)   | i <- [b..end] ]

-- howEven n
-- Recursively divide n by 2 until only an odd number
-- remains; return a tuple containing the number of
-- divisions performed and the resultant odd number
howEven :: Int -> (Int, Int)
howEven n
  = until (odd.snd) (\(a, b) -> (a+1, b `div` 2)) (0, n)