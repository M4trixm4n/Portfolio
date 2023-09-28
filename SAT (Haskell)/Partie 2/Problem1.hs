-- |
-- -- @Module : Problem1
-- -- @Brief : Contient les formules du problème 1
-- -- @Date : 2023
-- -- @Description : Partie 2 du sujet Haskell
module Problem1
  ( interp,
    indis,
    s0,
    aliceIgn,
    bobIgn,
    fatherAnn,
    after,
    problem1,
  )
where

import EL

-- |
-- -- @Name : interp
-- -- @Args : Prend une proposition
-- -- @Return : Retourne un ensemble de monde
-- -- @Brief : Liste l'ensemble possible que la proposition possède
interp :: Prop -> [World]
interp "as" = [10, 11]
interp "bs" = [01, 11]
interp _ = []

-- |
-- -- @Name : indis
-- -- @Args : Prend une agent et monde
-- -- @Return : Retourne un ensemble de monde
-- -- @Brief : Liste l'ensemble possible que l'agent peut interpreter
indis :: Agent -> World -> [World]
indis "a" 00 = [00, 10]
indis "a" 10 = [00, 10]
indis "b" 00 = [00, 01]
indis "b" 01 = [00, 01]
indis "a" 01 = [01, 11]
indis "a" 11 = [01, 11]
indis "b" 10 = [10, 11]
indis "b" 11 = [10, 11]
indis _ _ = []

-- |
-- -- @Name : S0
-- -- @Args : Rien
-- -- @Return : Retourne un epiState
-- -- @Brief : Epistate de l'énoncé
s0 :: EpiState
s0 = (interp, indis, 01)

-- |
-- -- @Name : fatherAnn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La phrase qui dit que au moin un des deux est sale
fatherAnn :: EpiFormula
fatherAnn = Not (Eqv (Var "as") (Var "bs"))

-- |
-- -- @Name : aliceIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La phrase qui dit que alice sait si bob est sale mais pas si elle est elle-même sale
aliceIgn :: EpiFormula
aliceIgn = Not (Knows "a" (Var "as"))

-- |
-- -- @Name : bobIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La phrase qui dit que bob sait si alice est sale mais pas si il est lui-même sale
bobIgn :: EpiFormula
bobIgn = Not (Knows "b" (Var "bs"))

-- |
-- -- @Name : after
-- -- @Args : Rien
-- -- @Return : Retourne une epiFormule
-- -- @Brief : Sous partie du probleme 1
after :: EpiFormula
after = After fatherAnn (After (And aliceIgn (Not bobIgn)) (Not (And aliceIgn bobIgn)))

-- |
-- -- @Name : problem1
-- -- @Return :  Retourne une epiFormule
-- -- @Brief : La formule du problème
problem1 :: EpiFormula
problem1 = And (And bobIgn aliceIgn) after