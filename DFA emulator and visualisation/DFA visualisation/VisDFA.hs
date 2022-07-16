module VisDFA (visDFA, visDFANamed)
where

import Data.List (sort, groupBy)

-- a third party Haskell package wrapping the GraphViz suite
-- of Graph Visualisation tools:
-- GraphViz tools:   https://graphviz.org/
-- GraphViz wrapper: https://hackage.haskell.org/package/graphviz
import Data.GraphViz
import Data.GraphViz.Attributes.Complete(Attribute(RankDir),RankDir(FromLeft))

import DFA



visDFA :: DFA -> IO ()
visDFA
  = visDFANamed "dfa"

visDFANamed :: String -> DFA -> IO ()
visDFANamed name dfa
  = do
      -- make sure the DFA is well-formed
      putStrLn "Checking DFA..."
      checkDFA dfa

      putStrLn "Drawing DFA..."
      -- convert the DFA to GraphViz's DOT format
      let dfaDotGraph = dfaToDot dfa
      -- run GraphViz to layout the graph and save it as
      -- an image.
      addExtension (runGraphviz dfaDotGraph) Png name
      putStrLn $ "DFA drawn and saved as '" ++ name ++ ".png'"


------------------------------------------------

dfaToDot :: DFA -> DotGraph Int
dfaToDot dfa
  = dfaToDot' dfa $ defaultParams { globalAttributes = globalAttrs
                                  , isDirected       = True
                                  , fmtNode          = customNodeFmt
                                  , fmtEdge          = customEdgeFmt
                                  }
    where
      (qs, as, ts, q0, qa) = dfa
      -- formatting:
      customNodeFmt (n, l)
        = [toLabel l, shape (stateShape n)]
        where
          stateShape n
            | n == -1     = PlainText    -- invisible pre-start state
            | n `elem` qa = DoubleCircle -- accept states
            | otherwise   = Circle       -- non-accept states
      customEdgeFmt (_, _, l)
        = [toLabel l, arrowTo vee]
      globalAttrs
        = [ GraphAttrs [RankDir FromLeft], NodeAttrs  [], EdgeAttrs  []]

dfaToDot' :: DFA -> GraphvizParams Int String String () String -> DotGraph Int
dfaToDot' dfa params = graphElemsToDot params nodes edges
  where
    (qs, as, ts, q0, qa) = dfa
    
    -- compute a list of nodes and their labels:
    -- (including invisible starting 'node' with no label)
    nodes = (-1, "") : [(q, show q)| q <- qs]

    -- compute a list of directed edges and their labels:
    -- each edge
    transitions = sort [((s, t), x) | ((s, x), t) <- ts]
    edgeGroups  = groupBy (\a b -> fst a == fst b) transitions
    edges = (-1, q0, "") : [(s, t, map snd ts) | ts@(((s,t),_):_) <- edgeGroups]
