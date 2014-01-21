{-
 - Interpreter:
 -   lambda-calculus interpreter in Haskell
 -   accompanying file: lam.cpp
 -}

data Term = Var String
          | Lam String Term
          | Ap Term Term
          | IntConst Int
          deriving (Show)

data Value = Closure (Value -> Value)
           | IntValue Int

instance Show Value where
  show (Closure _)  = "<fun>"
  show (IntValue n) = show n

eval :: [(String, Value)] -> Term -> Value
eval env (IntConst n) = IntValue n
eval env (Var x) = val
  where Just val = lookup x env
eval env (Lam x e) = Closure (\val -> eval ((x,val):env) e)
eval env (Ap e1 e2) =
  case (eval env e1, eval env e2) of
    (Closure fun, val) -> fun val
    _ -> error "type error"

test1 = Lam "x" (Lam "y" (Var "x"))
test2 = Ap (Ap test1 (IntConst 5)) (IntConst 8)
test3 = Ap test1 (IntConst 7)
