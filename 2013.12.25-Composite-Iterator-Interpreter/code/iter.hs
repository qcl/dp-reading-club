{-
 - Iterator:
 -   zipper demo for lists
 -}

data ListIter a = Iter [a] [a] -- zipper
                deriving (Show)

isDone (Iter _ []) = True
isDone (Iter _ _)  = False

begin xs = Iter [] xs
end   xs = Iter (reverse xs) []

deref (Iter _ (a:_)) = a
next  (Iter before (a:rest)) = Iter (a:before) rest
prev  (Iter (a:before) rest) = Iter before (a:rest)

{-

*Main> let it' = begin[1,2,3,4]
*Main> deref it'
1
*Main> let it'' = next it'
*Main> deref it''
2
*Main> let it''' = prev it''
*Main> deref it'''
1
*Main> let it'''' = next it''
*Main> deref it''''
3
*Main> it'
Iter [] [1,2,3,4]
*Main> it''
Iter [1] [2,3,4]
*Main> it'''
Iter [] [1,2,3,4]
*Main> it''''
Iter [2,1] [3,4]

-}
