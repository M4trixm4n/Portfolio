-- |
-- -- @Module : EL
-- -- @Brief : Contient les types et fonctions permettant de créer et résoudre des formules.
-- -- @Date : 2023
-- -- @Description : Partie 2 du sujet Haskell
module EL
  ( Prop,
    Agent,
    World,
    EpiState,
    EpiFormula (..),
    epiSat,
    update,
    test,
    testAll,
    testEpiSat,
    testUpdate,
  )
where

-----------------------------------------------------------------------
--                           Les Types                               --
-----------------------------------------------------------------------

-- |
-- -- @Name : EpiFormula
-- -- @Brief : Contient des prototype de formules pour créer des formules logiques
data EpiFormula
  = T
  | F
  | Or EpiFormula EpiFormula
  | And EpiFormula EpiFormula
  | Not EpiFormula
  | Eqv EpiFormula EpiFormula
  | Imp EpiFormula EpiFormula
  | Var String
  | Knows Agent EpiFormula
  | After EpiFormula EpiFormula
  deriving (Eq, Show)

-- |
-- -- @Name : Prop
-- -- @Brief : Type de la proposition
type Prop = String

-- |
-- -- @Name : Agent
-- -- @Brief : Type de l'agent
type Agent = String

-- |
-- -- @Name : World
-- -- @Brief : Type du monde
type World = Int

-- |
-- -- @Name : EpiState
-- -- @Brief : Type quui défini l'épistate
type EpiState = (Prop -> [World], Agent -> World -> [World], World)

-- |
-- -- @Name : epiSatKnows
-- -- @Args : Prend en argument un Epistate, un ensemble de monde ainsi qu'une epiformule
-- -- @Return : Retourne vrai ou faux
-- -- @Brief : Fonction intermédiare du epiSat
epiSatKnows :: EpiState -> [World] -> EpiFormula -> Bool
epiSatKnows _ [] f = True
epiSatKnows (interp, indis, w) (ws : wss) f
  | not (epiSat (interp, indis, ws) f) = False
  | otherwise = epiSatKnows (interp, indis, w) wss f

-- |
-- -- @Name : epiSat
-- -- @Args : Prend en argument un Epistate et une epiFormule
-- -- @Return : Retourne vrai ou faux
-- -- @Brief : Retourne si la formule est vrai ou faux
epiSat :: EpiState -> EpiFormula -> Bool
epiSat _ T = True
epiSat _ F = False
epiSat (interp, _, w) (Var a)
  | w `elem` interp a = True
  | otherwise = False
epiSat e (Or phi psi) = epiSat e phi || epiSat e psi
epiSat e (And phi psi) = epiSat e phi && epiSat e psi
epiSat e (Not phi) = not (epiSat e phi)
epiSat e (Eqv phi psi) = epiSat e phi == epiSat e psi
epiSat e (Imp phi psi) = not (epiSat e phi) || epiSat e psi
epiSat (interp, indis, w) (Knows a phi) = epiSatKnows (interp, indis, w) (indis a w) phi
epiSat e (After phi psi) = epiSat e phi && epiSat (update e phi) psi

-- |
-- -- @Name : update
-- -- @Args : Prend en argument un Epistate et une epiFormule
-- -- @Return : Retourne un nouveau EpiState
-- -- @Brief : Met à jour l'epiState
update :: EpiState -> EpiFormula -> EpiState
update (interp, indis, w) f =
  ( fmap (filter (\x -> epiSat (interp, indis, x) f)) interp,
    fmap (fmap (filter (\y -> epiSat (interp, indis, y) f))) indis,
    w
  )

-- |
-- -- @Name : testEpiSat
-- -- @Brief : Permet de tester un ensemble de test
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : si les tests sont tous valides
testEpiSat :: [Bool]
testEpiSat = [True]

-- |
-- -- @Name : testUpdate
-- -- @Brief : Permet de tester un ensemble de test
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : si les tests sont tous valides
testUpdate :: [Bool]
testUpdate = [True]

-- |
-- -- @Name : test
-- -- @Brief : Permet de tester un ensemble de test
-- -- @Arguments : Un tableau de test ([Bool])
-- -- @Return : si les tests sont tous valides
test :: [Bool] -> Bool
test [] = True
test (l : ls)
  | null ls = True
  | not l = False
  | otherwise = test ls

-- |
-- -- @Name : testAll
-- -- @Brief : Permet de dire si les tests sont valides ou non
-- -- @Arguments : Rien
-- -- @Return : Le message du test
testAll :: [Char]
testAll
  | not
      (test testEpiSat && test testUpdate) =
    "Failure"
  | otherwise = "Succes"