countInt :: (Integral a) => a -> [a] -> a
countInt n [] = 0
countInt n (x:xs)
    | n == x = 1 + countInt n xs
    | otherwise = countInt n xs

countEq :: (Eq a, Integral b) => a -> [a] -> b
countEq n [] = 0
countEq n (x:xs)
    | n == x = 1 + countEq n xs
    | otherwise = countEq n xs

append :: [a] -> [a] -> [a]
append [] [] = []
append (x:xs) [] = x:xs
append [] (x:xs) = x:xs
append (x:xs) (y:ys) = x: append xs (y:ys)

skipEven :: [a] -> [a]
skipEven [] = []
skipEven (_:xs) = skipOdd xs

skipOdd :: [a] -> [a]
skipOdd [] = []
skipOdd (x:xs) = x: skipEven xs

flatten' ::  [[a]] -> [a]
flatten' [] = []
flatten' xss = [x | xs <- xss, x <- xs]

positive n= n >= 0 
nonnegatives :: (Integral a) => [a] -> [a]
nonnegatives = filter positive 

odds n= n `mod` 2 == 1
oddsOnly :: (Integral a) => [a] -> [a]
oddsOnly = filter odds

doubleAll :: (Integral a) => [a] -> [a]
doubleAll [] = []
doubleAll [x] = [2 * x]
doubleAll (x:xs) = (2 * x): doubleAll xs

commafy :: [Int] -> String
commafy [] = ""
commafy [x] = show x
commafy (x:xs) = (show x) ++ ',': commafy xs

