-------------------------------------------------------------------------
--
--  Regular expressions as structure trees in Haskell
--
-------------------------------------------------------------------------

data RegExp
  = Sym Char              -- Basic symbol (for simplicity, assume digits)
  | Eps                   -- Epsilon, the regular expression with language {ε}
  | Empty                 -- The regular expression with language {}
  | Concat RegExp RegExp  -- Concatenation
  | Union RegExp RegExp   -- Union
  | Star RegExp           -- Kleene star
  deriving (Eq,Show)

-------------------------------------------------------------------------
--
--  Parsing
--
-------------------------------------------------------------------------

--  Here is a context-free grammar for the input language:
--
--    s  ->  p s'                    (sums)
--    p  ->  f p'                    (products)
--    f  ->  b f'                    (factors, possibly starred)
--    b  ->  digit | E | e | ( s )   (basic factors)
--    s' ->  eps | + p s'
--    p' ->  eps | f p'
--    f' ->  eps | * f'
--
--  For simplicity we assume symbols are single digits.
--
--  A parser takes a list of characters. The parsers for s, p, f and b
--  consume characters greedily in an attempt to recognise well-formed
--  regexps of syntactic category s, p, f and b, respectively. When one of
--  these parsers finds such a (sub-)regexp r, it returns r, together with
--  the remaining (un-consumed) characters.
--  The parsers for s', p' and f' find snippets such as "+ ..." and "*",
--  that is, operations that have to be applied to an already parsed regexp.
--  These parsers should therefore return functions, or regexp transformers.
--  For example, the parser for f' checks whether an expression found to be
--  a valid b (a "basic factor") has a Kleene star attached to it. The parser
--  produces the wrapper 'Star' if a star is attached, otherwise no wrapper
--  (that is, it uses the identity function id as a wrapper).
--  These differences explain the use of a type variable in the Parser type
--  below. A parser of type `Parser a' is a parser returning objects of
--  type `a' (in addition to the remaining un-consumed characters).

type Parser a = String -> (a, String)

digits
  = ['0'..'9']

parse :: String -> RegExp
parse input
  | not (null bad_chars) = error ("Bad symbol(s) found: " ++ bad_chars)
  | null remaining_input = r
  | otherwise            = error ("Trailing garbage: " ++ show remaining_input)
    where
      input1               = filter (`notElem` " \t") input
      bad_chars            = filter (not.valid) input1
      valid c              = c `elem` (digits ++ "Ee+*()")
      (r, remaining_input) = parse_s input1

-------------------------------------------------------------------------

--  We name the individual parsers after their syntactic categories.
--  The parser parse_s recognises an arbitrary regular expression.

parse_s, parse_p, parse_f :: Parser RegExp

parse_s input
  = (wrapper r, input2)
    where
      (r, input1)       = parse_p input
      (wrapper, input2) = parse_s' input1

parse_p input
  = (wrapper r, input2)
    where
      (r, input1)       = parse_f input
      (wrapper, input2) = parse_p' input1

-- Now is the time to complete the function parse_f
-- (and parse_f' down at the bottom of this file), both
-- of which are currently incomplete.
parse_f input
  = (wrapper r, input2)
    where
      (r, input1)       = parse_b input
      (wrapper, input2) = parse_f' input1

-- The parser for b is incomplete, you should copy over your
-- completed version from the previous problem.
parse_b ('(' : input)
  = (r, consume_right_paren input1)
    where
      (r, input1) = parse_s input
      consume_right_paren s
        | null s        = error "Expected ')', found end of input"
        | head s /= ')' = error ("Expected ')', found '" ++ s ++ "'")
        | otherwise     = tail s
parse_b (c:cs)
  | c `elem` digits = (Sym c, cs)
  | c == 'e'        = (Eps  , cs)
  | c == 'E'        = (Empty, cs)
  | otherwise
      = error ("Expected E, e, '(' or digit, found '" ++ (c:cs) ++ "'")

parse_s', parse_p', parse_f' :: Parser (RegExp -> RegExp)

parse_s' ('+' : input)
  = (\r -> Union r (wrapper r1), input2)
    where
      (r1, input1)      = parse_p input
      (wrapper, input2) = parse_s' input1
parse_s' input
  = (id, input)

parse_p' input
  | null input             = (id, [])
  | head input `elem` "+)" = (id, input)
  | otherwise              = (\r -> Concat r (wrapper r1), input2)
    where
      (r1, input1)      = parse_f input
      (wrapper, input2) = parse_p' input1

-- this function is currently unused and incomplete, you
-- should fix it and use it in your definition for parse_f
parse_f' ('*' : input)
  = (\r -> Star (wrapper r) , input1)
    where
      (wrapper, input1) = parse_f' input
parse_f' input
  = (id, input)
