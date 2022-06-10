{- | CSE 116: All about fold.

     For this assignment, you may use the following library functions:

     length
     append (++)
     map
     foldl'
     foldr
     unzip
     zip
     reverse

  Use www.haskell.org/hoogle to learn more about the above.

  Do not change the skeleton code! The point of this assignment
  is to figure out how the functions can be written this way
  (using fold). You may only replace the `error "TBD:..."` terms.

-}

module Hw3 where

import Prelude hiding (replicate, sum)
import Data.List (foldl')

foldLeft :: (a -> b -> a) -> a -> [b] -> a
foldLeft = foldl'

--------------------------------------------------------------------------------
-- | sqSum [x1, ... , xn] should return (x1^2 + ... + xn^2)
--
-- >>> sqSum []
-- 0
--
-- >>> sqSum [1,2,3,4]
-- 30
--
-- >>> sqSum [(-1), (-2), (-3), (-4)]
-- 30

sqSum :: [Int] -> Int
sqSum xs = foldLeft f base xs
  where
   f a x = a + (x * x)
   base  = 0

--------------------------------------------------------------------------------
-- | `pipe [f1,...,fn] x` should return `f1(f2(...(fn x)))`
--
-- >>> pipe [] 3
-- 3
--
-- >>> pipe [(\x -> x+x), (\x -> x + 3)] 3
-- 12
--
-- >>> pipe [(\x -> x * 4), (\x -> x + x)] 3
-- 24

pipe :: [(a -> a)] -> (a -> a)
pipe fs   = foldLeft f base fs
  where
    f a x = a.x
    base  = (\e -> e)

--------------------------------------------------------------------------------
-- | `sepConcat sep [s1,...,sn]` returns `s1 ++ sep ++ s2 ++ ... ++ sep ++ sn`
--
-- >>> sepConcat "---" []
-- ""
--
-- >>> sepConcat ", " ["foo", "bar", "baz"]
-- "foo, bar, baz"
--
-- >>> sepConcat "#" ["a","b","c","d","e"]
-- "a#b#c#d#e"

sepConcat :: String -> [String] -> String
sepConcat _ []     = ""
sepConcat sep (x:xs) = foldLeft f base l
  where
    f a x            = a ++ sep ++ x
    base             = x
    l                = xs

intString :: Int -> String
intString = show

--------------------------------------------------------------------------------
-- | `stringOfList pp [x1,...,xn]` uses the element-wise printer `pp` to
--   convert the element-list into a string:
--
-- >>> stringOfList intString [1, 2, 3, 4, 5, 6]
-- "[1, 2, 3, 4, 5, 6]"
--
-- >>> stringOfList (\x -> x) ["foo"]
-- "[foo]"
--
-- >>> stringOfList (stringOfList show) [[1, 2, 3], [4, 5], [6], []]
-- "[[1, 2, 3], [4, 5], [6], []]"

stringOfList :: (a -> String) -> [a] -> String
stringOfList f xs = "[" ++ (sepConcat ", " (map f xs )) ++ "]"

--------------------------------------------------------------------------------
-- | `clone x n` returns a `[x,x,...,x]` containing `n` copies of `x`
--
-- >>> clone 3 5
-- [3,3,3,3,3]
--
-- >>> clone "foo" 2
-- ["foo", "foo"]

clone :: a -> Int -> [a]
clone x n = if (n > 0)
            then ([x] ++ clone x (n-1))
            else ([])

type BigInt = [Int]

--------------------------------------------------------------------------------
-- | `padZero l1 l2` returns a pair (l1', l2') which are just the input lists,
--   padded with extra `0` on the left such that the lengths of `l1'` and `l2'`
--   are equal.
--
-- >>> padZero [9,9] [1,0,0,2]
-- [0,0,9,9] [1,0,0,2]
--
-- >>> padZero [1,0,0,2] [9,9]
-- [1,0,0,2] [0,0,9,9]

padZero :: BigInt -> BigInt -> (BigInt, BigInt)
padZero l1 l2 
  | (length l1) > (length l2) = (l1, (clone 0 ((length l1)-(length l2))) ++ l2)
  | (length l1) < (length l2) = ((clone 0 ((length l2)-(length l1))) ++ l1, l2)
  | otherwise                 = (l1, l2)

--------------------------------------------------------------------------------
-- | `removeZero ds` strips out all leading `0` from the left-side of `ds`.
--
-- >>> removeZero [0,0,0,1,0,0,2]
-- [1,0,0,2]
--
-- >>> removeZero [9,9]
-- [9,9]
--
-- >>> removeZero [0,0,0,0]
-- []

removeZero :: BigInt -> BigInt
removeZero [] = []
removeZero (x:xs) = if (x == 0)
                    then (removeZero xs)
                    else (x:xs)


--------------------------------------------------------------------------------
-- | `bigAdd n1 n2` returns the `BigInt` representing the sum of `n1` and `n2`.
--
-- >>> bigAdd [9, 9] [1, 0, 0, 2]
-- [1, 1, 0, 1]
--
-- >>> bigAdd [9, 9, 9, 9] [9, 9, 9]
-- [1, 0, 9, 9, 8]
addTwoLists :: BigInt -> BigInt -> BigInt--Requires that they be of the same length
addTwoLists [] []         = []
addTwoLists (a:ab) []     = (a:ab)
addTwoLists [] (a:ab)     = (a:ab)
addTwoLists (a:ab) (c:cd) = if(length(a:ab) == length(c:cd))
                            then([a+c] ++ addTwoLists (ab) (cd))
                            else(addTwoLists (fst (padZero (a:ab) (c:cd))) (snd (padZero (a:ab) (c:cd))))

expandListElem :: Int  -> [Int]
expandListElem x    = if (x > 9)
                          then ([-(div x 10),(mod x 10)])-- negative means carry over
                          else ([x])

addNegativeToList :: Int -> [Int] -> [Int]
addNegativeToList y []         = [0-y]
addNegativeToList y (x:xs)
  | length (x:xs) == 1  = expandListElem (x+(0-y))
  | otherwise           = (x:[(head xs) + (0-y)])--Always presumes just a length of two in the list


addSmallListToBig :: [Int] -> [Int] -> [Int]
addSmallListToBig [] []     = []
addSmallListToBig (x:xs) [] = (x:xs)
addSmallListToBig [] (x:xs) = (x:xs)
addSmallListToBig (x:xs) (y:ys)------------------(HeadBase:TailBase) (HeadSmall:TailSmall)
  | x < 0              = (addSmallListToBig (xs) (addNegativeToList x (y:ys)))
  | otherwise            = ((y:ys) ++ (x:xs))


bigAdd :: BigInt -> BigInt -> BigInt
bigAdd l1 l2     = removeZero res
  where
    (l1', l2')   = padZero l1 l2
    res          = foldLeft f base args
    f a x        = addSmallListToBig a (expandListElem x)
    base         = []
    args         = (reverse (addTwoLists l1' l2')) ++ [0]


--------------------------------------------------------------------------------
-- | `mulByInt i n` returns the result of multiplying
--   the int `i` with `BigInt` `n`.
--
-- >>> mulByInt 9 [9,9,9,9]
-- [8,9,9,9,1]
intToList :: Int -> [Int]
intToList x 
  | x > 9     = (intToList (div x 10)) ++ (intToList (mod x 10))
  | otherwise = [x]

mulByInt :: Int -> BigInt -> BigInt
mulByInt i n 
  | i > 9     = bigMul (intToList i) n --Uses bigMul when attempting to multiply a multi-digit int
  | otherwise = foldLeft f base args
    where
      f a x =  addSmallListToBig a (expandListElem (x * i))
      base  = []
      args  = (reverse n) ++ [0]

--------------------------------------------------------------------------------
-- | `bigMul n1 n2` returns the `BigInt` representing the product of `n1` and `n2`.
--
-- >>> bigMul [9,9,9,9] [9,9,9,9]
-- [9,9,9,8,0,0,0,1]
--
-- >>> bigMul [9,9,9,9,9] [9,9,9,9,9]
-- [9,9,9,9,8,0,0,0,0,1]

halfPadZero :: BigInt -> BigInt -> BigInt --Pads only the first bigint
halfPadZero l1 l2
  | (length l1) < (length l2) = (clone 0 ((length l2)-(length l1))) ++ l1
  | otherwise                 = l1

listsIntoListOfTuples :: [Int] -> [Int] -> [(Int,[Int])]
listsIntoListOfTuples (a:ab) (c:cd) = if (length (a:ab) > 1)
                                        then([(a,(c:cd))] ++ listsIntoListOfTuples (ab) (c:cd))
                                        else([(a,(c:cd))])

bigMul :: BigInt -> BigInt -> BigInt
bigMul l1 l2 = res
  where
    (_, res) = foldLeft f base args
    f a x    = (fst a+1,bigAdd (snd a) ((mulByInt (fst x) (snd x)) ++ clone 0 (fst a)))
    base     = (0,[])
    args     = listsIntoListOfTuples (reverse (halfPadZero l1 l2)) (halfPadZero l2 l1)
