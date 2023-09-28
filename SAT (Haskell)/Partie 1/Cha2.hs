-- |
-- -- @Module : Cha2
-- -- @Brief : Module avec les fonctions nécéssaire pour créer le challenge 2.
-- -- @Date : 2023
-- -- @Description : Contient l'ensemble des contraintes pour l'épreuve 2
module Cha2
  ( challenge2,
  )
where

import CPL

-- |
-- -- @Name : door1
-- -- @Brief : Condition de la porte 1
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door1 :: Formula
door1 = Or (Var "p1") (Var "p2")

-- |
-- -- @Name : door2
-- -- @Brief : Condition de la porte 2
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door2 :: Formula
door2 = Var "t1"

-- |
-- -- @Name --> challenge2
-- -- @Brief --> est une fonction générant le challenge 2
-- -- @Arguments --> Aucun
-- -- @Return --> Une formule
challenge2 :: Formula
challenge2 = Or (And door1 door2) (And (Not door1) (Not door2))