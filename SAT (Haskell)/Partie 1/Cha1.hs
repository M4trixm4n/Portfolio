-- |
-- -- @Module : Cha1
-- -- @Brief : Module avec les fonctions nécéssaire pour créer le challenge 1.
-- -- @Date : 2023
-- -- @Description : Contient l'ensemble des contraintes pour l'épreuve 1
module Cha1
  ( challenge1,
  )
where

import CPL

-- |
-- -- @Name : door1
-- -- @Brief : Condition de la porte 1
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door1 :: Formula
door1 = And (Var "p1") (Var "t2")

-- |
-- -- @Name : door2
-- -- @Brief : Condition de la porte 1
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door2 :: Formula
door2 = And (Or (Var "p1") (Var "p2")) (Or (Var "t1") (Var "t2"))

-- |
-- -- @Name : constraint
-- -- @Brief : est une fonction générant une formule permettant de définir si oui ou non deux élements sont présent dans la même cellule ou non.
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint :: Formula
constraint = And (Not (And (Var "t1") (Var "p1"))) (Not (And (Var "p2") (Var "t2")))

-- |
-- -- @Name --> reglement
-- -- @Brief --> est une fonction permettant de vérifier qu'une porte ment et une autre ne ment pas.
-- -- @Arguments --> Aucun
-- -- @Return --> Une formule
reglement :: Formula
reglement = Or (And door1 (Not door2)) (And door2 (Not door1))

-- |
-- -- @Name --> challenge1
-- -- @Brief --> est une fonction générant le challenge 1
-- -- @Arguments --> Aucun
-- -- @Return --> Une formule
challenge1 :: Formula
challenge1 = And constraint reglement
