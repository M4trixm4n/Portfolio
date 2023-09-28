-- |
-- -- @Module : Cha4
-- -- @Brief : Module avec les fonctions nécéssaire pour créer le challenge 4.
-- -- @Date : 2023
-- -- @Description : Contient l'ensemble des contraintes pour l'épreuve 4.
module Cha4
  ( challenge4,
  )
where

import CPL

-- |
-- -- @Name : door1
-- -- @Brief : Condition de la porte 1
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door1 :: Formula
door1 = And (Not (Var "t1")) (Not (Var "t2"))

-- |
-- -- @Name : door2
-- -- @Brief : Condition de la porte 2
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door2 :: Formula
door2 = Var "p1"

-- |
-- -- @Name : constraint1
-- -- @Brief : est une fonction générant une formule permettant de définir la contrainte 1  du challenge4
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint1 :: Formula
constraint1 = And (Imp (Var "p1") door1) (Imp (Var "t1") (Not door1))

-- |
-- -- @Name : constraint2
-- -- @Brief : est une fonction générant une formule permettant de définir la contrainte 2  du challenge4
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint2 :: Formula
constraint2 = And (Imp (Var "p2") (Not door2)) (Imp (Var "t2") door2)

-- |
-- -- @Name --> challenge4
-- -- @Brief --> est une fonction générant le challenge 4
-- -- @Arguments --> Aucun
-- -- @Return --> Une formule
challenge4 :: Formula
challenge4 = And constraint1 constraint2