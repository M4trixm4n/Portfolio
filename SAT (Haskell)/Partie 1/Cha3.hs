-- |
-- -- @Module : Cha3
-- -- @Brief : Module avec les fonctions nécéssaire pour créer le challenge 3.
-- -- @Date : 2023
-- -- @Description : Contient l'ensemble des contraintes pour l'épreuve 3
module Cha3
  ( challenge3,
  )
where

import CPL

-- |
-- -- @Name : door1
-- -- @Brief : Condition de la porte 1
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door1 :: Formula
door1 = And (Or (Var "t1") (Var "p2")) (Not (And (Var "t1") (Var "p2")))

-- |
-- -- @Name : door2
-- -- @Brief : Condition de la porte 2
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door2 :: Formula
door2 = Var "p1"

-- |
-- -- @Name --> challenge3
-- -- @Brief --> est une fonction générant le challenge 3
-- -- @Arguments --> Aucun
-- -- @Return --> Une formule
challenge3 :: Formula
challenge3 = Not (Eqv (And door1 door2) (And (Not door1) (Not door2)))