-- |
-- -- @Module : Problem3
-- -- @Brief : Contient les formules du problème 3
-- -- @Date : 2023
-- -- @Description : Partie 2 du sujet Haskell
module Problem3
  ( interp,
    indis,
    s0,
    aliceIgn,
    bobIgn,
    fatherAnn,
    problem3,
  )
where

import EL

-- |
-- -- @Name : interp
-- -- @Args : Prend une proposition
-- -- @Return : Retourne un ensemble de monde
-- -- @Brief : Liste l'ensemble possible que la proposition possède
interp :: Prop -> [World]
interp "as" = [100, 110, 101, 111]
interp "bs" = [010, 011, 110, 111]
interp "cs" = [001, 011, 101, 111]
interp _ = []

-- |
-- -- @Name : indis
-- -- @Args : Prend une agent et monde
-- -- @Return : Retourne un ensemble de monde
-- -- @Brief : Liste l'ensemble possible que l'agent peut interpreter
indis :: Agent -> World -> [World]
indis "a" 000 = [100, 000]
indis "a" 001 = [001, 101]
indis "a" 010 = [010, 110]
indis "a" 011 = [011, 111]
indis "a" 100 = [100, 000]
indis "a" 101 = [101, 001]
indis "a" 110 = [110, 010]
indis "a" 111 = [111, 011]
indis "b" 000 = [000, 010]
indis "b" 001 = [001, 011]
indis "b" 010 = [010, 000]
indis "b" 011 = [011, 001]
indis "b" 100 = [100, 110]
indis "b" 101 = [101, 111]
indis "b" 110 = [110, 100]
indis "b" 111 = [111, 101]
indis "c" 000 = [000, 001]
indis "c" 001 = [001, 000]
indis "c" 010 = [010, 011]
indis "c" 011 = [011, 010]
indis "c" 100 = [100, 101]
indis "c" 101 = [101, 100]
indis "c" 110 = [110, 111]
indis "c" 111 = [111, 110]
indis _ _ = []

-- |
-- -- @Name : S0
-- -- @Args : Rien
-- -- @Return : Retourne un epiState
-- -- @Brief : Epistate de l'énoncé
s0 :: EpiState
s0 = (interp, indis, 111)

-- |
-- -- @Name : fatherAnn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La phrase qui dit que au moin un des deux est sale
fatherAnn :: EpiFormula
fatherAnn = Or (Var "as") (Var "bs")

-- |
-- -- @Name : aliceIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La phrase qui dit que alice sait si bob ou caroline est sale mais pas si elle est elle-même sale
aliceIgn :: EpiFormula
aliceIgn = And (And (Not (Knows "a" (Var "as"))) (Knows "a" (Var "bs"))) (Knows "a" (Var "cs"))

-- |
-- -- @Name : bobIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief : La phrase qui dit que bob sait si alice ou caroline est sale mais pas si il est lui-même sale
bobIgn :: EpiFormula
bobIgn = And (And (Not (Knows "b" (Var "bs"))) (Knows "b" (Var "as"))) (Knows "b" (Var "cs"))

-- |
-- -- @Name : carolineIgn
-- -- @Return : Retourne une epiFormule
-- -- @Brief :La phrase qui dit que alice sait si alice ou bob est sale mais pas si elle est elle-même sale
carolineIgn :: EpiFormula
carolineIgn = And (And (Not (Knows "c" (Var "cs"))) (Knows "c" (Var "as"))) (Knows "c" (Var "bs"))

-- |
-- -- @Name : after
-- -- @Args : Rien
-- -- @Return : Retourne une epiFormule
-- -- @Brief : Sous partie du probleme 3
after :: EpiFormula
after = And (And aliceIgn (And bobIgn carolineIgn)) after2

-- |
-- -- @Name : after2
-- -- @Args : Rien
-- -- @Return : Retourne une epiFormule
-- -- @Brief : Sous partie 2 du probleme 3
after2 :: EpiFormula
after2 = After (And aliceIgn (And bobIgn carolineIgn)) (And (And (Not aliceIgn) (Not bobIgn)) (Not carolineIgn))

-- |
-- -- @Name : problem3
-- -- @Return :  Retourne une epiFormule
-- -- @Brief : La formule du problème 3
problem3 :: EpiFormula
problem3 = And (And aliceIgn (And bobIgn carolineIgn)) (After fatherAnn after)
