-- |
-- -- @Module : Problem4
-- -- @Brief : Contient les formules du problème 4
-- -- @Date : 2023
-- -- @Description : Partie 2 du sujet Haskell
module Problem4
  ( interp,
    indis,
    anneIgn,
    anneKnows,
    billIgn,
    problem4,
  )
where

import EL

-- |
-- -- @Name : interp
-- -- @Args : Prend une proposition
-- -- @Return : Retourne un ensemble de monde
-- -- @Brief : Liste l'ensemble possible que la proposition possède
interp :: Prop -> [World]
interp "a0" = [01]
interp "a1" = [10, 12]
interp "a2" = [21, 23]
interp "a3" = [32, 34]
interp "a4" = [43]
interp "b0" = [10]
interp "b1" = [01, 21]
interp "b2" = [12, 32]
interp "b3" = [23, 43]
interp "b4" = [34]
interp _ = []

-- |
-- -- @Name : indis
-- -- @Args : Prend une agent et monde
-- -- @Return : Retourne un ensemble de monde
-- -- @Brief : Liste l'ensemble possible que l'agent peut interpreter
indis :: Agent -> World -> [World]
indis "a" 01 = [01]
indis "a" 10 = [10, 12]
indis "a" 12 = [12, 10]
indis "a" 21 = [21, 23]
indis "a" 23 = [23, 21]
indis "a" 32 = [32, 34]
indis "a" 43 = [43]
indis "a" 34 = [34, 32]
indis "b" 01 = [01]
indis "b" 10 = [10, 12]
indis "b" 12 = [12, 10]
indis "b" 21 = [21, 23]
indis "b" 23 = [23, 21]
indis "b" 32 = [32, 34]
indis "b" 43 = [43]
indis "b" 34 = [34, 32]
indis _ _ = []

-- |
-- -- @Name : S0
-- -- @Args : Rien
-- -- @Return : Retourne un epiState
-- -- @Brief : Epistate de l'énoncé
s0 :: EpiState
s0 = (interp, indis, 12)

-- |
-- -- @Name : anneIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La formule qui dit que anne ne connait pas le nombre de bob
anneIgn :: EpiFormula
anneIgn = Not (Knows "a" (And (Var "b0") (And (Var "b1") (And (Var "b2") (And (Var "b3") (Var "b4"))))))

-- |
-- -- @Name : anneIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La formule qui dit que bob ne connait pas le nombre de anne
billIgn :: EpiFormula
billIgn = Not (Knows "b" (And (Var "a0") (And (Var "a1") (And (Var "a2") (And (Var "a3") (Var "a4"))))))

-- |
-- -- @Name : anneKnows
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La formule qui dit que anne connait le nombre de bill
anneKnows :: EpiFormula
anneKnows = Knows "a" (Or (Var "b0") (Or (Var "b1") (Or (Var "b2") (Or (Var "b3") (Var "b4")))))

-- |
-- -- @Name : problem4
-- -- @Return :  Retourne une epiFormule
-- -- @Brief : La formule du problème 4
problem4 :: EpiFormula
problem4 = And (And billIgn anneIgn) (After (After (Knows "b" anneIgn) (Knows "a" billIgn)) anneKnows)