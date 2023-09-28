-- |
-- -- @Module : Cha6
-- -- @Brief : Module avec les fonctions nécéssaire pour créer le challenge .
-- -- @Date : 2023
-- -- @Description : Contient l'ensemble des contraintes pour l'épreuve 6.
module Cha6
  ( challenge6,
  )
where

import CPL
import Data.ByteString (cons)

-- |
-- -- @Name : door1
-- -- @Brief : Condition de la porte 1
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door1 :: Formula
door1 = Var "t1"

-- |
-- -- @Name : door2
-- -- @Brief : Condition de la porte 2
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door2 :: Formula
door2 = Var "p2"

--  |
-- -- @Name : door3
-- -- @Brief : Condition de la porte 3
-- -- @Arguments : Rien
-- -- @Return : Retourne une formule
door3 :: Formula
door3 = Var "t2"

-- |
-- -- @Name : constraint1_1
-- -- @Brief : est une fonction générant une formule aidant à définir la contrainte 1  du challenge 6
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint1_1 :: Formula
constraint1_1 = And (And (And (Var "t1") (Not (Var "p1"))) (And (Var "t3") (Not (Var "p3")))) (And (Var "p2") (Not (Var "t2")))

-- |
-- -- @Name : constraint1_2
-- -- @Brief : est une fonction générant une formule aidant à définir la contrainte 1  du challenge 6
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint1_2 :: Formula
constraint1_2 = And (And (And (Var "t3") (Not (Var "p3"))) (And (Var "t2") (Not (Var "p2")))) (And (Var "p1") (Not (Var "t1")))

-- |
-- -- @Name : constraint1_3
-- -- @Brief : est une fonction générant une formule aidant à définir la contrainte 1  du challenge 6
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint1_3 :: Formula
constraint1_3 = And (And (And (Var "t1") (Not (Var "p1"))) (And (Var "t2") (Not (Var "p2")))) (And (Var "p3") (Not (Var "t3")))

-- |
-- -- @Name : constraint1
-- -- @Brief : est une fonction générant une formule permettant de définir la contrainte 1  du challenge 6
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint1 :: Formula
constraint1 = Not (Eqv (Not (Eqv constraint1_1 constraint1_2)) constraint1_3)

-- |
-- -- @Name : constraint2
-- -- @Brief : est une fonction générant une formule permettant de définir la contrainte 2  du challenge 6
-- -- @Arguments : Aucun
-- -- @Return : Une formule
constraint2 :: Formula
constraint2 = Not (Eqv (Not (Eqv (And (And door1 (Not door2)) (Not door3)) (And (And (Not door1) door2) (Not door3)))) (And (And (Not door1) (Not door2)) door3))

-- |
-- -- @Name --> challenge6
-- -- @Brief --> est une fonction générant le challenge 6
-- -- @Arguments --> Aucun
-- -- @Return --> Une formule
challenge6 :: Formula
challenge6 = And constraint1 constraint2