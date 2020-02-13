module Hw4 where

data LTree a = LLeaf a | LNode (LTree a) a (LTree a) deriving Show
--mytree0 = LNode (LLeaf 0) 1 (LLeaf 2)
--mytree1 = LNode (LLeaf 5) 6 (LLeaf 7)
--mytree2 = LNode mytree0 100 mytree1
mytree0 = LLeaf 0
mytree2 = LNode (LNode (LLeaf 0) 1 (LLeaf 2))
                100
                (LNode (LLeaf 5) 6 (LLeaf 7))
mytree3 = (LNode (LNode (LNode mytree2 0 mytree2) 0 mytree2) 0 mytree2)

minTree :: LTree Integer -> Integer
minTree (LLeaf x) = x
minTree (LNode t1 y t2) = (min y (min (minTree t1) (minTree t2)))

countLeaves :: LTree a -> Integer
countLeaves (LLeaf x) = 1
countLeaves (LNode t1 x t2) = (countLeaves t1) + (countLeaves t2)

countEvenNodes :: LTree Integer -> Integer
countEvenNodes (LLeaf x) = 0
countEvenNodes (LNode t1 y t2) = (countEvenNodes t1) + (if y `mod` 2 == 0 then 1 else 0) + (countEvenNodes t2)

occursInLeaves :: (Eq a) => a -> LTree a -> Bool
occursInLeaves x (LLeaf t) = (if x == t then True else False)
occursInLeaves x (LNode t1 t t2) = (if (occursInLeaves x t1 == True) || (occursInLeaves x t2 == True) then True else False)

checkNoCover :: (Eq a) => a -> LTree a -> Bool
checkNoCover x (LLeaf t) = (if x == t then True else False)
checkNoCover x (LNode t1 t t2)
    | t == x = False
    | otherwise = checkNoCover x t1 || checkNoCover x t2

foldTree :: (b -> a -> b -> b) -> (a -> b) -> LTree a -> b
foldTree comb base (LLeaf x) = base x
foldTree comb base (LNode t1 y t2) = comb (foldTree comb base t1)
                                          y
                                          (foldTree comb base t2)
minTree2 :: LTree Integer -> Integer
minTree2 = foldTree (\x1 x x2 -> min x (min x1 x2)) id


countLeaves2 :: LTree a -> Integer
countLeaves2 = foldTree (\x1 x x2 -> x1 + x2) (\x -> 1)


countEvenNodes2 :: LTree Integer -> Integer
countEvenNodes2 = foldTree (\x1 x x2 -> x1 + (if x `mod` 2 == 0 then 1 else 0) + x2) (\x -> 0)


occursInLeaves2 :: (Eq a) => a -> LTree a -> Bool
occursInLeaves2 k = foldTree (\x1 x x2 -> x1 || x2) (\x -> k == x) 


checkNoCover2 :: (Eq a) => a -> LTree a -> Bool
checkNoCover2 k = foldTree (\x1 x x2 -> (if k == x then False else x1 || x2)) (\x -> k == x)

